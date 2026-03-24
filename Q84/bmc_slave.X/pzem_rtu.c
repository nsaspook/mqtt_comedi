#include "pzem_rtu.h"

typedef struct M_time_data { // ISR used, mainly for non-atomic mod problims
	uint32_t clock_500hz;
	uint32_t clock_500ahz;
	uint32_t clock_2hz;
} M_time_data;

static volatile uint8_t cc_stream_file, *cc_buffer, cc_buffer_0[MAX_DATA], cc_buffer_tx[MAX_DATA]; // RX and TX command buffers

static volatile M_data M = {
	.blink_lock = false,
	.power_on = true,
};

static volatile M_time_data MT = {
	.clock_500ahz = 0,
	.clock_2hz = 0,
	.clock_500hz = 0,
};

static const uint16_t ITDELAY = 3; // KWH half-duplex delay
static const uint16_t ITMDELAY = 2; // half-duplex delay
static const uint16_t ITEDELAY = 2; // half-duplex delay
static const uint16_t IRDELAY = 1200; // receive timeout
static const uint16_t ICDELAY = 40; // fast query delay 100ms
static const uint16_t IQDELAY = 2; // slow query delay 1s
static const uint16_t ITODELAY = 4; // misc delay
static const uint16_t ISPACING = 5000; // control loop cpu usage factor
static const uint16_t IDUPL_DELAY = 2; // extra duplex delay mode

static bool pzem_modbus_write_check(C_data *, bool*, uint16_t);
static bool pzem_modbus_read_check(C_data *, bool*, uint16_t, void (* DataHandler)(void));
static bool pzem_modbus_read_dir_check(C_data *, bool*, uint16_t, void (* DataHandler)(void));

static void pzem_data_handler(void);
static void pzem_dir_handler(void);

static void half_dup_tx(const bool);
static void half_dup_rx(const bool);

static const uint8_t
// transmit frames for commands
modbus_pz_data1[] = {MADDR, READ_HOLDING_REGISTERS, 0x00, 0x00, 0x00, PZ_DATA_LEN1},
modbus_pz_dir1[] = {MADDR, READ_HOLDING_REGISTERS, 0x00, 0x00, 0x00, PZ_DATA_DIR1},
// receive frames prototypes for received data checking
im_data1[(PZ_DATA_LEN1 * 2) + 5] = {MADDR, READ_HOLDING_REGISTERS, 0x00},
im_dir1[(PZ_DATA_DIR1 * 2) + 5] = {MADDR, READ_HOLDING_REGISTERS, 0x00};

/*
 * register data frames
 */
PZ_data1 pz, *pz_ptr;
PZ_dir1 pzd, *pzd_ptr;
PZ_tmp pz_tmp;
PZD_tmp pzd_tmp;
PZ_data2 pzt;

/*
 * state machine hardware timers interrupt ISR functions setup
 */
void init_pz_mb_master_timers(void)
{
	cc_buffer = cc_buffer_0;
	pz_ptr = (PZ_data1*) & cc_buffer[3];
	pzd_ptr = (PZ_dir1*) & cc_buffer[3];
	TMR4_SetInterruptHandler(timer_500ms_tick);
	TMR4_StartTimer();
	TMR3_SetInterruptHandler(timer_2ms_tick);
	TMR3_StartTimer();
}

/*
 * we start with ID and the cycle other command sequences
 * Use UART3
 * MODBUS command failure restarts STATE MACHINE to CLEAR case
 */
