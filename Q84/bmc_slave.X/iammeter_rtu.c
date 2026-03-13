#include "iammeter_rtu.h"

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

static bool iammeter_modbus_write_check(C_data *, bool*, uint16_t);
static bool iammeter_modbus_read_check(C_data *, bool*, uint16_t, void (* DataHandler)(void));
static bool iammeter_modbus_read_id_check(C_data *, bool*, uint16_t);
static bool modbus_read_dcu_check_im(C_data *, bool*, uint16_t);

static void iammeter_data_handler(void);

static void half_dup_tx(const bool);
static void half_dup_rx(const bool);

static const uint8_t
wim_single[] = {0x01, 0x10, 0x00, 0x0C, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0xF3, 0xFA},
wim_three_forward[] = {0x01, 0x10, 0x00, 0x63, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x92},
wim_three_reverse[] = {0x01, 0x10, 0x00, 0x66, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x75, 0xAD},
wim_get_id[] = {0x00, 0x03, 0x00, 0x04, 0x00, 0x01, 0xC4, 0x1A};

static const uint8_t
// transmit frames for commands
modbus_im_data1[] = {MADDR, READ_HOLDING_REGISTERS, 0x00, 0x48, 0x00, IM_DATA_LEN1},
// receive frames prototypes for received data checking
im_data1[(IM_DATA_LEN1 * 2) + 5] = {MADDR, READ_HOLDING_REGISTERS, 0x00};

/*
 * register data frames
 */
IM_data1 im, *im_ptr;
IM_tmp im_tmp;
IM_data2 imt;

/*
 * state machine hardware timers interrupt ISR functions setup
 */
void init_im_mb_master_timers(void)
{
	cc_buffer = cc_buffer_0;
	im_ptr = (IM_data1*) & cc_buffer[3];
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
int8_t iammeter_controller_work(C_data * client)
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
		if (client->mcmd > I_LAST) {
			client->mcmd = I_DATA1;
		}
		/*
		 * command specific TX buffer setup
		 */
		switch (client->modbus_command) {
		case I_DATA1: // read code request
			client->trace = T_data;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_im_data1, sizeof(modbus_im_data1));
			break;
		case I_LAST: // end of command sequences
			client->cstate = CLEAR;
			client->mcmd = I_DATA1; // what do we run next
			break;
		default:
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_im_data1, sizeof(modbus_im_data1));
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
			case I_DATA1: // check for controller data1 codes
				iammeter_modbus_read_check(client, &client->data_ok, sizeof(im_data1), iammeter_data_handler);
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

int8_t reset_iammeter_kwh(C_data * client)
{
	static uint8_t m_data = 0;

	client->trace = T_begin;
	switch (client->cstate) {
	case CLEAR:
		client->trace = T_clear;
		clear_2hz();
		clear_500ahz();
		client->cstate = INIT;
		break;
	case INIT:
		client->trace = T_init;
		/*
		 * command specific TX buffer setup
		 */
		switch (client->resets++) {
		case 0:
			client->req_length = modbus_dcu_send_msg_im((void*) cc_buffer_tx, (const void *) &wim_single, sizeof(wim_single));
			break;
		case 1:
			client->req_length = modbus_dcu_send_msg_im((void*) cc_buffer_tx, (const void *) &wim_three_forward, sizeof(wim_three_forward));
			break;
		case 2:
			client->req_length = modbus_dcu_send_msg_im((void*) cc_buffer_tx, (const void *) &wim_three_reverse, sizeof(wim_three_reverse));
			break;
		default:
			client->req_length = modbus_dcu_send_msg_im((void*) cc_buffer_tx, (const void *) &wim_single, sizeof(wim_single));
			client->resets = 0;
			break;
		}

		/*
		 * RS485 master query speed
		 */

		half_dup_tx(true);
		M.recv_count = 0;
		client->cstate = SEND;
		clear_500hz();
		client->trace = T_init_d;

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
			client->iam_count++;
			M.sends++;
			if (serial_trmt()) { // check for serial UART transmit shift register and buffer empty
				clear_500hz(); // clear timer until buffer empty
			}
			delay_ms(ITDELAY + client->req_length);
			DERE_SetLow(); // enable MODBUS receiver
		}
		break;
	case RECV:
		client->trace = T_recv;
		if (UART3_is_rx_ready()) {
			m_data = UART3_Read(); // receiver data to buffer
			cc_buffer[M.recv_count] = m_data;
			if (++M.recv_count >= MAX_DATA) {
				M.recv_count = 0; // reset buffer position
			}
		}
		if (get_500hz(false) >= ITEDELAY) { // state machine execute timer test
			client->trace = T_recv_r;
			half_dup_rx(false); // no delays here
			modbus_read_dcu_check_im(client, &client->version_ok, 1);
			client->cstate = CLEAR;
		}
		break;
	default:
		break;
	}
	return client->trace;
}

