#include "modbus_master.h"

#define	ON	1
#define	OFF	0

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

C_data C = {
	.mcmd = G_ID,
	.cstate = CLEAR,
	.modbus_command = G_ID,
	.req_length = 0,
	.trace = 0,
	.config_ok = false,
	.id_ok = false,
	.passwd_ok = false,
	.light_ok = false,
	.M.blink_lock = false,
	.M.power_on = true,
	.tm_ok = false,
};

volatile struct VM_type VM = {
	.StartTime = 1,
	.TimeUsed = 1,
	.pacing = 1,
	.pwm_update = true,
	.pwm_stop = true,
	.fault_active = false,
	.fault_count = 0,
	.dmt_sosc_flag = false,
};

/*
 * send and receive MODBUS templates for 3-phase energy monitor EM540
 * https://www.gavazzionline.com/pdf/EM540_DS_ENG.pdf
 * https://gavazzi.se/app/uploads/2022/03/em500-cp-v1r3-eng.pdf
 */
const uint8_t
// transmit frames for commands
modbus_em_id[] = {MADDR, READ_HOLDING_REGISTERS, 0x00, 0x0b, 0x00, 0x01}, // Carlo Gavazzi Controls identification code
modbus_em_version[] = {MADDR, READ_HOLDING_REGISTERS, 0x03, 0x02, 0x00, 0x01}, // Firmware version and revision code
modbus_em_data1[] = {MADDR, READ_HOLDING_REGISTERS, 0x00, 0x00, 0x00, EM_DATA_LEN1}, // last number is 16-bit words wanted from the start register address 0x0000
modbus_em_data2[] = {MADDR, READ_HOLDING_REGISTERS, 0x05, 0x00, 0x00, EM_DATA_LEN2}, // last number is 16-bit words wanted from the start register address 0x0500
modbus_em_serial[] = {MADDR, READ_HOLDING_REGISTERS, 0x50, 0x00, 0x00, SERIAL_DATA_LEN}, // last number is 16-bit words wanted from the start register address 0x5000
modbus_em_config[] = {MADDR, WRITE_SINGLE_REGISTER, 0x10, 0x02, 0x00, 0x02}, // System configuration, Value 2 = ?2P? (2-phase with neutral)
modbus_em_passwd[] = {MADDR, WRITE_SINGLE_REGISTER, 0x10, 0x00, 0x00, 0x00}, // Password configuration, set to no password = 0
modbus_em_light[] = {MADDR, WRITE_SINGLE_REGISTER, 0x16, 0x04, 0x00, 0x05}, // back-light timeout, 5 min
// receive frames prototypes for received data checking
em_id[] = {MADDR, READ_HOLDING_REGISTERS, 0x00, 0x00, 0x00, 0x00, 0x00},
em_version[] = {MADDR, READ_HOLDING_REGISTERS, 0x00, 0x00, 0x00, 0x00, 0x00},
em_data1[(EM_DATA_LEN1 * 2) + 5] = {MADDR, READ_HOLDING_REGISTERS, 0x00}, // number of 16-bit words returned, IN BYTES
em_data2[(EM_DATA_LEN2 * 2) + 5] = {MADDR, READ_HOLDING_REGISTERS, 0x00}, // number of 16-bit words returned, IN BYTES
em_serial[(SERIAL_DATA_LEN * 2) + 5] = {MADDR, READ_HOLDING_REGISTERS, 0x00}, // number of 16-bit words returned, IN BYTES
em_config[] = {MADDR, WRITE_SINGLE_REGISTER, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
em_passwd[] = {MADDR, WRITE_SINGLE_REGISTER, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
em_light[] = {MADDR, WRITE_SINGLE_REGISTER, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


EM_data1 em, *em_ptr;
EM_data2 emt;
EM_serial ems;
EM_version emv;

static void clear_2hz(void);
static void clear_500ahz(void);
static void clear_500hz(void);
static uint32_t get_2hz(const uint8_t);
static uint32_t get_500ahz(const uint8_t);
static uint32_t get_500hz(const uint8_t);
static void timer_500ms_tick(void);
static void timer_2ms_tick(void);

static void half_dup_tx(const bool);
static void half_dup_rx(const bool);
static bool serial_trmt(void);
static uint16_t modbus_rtu_send_msg(void *, const void *, uint16_t);
static uint16_t modbus_rtu_send_msg_crc(volatile uint8_t *, uint16_t);
static uint16_t crc16_receive(const C_data *);
static void log_crc_error(const uint16_t, const uint16_t);
static void UART3_DefaultFramingErrorHandler_mb(void);
static void UART3_DefaultOverrunErrorHandler_mb(void);
static void UART3_DefaultErrorHandler_mb(void);

static bool modbus_write_check(C_data *, bool*, uint16_t);
static bool modbus_read_check(C_data *, bool*, uint16_t, void (* DataHandler)(void));
static bool modbus_read_id_check(C_data *, bool*, uint16_t);

static void em_data_handler(void);
static void emt_data_handler(void);
static void ems_data_handler(void);
static void emv_data_handler(void);

/*
 * add the required CRC bytes to a MODBUS message
 */
static uint16_t modbus_rtu_send_msg_crc(volatile uint8_t *req, uint16_t req_length)
{
	uint16_t crc;

	crc = crc16(req, req_length);
	req[req_length++] = crc >> (uint16_t) 8;
	req[req_length++] = crc & 0x00FF;

	return req_length;
}

/*
 * constructs a properly formatted RTU message with CRC from a program memory array to the data memory array buffer
 */
uint16_t modbus_rtu_send_msg(void *cc_buffer, const void *modbus_cc_mode, uint16_t req_length)
{
	memcpy((void*) cc_buffer, (const void *) modbus_cc_mode, req_length);
	/*
	 * add the CRC and increase message size by two bytes for the CRC16
	 */
	return modbus_rtu_send_msg_crc((volatile uint8_t *) cc_buffer, req_length);
}

/*
 * calculate a CRC16 from the data buffer
 */
uint16_t crc16(volatile uint8_t *buffer, uint16_t buffer_length)
{
	uint8_t crc_hi = 0xFF; /* high CRC byte initialized */
	uint8_t crc_lo = 0xFF; /* low CRC byte initialized */
	uint8_t i; /* will index into CRC lookup */
	uint16_t crc16t;

	/* pass through message buffer */
	while (buffer_length--) {
		i = crc_hi ^ *buffer++; /* calculate the CRC  */
		crc_hi = crc_lo ^ table_crc_hi[i];
		crc_lo = table_crc_lo[i];
	}

	crc16t = (uint16_t) crc_hi << (uint16_t) 8 | (uint16_t) crc_lo;
	return crc16t;
}

/*
 * callback for UART received character from MODBUS client
 * for each RX byte received on the RS485 serial port
 */
void my_modbus_rx_32(void)
{
	static uint8_t m_data = 0;

	INT_TRACE;
	M.rx = true;
	/*
	 * process received controller data stream
	 */
	m_data = Srbuffer; // receiver data buffer
	cc_buffer[M.recv_count] = m_data;
	if (++M.recv_count >= MAX_DATA) {
		M.recv_count = 0; // reset buffer position
	}
}

uint8_t init_stream_params(void)
{
	M.config = false;
	return 0;
}

/*
 * state machine hardware timers interrupt ISR functions setup
 */
void init_mb_master_timers(void)
{
	cc_buffer = cc_buffer_0;
	em_ptr = (EM_data1*) & cc_buffer[3];
	TMR4_SetInterruptHandler(timer_500ms_tick);
	TMR4_StartTimer();
	TMR3_SetInterruptHandler(timer_2ms_tick);
	TMR3_StartTimer();
}

/*
 * helper functions
 * received CRC16 bytes from client
 */
static uint16_t crc16_receive(const C_data * client)
{
	uint16_t crc16r;

	crc16r = ((uint16_t) cc_buffer[client->req_length - 2] << (uint16_t) 8) | ((uint16_t) cc_buffer[client->req_length - 1] & 0x00ff);
	return crc16r;
}

static void log_crc_error(const uint16_t c_crc, const uint16_t c_crc_rec)
{
	M.crc_calc = c_crc;
	M.crc_data = c_crc_rec;
	M.crc_error++;
	M.error++;
}

/*
 * reorder 16-bit word bytes for int32_t
 * https://control.com/forums/threads/endianness-for-32-bit-data.48584/
 * https://ctlsys.com/support/common_modbus_protocol_misconceptions/
 * https://iotech.force.com/edgexpert/s/article/Byte-and-Word-Swapping-in-Modbus
 *
 * "Little Endian" slaves or "Big Endian" slaves
 * Byte endianness with Word endianness?
 * Lions and Tigers and Bears!
 */
int32_t mb32_swap(const int32_t value)
{
	uint8_t i;
	union MREG32 dvalue;

	// program it simple and easy to understand way, let the compiler optimize the expressions
	dvalue.value = value;
	i = dvalue.bytes[0];
	dvalue.bytes[0] = dvalue.bytes[1];
	dvalue.bytes[1] = i;
	i = dvalue.bytes[2];
	dvalue.bytes[2] = dvalue.bytes[3];
	dvalue.bytes[3] = i;
	return dvalue.value;
}

int16_t mb16_swap(const int16_t value)
{
	uint8_t i;
	union MREG dvalue;

	// program it simple and easy to understand way, let the compiler optimize the expressions
	dvalue.value = value;
	i = dvalue.bytes[0];
	dvalue.bytes[0] = dvalue.bytes[1];
	dvalue.bytes[1] = i;
	return dvalue.value;
}

/*
 * Simple MODBUS master state machine
 * this needs to run in the main programming loop
 * to handle RS485 serial I/O exchanges
 */
int8_t master_controller_work(C_data * client)
{
	static uint32_t spacing = 0;

	if (spacing++ <SPACING && !M.rx) {
		return T_spacing;
	}

	IO_RF7_SetHigh();
	spacing = 0;

	client->trace = T_begin;
	switch (client->cstate) {
	case CLEAR:
		client->trace = T_clear;
		clear_2hz();
		clear_500ahz();
		client->cstate = INIT;
		client->modbus_command = client->mcmd++; // sequence modbus commands to client
		if (client->modbus_command == G_CONFIG && client->config_ok) { // skip if we have valid data from client
			client->modbus_command = client->mcmd++;
		}
		if (client->modbus_command == G_PASSWD && client->passwd_ok) { // skip if we have valid data from client
			client->modbus_command = client->mcmd++;
		}
		if (client->modbus_command == G_LIGHT && client->light_ok) { // skip if we have valid data from client
			client->modbus_command = client->mcmd++;
		}
		if (client->modbus_command == G_VERSION && client->version_ok) { // skip if we have valid data from client
			client->modbus_command = client->mcmd++;
		}
		if (client->modbus_command == G_SERIAL && client->serial_ok) { // skip if we have valid data from client
			client->modbus_command = client->mcmd++;
		}
		if (client->mcmd > G_LAST) {
			client->mcmd = G_ID;
		}
		/*
		 * command specific tx buffer setup
		 */
		switch (client->modbus_command) {
		case G_VERSION: // write code request
			client->trace = T_version;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_em_version, sizeof(modbus_em_version));
			break;
		case G_SERIAL: // write code request
			client->trace = T_serial;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_em_serial, sizeof(modbus_em_serial));
			break;
		case G_LIGHT: // write code request
			client->trace = T_light;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_em_light, sizeof(modbus_em_light));
			break;
		case G_PASSWD: // write code request
			client->trace = T_passwd;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_em_passwd, sizeof(modbus_em_passwd));
			break;
		case G_CONFIG: // write code request
			client->trace = T_config;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_em_config, sizeof(modbus_em_config));
			break;
		case G_DATA1: // read code request
			client->trace = T_data;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_em_data1, sizeof(modbus_em_data1));
			break;
		case G_DATA2: // read code request
			client->trace = T_data;
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_em_data2, sizeof(modbus_em_data2));
			break;
		case G_LAST: // end of command sequences
			client->cstate = CLEAR;
			client->mcmd = G_ID; // what do we run next
			break;
		case G_ID: // operating mode request
			client->trace = T_id;
		default:
			client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_em_id, sizeof(modbus_em_id));
			break;
		}
		break;
	case INIT:
		client->trace = T_init;
		/*
		 * MODBUS master query speed
		 */