int8_t pzem_controller_work(C_data * client)
{
	static uint32_t spacing = 0;
	static uint8_t m_data = 0;

	if (spacing++ <ISPACING && (client->cstate != RECV)) {
		return T_spacing;
	}

	spacing = 0;

	client->trace = T_begin;
	switch (client->cstate) {
	case CLEAR:
		client->trace = T_clear;
		clear_2hz();
		clear_500ahz();
		client->cstate = INIT;
		client->modbus_command = client->mcmd++; // sequence MODBUS commands to client
		if (client->mcmd > P_LAST) {
			client->mcmd = P_DATA1;
		}
		/*
		 * command specific TX buffer setup
		 */
		switch (client->modbus_command) {
		case P_DATA1: // read data request
			client->trace = T_data;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_pz_data1, sizeof(modbus_pz_data1));
			break;
		case P_DIR1: // read info request
			client->trace = T_id;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_pz_dir1, sizeof(modbus_pz_dir1));
			break;
		case P_LAST: // end of command sequences
			client->cstate = CLEAR;
			client->mcmd = P_DATA1; // what do we run next
			break;
		default:
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_pz_data1, sizeof(modbus_pz_data1));
			break;
		}
		break;
	case INIT:
		client->trace = T_init;
		/*
		 * MODBUS master query speed
		 */