static void iammeter_data_handler(void)
{
	/*
	 * load IAMMETER data pointer with receive buffer to data structure
	 * and munge the data into the correct local formats for client
	 */
	em_ptr = (EM_data1*) & cc_buffer[3];
	em.vl1n = mb32_swap(im_ptr->vl1n) / 10;
	em.vl2n = mb32_swap(im_ptr->vl2n) / 10;
	em.vl3n = mb32_swap(im_ptr->vl3n) / 10;
	em.vl1l2 = mb32_swap(im_ptr->vl1n) / 10;
	em.vl2l3 = mb32_swap(im_ptr->vl2n) / 10;
	em.vl3l1 = mb32_swap(im_ptr->vl3n) / 10;
	em.al1 = mb32_swap(im_ptr->al1)*10;
	em.al2 = mb32_swap(im_ptr->al2)*10;
	em.al3 = mb32_swap(im_ptr->al3)*10;
	em.wl1 = mb32_swap(im_ptr->wl1)*10;
	em.wl2 = mb32_swap(im_ptr->wl2)*10;
	em.wl3 = mb32_swap(im_ptr->wl3)*10;
	em.val1 = mb32_swap(im_ptr->rpp1s) / 10000;
	em.val2 = mb32_swap(im_ptr->rpp2s) / 10000;
	em.val3 = mb32_swap(im_ptr->rpp3s) / 10000;
	em.varl1 = mb32_swap(im_ptr->rpp1s) / 10000;
	em.varl2 = mb32_swap(im_ptr->rpp2s) / 10000;
	em.varl3 = mb32_swap(im_ptr->rpp3s) / 10000;
	em.wsys = mb32_swap(im_ptr->tps)*10;
	em.vasys = mb32_swap(im_ptr->tps)*10;
	em.varsys = mb32_swap(im_ptr->tps)*10;
	em.pfl1 = mb16_swap((const int16_t) im_ptr->pfl1);
	em.pfl2 = mb16_swap((const int16_t) im_ptr->pfl2);
	em.pfsys = mb16_swap((const int16_t) im_ptr->pfl1);
	em.hz = mb16_swap((const int16_t) im_ptr->hz);
	emt.hz = (int32_t) (((float) em.hz) * 10.0f);
	em_tmp.hz = (float) emt.hz;
	em_tmp.al1 = ((float) im.al1) / 100.0f;
}

static bool iammeter_modbus_write_check(C_data * client, bool* cstate, const uint16_t rec_length)
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
		client->mcmd = I_LAST; // what do we run next
	} else {
		if (get_500hz(false) >= IRDELAY) {
			client->cstate = CLEAR; // where do we go next
			client->mcmd = I_DATA1; // what do we run next
			M.to_error++;
			M.error++;
			if (client->data_ok) {
				MM_ERROR_C;
			}
		}
	}
	return *cstate;
}

static bool iammeter_modbus_read_check(C_data * client, bool* cstate, const uint16_t rec_length, void (* DataHandler)(void))
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
			log_crc_error(c_crc, c_crc_rec);
		}
		client->cstate = CLEAR;
	} else {
		if (get_500hz(false) >= IRDELAY) {
			client->cstate = CLEAR;
			MM_ERROR_C;
			client->mcmd = I_DATA1;
			M.to_error++;
			M.error++;
			client->id_ok = false;
			*cstate = false;
			client->config_ok = false;
			client->passwd_ok = false;
			client->data_ok = false;
			client->light_ok = false;
			client->version_ok = false;
			client->serial_ok = false;
		}
	}
	return *cstate;
}

static bool iammeter_modbus_read_id_check(C_data * client, bool* cstate, const uint16_t rec_length)
{
	uint16_t c_crc, c_crc_rec;

	client->req_length = rec_length;
	if (DBUG_R((M.recv_count >= client->req_length) && (cc_buffer[0] == MADDR) && (cc_buffer[1] == READ_HOLDING_REGISTERS))) {
		c_crc = crc16(cc_buffer, client->req_length - 2);
		c_crc_rec = crc16_receive(client, cc_buffer);
		if ((DBUG_R c_crc == c_crc_rec) && (cc_buffer[3] == MB_IAMMETER_ID_H) && (cc_buffer[4] == MB_IAMMETER_ID_L)) {
			MM_ERROR_C;
			client->id_ok = true;
			*cstate = true;
		} else {
			MM_ERROR_S;
			*cstate = false;
			client->id_ok = false;
			client->config_ok = false;
			client->passwd_ok = false;
			client->data_ok = false;
			client->light_ok = false;
			client->version_ok = false;
			client->serial_ok = false;
			log_crc_error(c_crc, c_crc_rec);
		}
		client->cstate = CLEAR;
	} else {
		if (get_500hz(false) >= IRDELAY) {
			client->cstate = CLEAR;
			client->mcmd = I_DATA1;
			M.to_error++;
			M.error++;
			client->id_ok = false;
			*cstate = false;
			client->config_ok = false;
			client->passwd_ok = false;
			client->data_ok = false;
			client->light_ok = false;
			client->version_ok = false;
			client->serial_ok = false;
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

uint16_t modbus_dcu_send_msg_im(void *cc_buffer, const void *modbus_cc_mode, const uint16_t req_length)
{
	memcpy((void*) cc_buffer, (const void *) modbus_cc_mode, req_length);
	return req_length;
}

static bool modbus_read_dcu_check_im(C_data * client, bool* cstate, const uint16_t rec_length)
{
	client->req_length = rec_length;
	if (DBUG_R((M.recv_count >= client->req_length))) {
		client->version_ok = true;
	} else {
		if (get_500hz(false) >= IRDELAY) {
			M.to_error++;
			M.error++;
			client->id_ok = false;
			*cstate = false;
			client->config_ok = false;
			client->passwd_ok = false;
			client->data_ok = false;
			client->link_ok = false;
			client->version_ok = false;
			client->serial_ok = false;
			MLED_SetHigh();
		}
	}
	return *cstate;
}

/*
 * callback for UART received character from MODBUS client
 * for each RX byte received on the RS485 serial port
 * don't share with other drivers
 */
void im_my_modbus_rx_32(void)
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

void iammeter_version(void)
{
	strncpy(em_info, "WEM3080 Driver      ", 32);
	strncpy(modbus_name [1], "WEM30", 12);
}