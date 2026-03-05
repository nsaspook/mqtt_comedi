#include "iammeter_rtu.h"

#define TDELAY		3	// half-duplex delay

typedef struct M_time_data { // ISR used, mainly for non-atomic mod problems
	uint32_t clock_500hz;
	uint32_t clock_500ahz;
	uint32_t clock_2hz;
} M_time_data;

static volatile uint8_t cc_stream_file, *cc_buffer, cc_buffer_0[MAX_DATA], cc_buffer_1[MAX_DATA], cc_buffer_tx[MAX_DATA]; // RX and TX command buffers

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
static void iammetert_data_handler(void);
static void iammeters_data_handler(void);
static void iammeterv_data_handler(void);

static void half_dup_tx(const bool);
static void half_dup_rx(const bool);

static uint8_t wem_single[] = {0x01, 0x10, 0x00, 0x0C, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0xF3, 0xFA},
wem_three_forward[] = {0x01, 0x10, 0x00, 0x63, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x92},
wem_three_reverse[] = {0x01, 0x10, 0x00, 0x66, 0x00, 0x02, 0x04, 0x00, 0x00, 0x00, 0x00, 0x75, 0xAD};

int8_t iammeter_controller_work(C_data * client)
{
	static uint32_t spacing = 0;

	if (spacing++ <SPACING && !M.rx) {
		return T_spacing;
	}

	spacing = 0;

	return client->trace;
}

