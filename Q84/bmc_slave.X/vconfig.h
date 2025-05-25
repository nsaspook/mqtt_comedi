/*
 * File:   vconfig.h
 * Author: root
 *
 * Created on February 19, 2019, 4:41 PM
 */

#ifndef VCONFIG_H
#define	VCONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

//#include <pic18f47q84.h>
	
#include <xc.h>
#include <time.h>
#include "mcc_generated_files/adc.h"
#include "mcc_generated_files/spi1.h"
#include "mcc_generated_files/pin_manager.h"

#define NHD		// SPI 20X4 display, nhd-0420d3z-nsw-bbw

#define VER	"V0.12"
	/** \file vconfig.h
	 * Software version and a brief doc for each version changes.
	    Version for 47Q84.

	 * V0.01 OPI daq slave via SPI2
	 * V0.03 add all ADC channels and clean up for SECS defines and variables
	 * V0.04 start adding the DIO devices code with speed and mode changes 4MHz and mode 1, 50KHz mode 3 for the display
	 * V0.05 have the SPI I/O routines working for the DIO devices
	 * V0.10 mainly working DO version
	 * V0.11 mainly working DI version
	 * V0.12 5 transfers for 24 bit DI SPI
	 */
/*
 * TIC12400 testing mode
 */
#define DIO_TEST
#define DIO_SHOW_BUF


#define SLED	MLED_LAT
#define DLED	DLED_LAT

#define TRACE
	//  RC2 MTRACE TP1

#define DEFAULT_TID	1
#define TDELAY		3000
#define SEQDELAY	10000
#define LDELAY		1000
#define SDELAY		500
#define BDELAY		300
#define DDELAY		500 // display update spacing
#define DFLIP		1500 // display info flipping spacing
#define ADCDELAY	5 // adc update rate

	/*
	 * characters per line on the display
	 */
#define MAX_STRLEN      20
#define MAX_LINE        MAX_STRLEN+1
#define MAX_TEXT        MAX_STRLEN+1
#define MAX_HELP_STRLEN	32

#define MAX_INFO        63
#define MAX_BUF         127
#define MAX_TERM        159
#define MAX_VTERM       4
#define MAX_LCD_LINES	4

#define HELP_VTERM      3
#define DBUG_VTERM      2
#define INFO_VTERM      1
#define MAIN_VTERM      0

#define SWITCH_VTERM	70 // time between main to info screen switches
#define SWITCH_DURATION	32 // time in the info screen

#define SPIN_CHAR       19

#define UART_SPEED_EADR         0x03F0 // offset from 0x380000
#define UART_SPEED_LOCK_EADR	0x03F1 // offset from 0x380000

#define HOST_UART       1
#define EQUIP_UART      2

