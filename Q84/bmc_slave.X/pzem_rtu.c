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

static void pzem_data_handler(void);
static void pzem_dir_handler(void);

static void half_dup_tx(const bool);
static void half_dup_rx(const bool);

static const uint8_t
// transmit frames for commands
modbus_pz_data1[] = {PZMADDR, READ_INPUT_REGISTERS, 0x00, 0x00, 0x00, PZ_DATA_LEN1},
modbus_pz_hz60[] = {PZMADDR, WRITE_PZEM_REGISTER, 0x00, PZEM_FREQUENCY_SYSTEM_REG, 0x00, 0x01, 0x02, 0x00, PZEM_FREQUENCY_60HZ}, // register 0x0002, data 0x0001
modbus_pz_hz50[] = {PZMADDR, WRITE_PZEM_REGISTER, 0x00, PZEM_FREQUENCY_SYSTEM_REG, 0x00, 0x01, 0x02, 0x00, PZEM_FREQUENCY_50HZ},
// receive frames prototypes for received data checking
pz_data1[(PZ_DATA_LEN1 * 2) + 5] = {PZMADDR, READ_INPUT_REGISTERS, 0x00},
pz_hz1[(PZ_DATA_HZ1 * 2) + 6] = {PZMADDR, WRITE_PZEM_REGISTER, 0x00};

/*
 * register data frames
 */
PZ_data1 pz, *pz_ptr;
PZ_tmp pz_tmp;

/*
 * state machine hardware timers interrupt ISR functions setup
 */
void init_pz_mb_master_timers(void)
{
	cc_buffer = cc_buffer_0;
	pz_ptr = (PZ_data1*) & cc_buffer[3];
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
		if ((client->modbus_command == (cmd_type) P_HZ1) && (client->config_ok)) { // skip if we have valid 60Hz set
			client->modbus_command = client->mcmd++;
		}
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
		case P_HZ1: // set line frequency request
			client->trace = T_config;
#ifdef USE50HZ
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_pz_hz50, sizeof(modbus_pz_hz50));
#else
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_pz_hz60, sizeof(modbus_pz_hz60));
#endif
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
				pzem_modbus_read_check(client, &client->data_ok, sizeof(pz_data1), pzem_data_handler);
				break;
			case P_HZ1: // set controller hz1 code to 50/60Hz
				pzem_modbus_write_check(client, &client->config_ok, sizeof(pz_hz1));
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
	em.vl1n = (uint16_t) pz_ptr->vl1n;
	em.vl2n = (uint16_t) pz_ptr->vl2n;
	em.vl3n = (uint16_t) pz_ptr->vl3n;
	em.vl1l2 = (uint16_t) pz_ptr->vl1n;
	em.vl2l3 = (uint16_t) pz_ptr->vl2n;
	em.vl3l1 = (uint16_t) pz_ptr->vl3n;
	em.al1 = (uint16_t) pz_ptr->al1;
	em.al2 = (uint16_t) pz_ptr->al2;
	em.al3 = (uint16_t) pz_ptr->al3;
	em.wl1 = pz_ptr->pap1s;
	em.wl2 = pz_ptr->pap2s;
	em.wl3 = pz_ptr->pap3s;
	em.val1 = pz_ptr->papp1s;
	em.val2 = pz_ptr->papp2s;
	em.val3 = pz_ptr->papp3s;
	em.varl1 = pz_ptr->prp1s;
	em.varl2 = pz_ptr->prp2s;
	em.varl3 = pz_ptr->prp3s;
	em.wsys = (int32_t) pz_ptr->caes;
	em.vasys = (int32_t) pz_ptr->cappes;
	em.varsys = (int32_t) pz_ptr->cres;
	em.pfl1 = (int16_t) pz_ptr->p1p2pf;
	em.pfl2 = (int16_t) pz_ptr->p1p2pf;
	em.pfl3 = (int16_t) pz_ptr->p1p2pf;
	em.pfsys = (int16_t) pz_ptr->p3cpf;
	em.hz = (int16_t) pz_ptr->hz1;
	emt.hz = (int32_t) (((float) em.hz) * 10.0f);
	em_tmp.hz = (float) emt.hz;
	em_tmp.al1 = ((float) em.al1);
	em_tmp.wl1 = (float) em.wl1;
	em_tmp.wl2 = (float) em.wl2;
	em_tmp.wl3 = (float) em.wl3;
	imd_tmp.ap1s = (float) em.wl1; // phase A:1 GTI power
	imd_tmp.ap2s = (float) em.wl2; // Phase B:2 Utility power flow
	imd_tmp.ap3s = (float) em.wl3; // Phase C:3 Load power
	imd_tmp.rpp1s = (float) em.val1; // Reactive power for each Phase input
	imd_tmp.rpp2s = (float) em.val2;
	imd_tmp.rpp3s = (float) em.val3;
	imd_tmp.tps = (float) em.wsys; // Total power
}

static bool pzem_modbus_write_check(C_data * client, bool* cstate, const uint16_t rec_length)
{
	uint16_t c_crc, c_crc_rec;

	client->req_length = rec_length;
	if (DBUG_R((M.recv_count >= client->req_length) && (cc_buffer[0] == PZMADDR) && (cc_buffer[1] == WRITE_PZEM_REGISTER))) {
		c_crc = crc16(cc_buffer, client->req_length - 2);
		c_crc_rec = crc16_receive(client, cc_buffer);
		if (DBUG_R c_crc == c_crc_rec) {
			client->config_ok = true;
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
	if (DBUG_R((M.recv_count >= client->req_length) && (cc_buffer[0] == PZMADDR) && (cc_buffer[1] == READ_INPUT_REGISTERS))) {
		c_crc = crc16(cc_buffer, client->req_length - 2);
		c_crc_rec = crc16_receive(client, cc_buffer);
#ifdef CRC_ERRORS
		client->c_crc = c_crc;
		client->c_crc_rec = c_crc_rec;
		client->c_crc_length = M.recv_count;
#endif
		if (DBUG_R c_crc == c_crc_rec) {
			client->data_ok = true;
			client->id_ok = true;
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
			client->id_ok = false;
			client->config_ok = false;
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
			client->id_ok = false;
			client->config_ok = false;
		}
	}
	return *cstate;
}

static bool pzem_modbus_read_dir_check(C_data * client, bool* cstate, const uint16_t rec_length, void (* DataHandler)(void), const uint8_t command)
{
	uint16_t c_crc, c_crc_rec;

	client->req_length = rec_length;
	if (DBUG_R((M.recv_count >= client->req_length) && (cc_buffer[0] == PZMADDR) && (cc_buffer[1] == command))) {
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
			client->config_ok = false;
			log_crc_error(c_crc, c_crc_rec);
		}
		client->cstate = CLEAR;
	} else {
		if (get_500hz(false) >= IRDELAY) {
			client->cstate = CLEAR;
			client->mcmd = P_DATA1;
			M.to_error++;
			M.error++;
			*cstate = false;
			client->id_ok = false;
			client->config_ok = false;
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