int8_t reset_iammeter_kwh(C_data * client)
{
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
		 * command specific tx buffer setup
		 */
		switch (client->resets++) {
		case 0:
			client->req_length = modbus_dcu_send_msg_im((void*) cc_buffer_tx, (const void *) &wem_single, sizeof(wem_single));
			break;
		case 1:
			client->req_length = modbus_dcu_send_msg_im((void*) cc_buffer_tx, (const void *) &wem_three_forward, sizeof(wem_three_forward));
			break;
		case 2:
			client->req_length = modbus_dcu_send_msg_im((void*) cc_buffer_tx, (const void *) &wem_three_reverse, sizeof(wem_three_reverse));
			break;
		default:
			client->req_length = modbus_dcu_send_msg_im((void*) cc_buffer_tx, (const void *) &wem_single, sizeof(wem_single));
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
		if (get_500hz(false) >= TEDELAY) {
			for (uint8_t i = 0; i < client->req_length; i++) {
				Swrite(cc_buffer_tx[i]);
			}
			client->cstate = RECV;
			clear_500hz(); // state machine execute background timer clear
			client->trace = T_send_d;
			client->iam_count++;
			M.sends++;
			M.rx = false;
			if (serial_trmt()) { // check for serial UART transmit shift register and buffer empty
				clear_500hz(); // clear timer until buffer empty
			}
			delay_ms(TDELAY + client->req_length);
			DERE_SetLow(); // enable modbus receiver
		}
		break;
	case RECV:
		client->trace = T_recv;
		if (get_500hz(false) >= TEDELAY) { // state machine execute timer test
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
	 * load EM540 data pointer with receive buffer to data structure
	 * and munge the data into the correct local formats for client
	 */
	em_ptr = (EM_data1*) & cc_buffer[3];
	em.vl1n = mb32_swap(em_ptr->vl1n);
	em.vl2n = mb32_swap(em_ptr->vl2n);
	em.vl3n = mb32_swap(em_ptr->vl3n);
	em.vl1l2 = mb32_swap(em_ptr->vl1l2);
	em.vl2l3 = mb32_swap(em_ptr->vl2l3);
	em.vl3l1 = mb32_swap(em_ptr->vl3l1);
	em.al1 = mb32_swap(em_ptr->al1);
	em.al2 = mb32_swap(em_ptr->al2);
	em.al3 = mb32_swap(em_ptr->al3);
	em.wl1 = mb32_swap(em_ptr->wl1);
	em.wl2 = mb32_swap(em_ptr->wl2);
	em.wl3 = mb32_swap(em_ptr->wl3);
	em.val1 = mb32_swap(em_ptr->val1);
	em.val2 = mb32_swap(em_ptr->val2);
	em.val3 = mb32_swap(em_ptr->val3);
	em.varl1 = mb32_swap(em_ptr->varl1);
	em.varl2 = mb32_swap(em_ptr->varl2);
	em.varl3 = mb32_swap(em_ptr->varl3);
	em.wsys = mb32_swap(em_ptr->wsys);
	em.vasys = mb32_swap(em_ptr->vasys);
	em.varsys = mb32_swap(em_ptr->varsys);
	em.pfl1 = mb16_swap(em_ptr->pfl1);
	em.pfl2 = mb16_swap(em_ptr->pfl2);
	em.pfsys = mb16_swap(em_ptr->pfsys);
	em.hz = mb16_swap(em_ptr->hz);

	em_tmp.al1 = ((float) em.al1) / 1000.0f;
}

static void iammetert_data_handler(void)
{
	/*
	 * move from receive buffer to data structure and munge the data into the correct local formats from MODBUS client
	 */
	memcpy((void*) &emt, (void*) &cc_buffer[3], sizeof(emt));
	emt.hz = mb32_swap(emt.hz);

	em_tmp.hz = ((float) emt.hz) / 1000.0f;
}

static void iammmeters_data_handler(void)
{
	/*
	 * move from receive buffer to data structure and munge the data into the correct local formats from MODBUS client
	 */
	memcpy((void*) &ems, (void*) &cc_buffer[3], sizeof(ems));
	ems.serial[13] = 0; // terminate serial string data
	ems.year = mb16_swap(ems.year);
}

static void iammeterv_data_handler(void)
{
	/*
	 * move from receive buffer to data structure and munge the data into the correct local formats from MODBUS client
	 */
	memcpy((void*) &emv, (void*) &cc_buffer[3], sizeof(emv));
	emv.firmware = mb16_swap(emv.firmware);
}

static bool iammeter_modbus_write_check(C_data * client, bool* cstate, const uint16_t rec_length)
{
	uint16_t c_crc, c_crc_rec;

	client->req_length = rec_length;
	if (DBUG_R((M.recv_count >= client->req_length) && (cc_buffer[0] == MADDR) && (cc_buffer[1] == WRITE_SINGLE_REGISTER))) {
		c_crc = crc16(cc_buffer, client->req_length - 2);
		c_crc_rec = crc16_receive(client);
		if (DBUG_R c_crc == c_crc_rec) {
			*cstate = true;
			MM_ERROR_C;
		} else {
			*cstate = false;
			log_crc_error(c_crc, c_crc_rec);
		}
		client->cstate = CLEAR; // where do we go next
		client->mcmd = G_LAST; // what do we run next
	} else {
		if (get_500hz(false) >= RDELAY) {
			client->cstate = CLEAR; // where do we go next
			client->mcmd = G_ID; // what do we run next
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
		c_crc_rec = crc16_receive(client);
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
			MM_ERROR_C;
			*cstate = false;
			client->data_ok = false;
			log_crc_error(c_crc, c_crc_rec);
		}
		client->cstate = CLEAR;
	} else {
		if (get_500hz(false) >= RDELAY) {
			client->cstate = CLEAR;
			MM_ERROR_C;
			client->mcmd = G_ID;
			M.to_error++;
			M.error++;
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
		c_crc_rec = crc16_receive(client);
		if ((DBUG_R c_crc == c_crc_rec) && (cc_buffer[3] == MB_EM540_ID_H) && (cc_buffer[4] == MB_EM540_ID_L)) {
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
		if (get_500hz(false) >= RDELAY) {
			client->cstate = CLEAR;
			client->mcmd = G_ID;
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

// switch RS transceiver to transmit mode and wait if not tx

static void half_dup_tx(const bool delay)
{
#ifndef AUTO_DERE
	if (DERE_GetValue()) {
		return;
	}
	DERE_SetHigh(); // enable modbus transmitter

	if (delay) {
		WaitMs(DUPL_DELAY); // busy waits
	}
#endif
}

// switch RS transceiver to receive mode and wait if not rx

static void half_dup_rx(const bool delay)
{
#ifndef AUTO_DERE
	if (!DERE_GetValue()) {
		return;
	}
	if (delay) {
		WaitMs(DUPL_DELAY); // busy waits
	}
	DERE_SetLow(); // enable modbus receiver
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
		if (get_500hz(false) >= RDELAY) {
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