#ifdef	FASTQ
		if (get_500ahz(false) >= CDELAY) {
#else
		if (get_2hz(false) >= QDELAY) {
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
		if (get_500hz(false) >= TEDELAY) {
			for (uint8_t i = 0; i < client->req_length; i++) {
				Swrite(cc_buffer_tx[i]);
			}
			client->cstate = RECV;
			clear_500hz(); // state machine execute background timer clear
			client->trace = T_send_d;
			M.sends++;
			M.rx = false;
			if (serial_trmt()) { // check for serial UART transmit shift register and buffer empty
				clear_500hz(); // clear timer until buffer empty
			}
#ifndef AUTO_DERE
			DERE_SetLow(); // enable modbus receiver
#endif
		}
		break;
	case RECV:
		client->trace = T_recv;
		if (get_500hz(false) >= TEDELAY) { // state machine execute timer test

			client->trace = T_recv_r;
#ifndef AUTO_DERE
			half_dup_rx(false); // no delays here
#endif

			/*
			 * check received response data for size and format for each command sent
			 */
			switch (client->modbus_command) {
			case G_LIGHT: // check for controller back-light codes
				modbus_write_check(client, &client->light_ok, sizeof(em_light));
				break;
			case G_PASSWD: // check for controller password codes
				modbus_write_check(client, &client->passwd_ok, sizeof(em_passwd));
				break;
			case G_CONFIG: // check for controller configuration codes
				modbus_write_check(client, &client->config_ok, sizeof(em_config));
				break;
			case G_DATA1: // check for controller data1 codes
				modbus_read_check(client, &client->data_ok, sizeof(em_data1), em_data_handler);
				break;
			case G_DATA2: // check for controller data2 codes
				modbus_read_check(client, &client->data_ok, sizeof(em_data2), emt_data_handler);
				break;
			case G_VERSION: // check for controller EM540 firmware codes
				modbus_read_check(client, &client->version_ok, sizeof(em_version), emv_data_handler);
				break;
			case G_SERIAL: // check for controller EM540 serial codes
				modbus_read_check(client, &client->serial_ok, sizeof(em_serial), ems_data_handler);
				break;
			case G_ID: // check for client module type
			default:
				modbus_read_id_check(client, &client->id_ok, sizeof(em_id));
				break;
			}
		}
		break;
	default:
		break;
	}
	IO_RF7_SetLow();
	return client->trace;
}