#define DEF_TIME        0

	const char msg_gemcmds[] = "Host CMDS: M C R P O L S D E H F";
	const char msg_freecmds[] = "Port baud rate unlocked        ";
	const char msg_gemremote[] = "Host CMDS: ENABLED REMOTE";

	struct spi_link_type { // internal SPI state table
		uint8_t SPI_LCD : 1;
		uint8_t SPI_AUX : 1;
		uint8_t LCD_TIMER : 1;
		volatile uint8_t LCD_DATA : 1;
		volatile uint8_t READ_DATA : 1;
		uint16_t delay;
		uint8_t config;
		uint8_t * txbuf;
		uint8_t * rxbuf;
		volatile int32_t int_count, int_read, des_bytes, src_bytes, or_bytes;
	};

	typedef struct B_type {
		volatile bool one_sec_flag;
		volatile uint16_t dim_delay;
		volatile bool display_update;
	} B_type;

	typedef enum {
		DIS_STR = 0,
		DIS_TERM,
		DIS_LOG,
		DIS_LOAD,
		DIS_UNLOAD,
		DIS_PUMP,
		DIS_HELP,
		DIS_SEQUENCE,
		DIS_SEQUENCE_M,
		DIS_ERR,
		DIS_FREE,
		DIS_CLEAR,
	} D_CODES;

	typedef struct terminal_type {
		uint8_t ack[32], mesgid;
		uint8_t TID, mcode, mparm, cmdlen, log_seq;
		uint8_t host_display_ack : 1;
		D_CODES info, help_temp;
		uint16_t ceid;
		uint16_t log_num;
	} terminal_type;

	typedef enum {
		SEQ_STATE_INIT = 0,
		SEQ_STATE_RX,
		SEQ_STATE_TX,
		SEQ_STATE_TRIGGER,
		SEQ_STATE_QUEUE,
		SEQ_STATE_DONE,
		SEQ_STATE_ERROR
	} SEQ_STATES;

	typedef enum {
		UI_STATE_INIT = 0,
		UI_STATE_HOST,
		UI_STATE_DEBUG,
		UI_STATE_LOG,
		UI_STATE_ERROR
	} UI_STATES;

	typedef enum {
		BMC_STATE_DISABLE = 0,
		BMC_STATE_COMM,
		BMC_STATE_OFFLINE,
		BMC_STATE_ONLINE,
		BMC_STATE_REMOTE,
		BMC_STATE_ERROR
	} BMC_STATES;

	extern const char * BMC_TEXT [];

	typedef enum {
		BMC_GENERIC = 0,
		BMC_VII80,
		BMC_E220,
		BMC_ERROR = 9
	} BMC_EQUIP;

	typedef enum {
		LINK_STATE_IDLE = 0,
		LINK_STATE_ENQ,
		LINK_STATE_EOT,
		LINK_STATE_ACK,
		LINK_STATE_DONE,
		LINK_STATE_NAK,
		LINK_STATE_ERROR
	} LINK_STATES;

	typedef enum {
		LINK_ERROR_NONE = 10,
		LINK_ERROR_T1,
		LINK_ERROR_T2,
		LINK_ERROR_T3,
		LINK_ERROR_T4,
		LINK_ERROR_CHECKSUM,
		LINK_ERROR_NAK,
		LINK_ERROR_ABORT,
		LINK_ERROR_SEND
	} LINK_ERRORS;

	typedef enum {
		MSG_ERROR_NONE = 0,
		MSG_ERROR_ID = 1,
		MSG_ERROR_STREAM = 3,
		MSG_ERROR_FUNCTION = 5,
		MSG_ERROR_DATA = 7,
		MSG_ERROR_TIMEOUT = 9,
		MSG_ERROR_DATASIZE = 11,
		MSG_ERROR_RESET = 20
	} MSG_ERRORS;

	typedef enum {
		SEND_ERROR_NONE = 0,
		SEND_ERROR_ABORT,
		SEND_ERROR_EOT,
		SEND_ERROR_T2,
		SEND_ERROR_T3,
		SEND_ERROR_DATA,
	} SEND_ERRORS;

	typedef enum {
		RECV_ERROR_NONE = 0,
		RECV_ERROR_NAK,
		RECV_ERROR_EOT,
		RECV_ERROR_T2,
		RECV_ERROR_T3,
		RECV_ERROR_CKSUM,
		RECV_ERROR_DATA,
	} RECV_ERRORS;

	typedef enum {
		TICKER_ZERO = 0,
		TICKER_LOW = 20,
		TICKER_HIGH = 40,
	} TICKER_VAL;

	typedef struct V_data { // control data structure
		SEQ_STATES s_state;
		UI_STATES ui_state;
		BMC_STATES g_state;
		BMC_EQUIP e_types;
		LINK_STATES m_l_state;
		LINK_STATES r_l_state;
		LINK_STATES t_l_state;
		char buf[MAX_BUF + 1], terminal[MAX_TERM + 1], info[MAX_INFO + 1];
		uint32_t ticks, systemb, tx_total, rx_total, bt_total, br_total, brn_total, btn_total,bmc_do, bmc_di;
		volatile uint32_t utc_ticks;
		int32_t testing;
		uint8_t stream, function, error, abort, msg_error, msg_ret, alarm, event;
		UI_STATES ui_sw;
		uint16_t r_checksum, t_checksum, checksum_error, timer_error, ping, mode_pwm, equip_timeout, sequences, all_errors, ceid;
		uint8_t rbit : 1, wbit : 1, ebit : 1, failed_send : 4, failed_receive : 4;
		terminal_type response;
		uint8_t uart, llid, sid, ping_count, euart, vterm, vterm_switch, uart_speed_fast;
		volatile uint8_t ticker;
		bool flipper, queue, debug, help, stack, help_id, rerror, speed_spin, set_sequ, log_s6f11, log_abort, log_char;
		adc_result_t v_tx_line, v_rx_line;
		int16_t tx_volts, rx_volts;
		char tx_rs232, rx_rs232;
		int16_t secs_value, cmd_value;
		time_t utc_cmd_value;
	} V_data;

	typedef struct V_help {
		const char message[MAX_HELP_STRLEN], display[MAX_HELP_STRLEN], extrams[MAX_HELP_STRLEN];
	} V_help;

	extern char spinners(uint8_t, const uint8_t);
	/* spinner defines */
#define MAX_SHAPES  6
	const char spin[MAX_SHAPES][MAX_STRLEN] = {
		"||//--", // classic LCD version with no \ character
		"||//--\\\\", // classic
		"OOOOOO--__-", // eye blink
		"vv<<^^>>", // point spinner
		"..**x#x#XX||--", // warp portal
		"..ooOOoo" // ball bouncer
	};
#define SPIN_VAL_UPDATE	5

	extern B_type B;

	extern void UART1_Initialize19200(void);
	extern void UART2_Initialize19200(void);
    
    extern void UART1_Initialize115200(void);
	extern void UART2_Initialize115200(void);

#ifdef	__cplusplus
}
#endif

#endif	/* VCONFIG_H */