#ifdef	FASTQ
		if (get_500ahz(false) >= ICDELAY) {
#else
		if (get_2hz(false) >= IQDELAY) {
#endif
#ifndef AUTO_DERE
			half_dup_tx(false); // no delays here
#endif
			M.recv_count = 0;
			client->cstate = SEND;
			clear_500hz();
			client->trace = T_init_d;
		}
		break;
	case SEND:
		client->trace = T_send;
		if (get_500hz(false) >= ITEDELAY) {
			for (uint8_t i = 0; i < client->req_length; i++) {
				Swrite(cc_buffer_tx[i]);
			}
			client->cstate = RECV;
			clear_500hz(); // state machine execute background timer clear
			client->trace = T_send_d;
			M.sends++;
			if (serial_trmt()) { // check for serial UART transmit shift register and buffer empty
				clear_500hz(); // clear timer until buffer empty
			}
#ifndef AUTO_DERE
			DERE_SetLow(); // enable MODBUS receiver
#endif
		}
		break;
	case RECV:
		client->trace = T_recv;
		if (get_500hz(false) >= ITEDELAY) { // state machine execute timer test
			client->trace = T_recv_r;
#ifndef AUTO_DERE
			half_dup_rx(false); // no delays here
#endif
			/*
			 * process received controller data stream
			 */
			if (UART3_is_rx_ready()) {
				m_data = UART3_Read(); // receiver data to buffer
				cc_buffer[M.recv_count] = m_data;
				if (++M.recv_count >= MAX_DATA) {
					M.recv_count = 0; // reset buffer position
				}
			}
			/*
			 * check received response data for size and format for each command sent
			 */
			switch (client->modbus_command) {
			case P_DATA1: // check for controller data1 codes
				pzem_modbus_read_check(client, &client->data_ok, sizeof(im_data1), pzem_data_handler);
				break;
			case P_DIR1: // check for controller dir1 codes
				pzem_modbus_read_dir_check(client, &client->id_ok, sizeof(im_dir1), pzem_dir_handler);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	return client->trace;
}

static void pzem_data_handler(void)
{
	/*
	 * load PZEM data pointer with receive buffer to data structure
	 * and munge the data into the correct local formats for client
	 */
	em_ptr = (EM_data1*) & cc_buffer[3];
	em.vl1n = mb16_swap((int16_t) pz_ptr->vl1n) / 10;
	em.vl2n = mb16_swap((int16_t) pz_ptr->vl2n) / 10;
	em.vl3n = mb16_swap((int16_t) pz_ptr->vl3n) / 10;
	em.vl1l2 = mb16_swap((int16_t) pz_ptr->vl1n) / 10;
	em.vl2l3 = mb16_swap((int16_t) pz_ptr->vl2n) / 10;
	em.vl3l1 = mb16_swap((int16_t) pz_ptr->vl3n) / 10;
	em.al1 = mb16_swap((int16_t) pz_ptr->al1)*10;
	em.al2 = mb16_swap((int16_t) pz_ptr->al2)*10;
	em.al3 = mb16_swap((int16_t) pz_ptr->al3)*10;
	em.wl1 = mb32_swap(pz_ptr->pap1s) / 10000;
	em.wl2 = mb32_swap(pz_ptr->pap2s) / 10000;
	em.wl3 = mb32_swap(pz_ptr->pap3s) / 10000;
	em.val1 = mb32_swap(pz_ptr->prp1s) / 10000;
	em.val2 = mb32_swap(pz_ptr->prp2s) / 10000;
	em.val3 = mb32_swap(pz_ptr->prp3s) / 10000;
	em.varl1 = mb32_swap(pz_ptr->prp1s) / 10000;
	em.varl2 = mb32_swap(pz_ptr->prp2s) / 10000;
	em.varl3 = mb32_swap(pz_ptr->prp3s) / 10000;
	em.wsys = mb32_swap(pz_ptr->caes);
	em.vasys = mb32_swap(pz_ptr->cres);
	em.varsys = mb32_swap(pz_ptr->cappes);
	em.pfl1 = mb16_swap((const int16_t) pz_ptr->p1p2pf);
	em.pfl2 = mb16_swap((const int16_t) pz_ptr->p1p2pf);
	em.pfl3 = mb16_swap((const int16_t) pz_ptr->p1p2pf);
	em.pfsys = mb16_swap((const int16_t) pz_ptr->p3cpf);
	em.hz = mb16_swap((const int16_t) pz_ptr->hz1);
	emt.hz = (int32_t) (((float) em.hz) * 100.0f);
	em_tmp.hz = (float) emt.hz;
	em_tmp.al1 = ((float) em.al1) / 100.0f;
	em_tmp.wl1 = (float) mb32_swap(pz_ptr->pap1s)*100;
	em_tmp.wl2 = (float) mb32_swap(pz_ptr->pap2s)*100;
	em_tmp.wl3 = (float) mb32_swap(pz_ptr->pap3s)*100;
}

static void pzem_dir_handler(void)
{
	pzd_ptr = (PZ_dir1*) & cc_buffer[3];
	//	pzd_tmp.ap1s = (float) mb32_swap(pzd_ptr->ap1s) / 100000; // phase A:1 GTI power
	//	pzd_tmp.ap2s = (float) mb32_swap(pzd_ptr->ap2s) / 100000; // Phase B:2 Utility power flow
	//	pzd_tmp.ap3s = (float) mb32_swap(pzd_ptr->ap3s) / 100000; // Phase C:3 Load power
	//	pzd_tmp.rpp1s = (float) mb32_swap(pzd_ptr->rpp1s) / 100000; // Reactive power for each Phase input
	//	pzd_tmp.rpp2s = (float) mb32_swap(pzd_ptr->rpp2s) / 100000;
	//	pzd_tmp.rpp3s = (float) mb32_swap(pzd_ptr->rpp3s) / 100000;
	//	pzd_tmp.tps = (float) mb32_swap(pzd_ptr->tps) / 100000; // Total power
}

static bool pzem_modbus_write_check(C_data * client, bool* cstate, const uint16_t rec_length)
{
	uint16_t c_crc, c_crc_rec;

	client->req_length = rec_length;
	if (DBUG_R((M.recv_count >= client->req_length) && (cc_buffer[0] == MADDR) && (cc_buffer[1] == WRITE_SINGLE_REGISTER))) {
		c_crc = crc16(cc_buffer, client->req_length - 2);
		c_crc_rec = crc16_receive(client, cc_buffer);
		if (DBUG_R c_crc == c_crc_rec) {
			*cstate = true;
			MM_ERROR_C;
		} else {
			*cstate = false;
			log_crc_error(c_crc, c_crc_rec);
		}
		client->cstate = CLEAR; // where do we go next
		client->mcmd = P_LAST; // what do we run next
	} else {
		if (get_500hz(false) >= IRDELAY) {
			client->cstate = CLEAR; // where do we go next
			client->mcmd = P_DATA1; // what do we run next
			M.to_error++;
			M.error++;
			if (client->data_ok) {
				MM_ERROR_C;
			}
		}
	}
	return *cstate;
}

static bool pzem_modbus_read_check(C_data * client, bool* cstate, const uint16_t rec_length, void (* DataHandler)(void))
{
	uint16_t c_crc, c_crc_rec;

	client->req_length = rec_length;
	if (DBUG_R((M.recv_count >= client->req_length) && (cc_buffer[0] == MADDR) && (cc_buffer[1] == READ_HOLDING_REGISTERS))) {
		c_crc = crc16(cc_buffer, client->req_length - 2);
		c_crc_rec = crc16_receive(client, cc_buffer);
#ifdef CRC_ERRORS
		client->c_crc = c_crc;
		client->c_crc_rec = c_crc_rec;
		client->c_crc_length = M.recv_count;
#endif
		if (DBUG_R c_crc == c_crc_rec) {
			client->data_ok = true;
			*cstate = true;
			/*
			 * move from receive buffer to data structure and munge the data into the correct local 32-bit format from MODBUS client
			 */
			DataHandler();
			client->data_prev = client->data_count;
			client->data_count++;
			MM_ERROR_C;
		} else {
			MM_ERROR_S;
			*cstate = false;
			client->data_ok = false;
			log_crc_error(c_crc, c_crc_rec);
		}
		client->cstate = CLEAR;
	} else {
		if (get_500hz(false) >= IRDELAY) {
			client->cstate = CLEAR;
			MM_ERROR_C;
			client->mcmd = P_DATA1;
			M.to_error++;
			M.error++;
			*cstate = false;
			client->data_ok = false;
		}
	}
	return *cstate;
}

static bool pzem_modbus_read_dir_check(C_data * client, bool* cstate, const uint16_t rec_length, void (* DataHandler)(void))
{
	uint16_t c_crc, c_crc_rec;

	client->req_length = rec_length;
	if (DBUG_R((M.recv_count >= client->req_length) && (cc_buffer[0] == MADDR) && (cc_buffer[1] == READ_HOLDING_REGISTERS))) {
		c_crc = crc16(cc_buffer, client->req_length - 2);
		c_crc_rec = crc16_receive(client, cc_buffer);
		if (DBUG_R c_crc == c_crc_rec) {
			MM_ERROR_C;
			client->id_ok = true;
			*cstate = true;
			/*
			 * move from receive buffer to data structure and munge the data into the correct local 32-bit format from MODBUS client
			 */
			DataHandler();
			client->data_prev = client->data_count;
			client->data_count++;
		} else {
			MM_ERROR_S;
			*cstate = false;
			client->id_ok = false;
			log_crc_error(c_crc, c_crc_rec);
		}
		client->cstate = CLEAR;
	} else {
		if (get_500hz(false) >= IRDELAY) {
			client->cstate = CLEAR;
			client->mcmd = P_DIR1;
			M.to_error++;
			M.error++;
			*cstate = false;
			client->id_ok = false;
		}
	}
	return *cstate;
}

// switch RS transceiver to transmit mode and wait if not TX

static void half_dup_tx(const bool delay)
{
#ifndef AUTO_DERE
	if (DERE_GetValue()) {
		return;
	}
	DERE_SetHigh(); // enable MODBUS transmitter

	if (delay) {
		WaitMs(IDUPL_DELAY); // busy waits
	}
#endif
}

// switch RS transceiver to receive mode and wait if not RX

static void half_dup_rx(const bool delay)
{
#ifndef AUTO_DERE
	if (!DERE_GetValue()) {
		return;
	}
	if (delay) {
		WaitMs(IDUPL_DELAY); // busy waits
	}
	DERE_SetLow(); // enable MODBUS receiver
#endif
}

/*
 * callback for UART received character from MODBUS client
 * for each RX byte received on the RS485 serial port
 * don't share with other drivers
 */
void pz_my_modbus_rx_32(void)
{
	static uint8_t m_data = 0;

	INT_TRACE;
	M.rx = true;
	/*
	 * process received controller data stream
	 */
	m_data = Srbuffer; // receiver data buffer
	cc_buffer[M.recv_count] = m_data; // review the scope of global cc_buffer
	if (++M.recv_count >= MAX_DATA) {
		M.recv_count = 0; // reset buffer position
	}
}

void pzem_version(void)
{
	strncpy(em_info, "PZEM Driver      ", 32);
	strncpy(modbus_name [1], "PZEM", 12);
}