/*
 * state machine no busy wait timers
 */
void clear_2hz(void)
{
	MT.clock_2hz = 0;
}

void clear_500ahz(void)
{
	MT.clock_500ahz = 0;
}

void clear_500hz(void)
{
	MT.clock_500hz = 0;
}

uint32_t get_2hz(const uint8_t mode)
{
	static uint32_t tmp = 0;

	if (mode) {
		return tmp;
	}

	tmp = MT.clock_2hz;
	return tmp;
}

/*
 * 500Hz updates
 * used for fast updates timing
 */
uint32_t get_500ahz(const uint8_t mode)
{
	static uint32_t tmp = 0;

	if (mode) {
		return tmp;
	}

	tmp = MT.clock_500ahz;
	return tmp;
}

uint32_t get_500hz(const uint8_t mode)
{
	static uint32_t tmp = 0;

	if (mode) {
		return tmp;
	}

	tmp = MT.clock_500hz;
	return tmp;
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

// ISR function for TMR4

void timer_500ms_tick(void)
{
	MT.clock_2hz++;
}

// ISR function for TMR3

void timer_2ms_tick(void)
{
	MT.clock_500hz++;
	MT.clock_500ahz++;
}

/*
 * check if we are done with interrupt background buffered transmission of serial data with FIFO
 *
 * TRMT: Transmit Shift Register is Empty bit (read-only)
 * 1 = Transmit shift register is empty and transmit buffer is empty (the last transmission has completed)
 * 0 = Transmit shift register is not empty, a transmission is in progress or queued in the transmit buffer
 *
 * ? 8-level deep First-In-First-Out (FIFO) transmit data buffer, ? 8-level deep FIFO receive data buffer
 * Interrupt is generated and asserted while the transmit buffer is empty
 *
 * so this will return 'true' after the buffer is empty 'interrupt' and after the last bit is on the wire
 */

static bool serial_trmt(void)
{
	return !(Strmt); // note, we invert the TRMT bit so it's true while transmitting
}

/*
 * serial port testing routine
 */
void mb_tx_test(C_data * client)
{
	if (TimerDone(TMR_MBTEST)) {
		StartTimer(TMR_MBTEST, 500);
		client->req_length = modbus_rtu_send_msg((void*) cc_buffer_tx, (const void *) modbus_em_passwd, sizeof(modbus_em_passwd));
		for (int8_t i = 0; i < client->req_length; i++) {
			if (Strdy()) {
				Swrite(cc_buffer_tx[i]);
			}
		}
	}
}

static void UART3_DefaultFramingErrorHandler_mb(void)
{
	INT_TRACE; // GPIO interrupt scope trace
	MM_ERROR_S;
}

static void UART3_DefaultOverrunErrorHandler_mb(void)
{
	INT_TRACE; // GPIO interrupt scope trace
	MM_ERROR_S;
}

static void UART3_DefaultErrorHandler_mb(void)
{
	INT_TRACE; // GPIO interrupt scope trace
	MM_ERROR_S;
}

void mb_setup(void)
{
	UART3_SetFramingErrorHandler(UART3_DefaultFramingErrorHandler_mb);
	UART3_SetOverrunErrorHandler(UART3_DefaultOverrunErrorHandler_mb);
	UART3_SetErrorHandler(UART3_DefaultErrorHandler_mb);
}

static bool modbus_write_check(C_data * client, bool* cstate, const uint16_t rec_length)
{
	uint16_t c_crc, c_crc_rec;

#ifdef	MB_EM540
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
#endif
	return *cstate;
}

static bool modbus_read_check(C_data * client, bool* cstate, const uint16_t rec_length, void (* DataHandler)(void))
{
	uint16_t c_crc, c_crc_rec;

#ifdef	MB_EM540
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
#endif
	return *cstate;
}

static bool modbus_read_id_check(C_data * client, bool* cstate, const uint16_t rec_length)
{
	uint16_t c_crc, c_crc_rec;

#ifdef	MB_EM540
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
#endif
	return *cstate;
}

static void em_data_handler(void)
{
	/*
	 * load EM540 data pointer with receive buffer to data structure
	 * and munge the data into the correct local formats for client
	 */
	em_ptr = (EM_data1*) & cc_buffer[3];
	em.vl1l2 = mb32_swap(em_ptr->vl1l2);
	em.al1 = mb32_swap(em_ptr->al1);
	em.wl1 = mb32_swap(em_ptr->wsys);
	em.val1 = mb32_swap(em_ptr->vasys);
	em.varl1 = mb32_swap(em_ptr->varsys);
	em.pfl1 = mb16_swap(em_ptr->pfl1);
	em.pfsys = mb16_swap(em_ptr->pfsys);
	em.hz = mb16_swap(em_ptr->hz);
#ifndef MB_EM540_ONE
	em.vl2l3 = mb32_swap(em_ptr->vl2l3);
	em.vl3l1 = mb32_swap(em_ptr->vl3l1);
	em.al2 = mb32_swap(em_ptr->al2);
	em.al3 = mb32_swap(em_ptr->al3);
	em.wl2 = mb32_swap(em_ptr->wl2);
	em.wl3 = mb32_swap(em_ptr->wl3);
	em.val2 = mb32_swap(em_ptr->val2);
	em.val3 = mb32_swap(em_ptr->val3);
	em.varl2 = mb32_swap(em_ptr->varl2);
	em.varl3 = mb32_swap(em_ptr->varl3);
#endif
}

static void emt_data_handler(void)
{
	/*
	 * move from receive buffer to data structure and munge the data into the correct local formats from MODBUS client
	 */
#ifdef	 MB_EM540_EMT
	memcpy((void*) &emt, (void*) &cc_buffer[3], sizeof(emt));
	emt.hz = mb32_swap(emt.hz);
#endif
}

static void ems_data_handler(void)
{
	/*
	 * move from receive buffer to data structure and munge the data into the correct local formats from MODBUS client
	 */
	memcpy((void*) &ems, (void*) &cc_buffer[3], sizeof(ems));
	ems.serial[13] = 0; // terminate serial string data
	ems.year = mb16_swap(ems.year);
}

static void emv_data_handler(void)
{
	/*
	 * move from receive buffer to data structure and munge the data into the correct local formats from MODBUS client
	 */
	memcpy((void*) &emv, (void*) &cc_buffer[3], sizeof(emv));
	emv.firmware = mb16_swap(emv.firmware);
}