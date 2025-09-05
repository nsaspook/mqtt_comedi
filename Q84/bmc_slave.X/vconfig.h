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

#define DIS_DEBUG	// active status display, disable during normal operation

#define VER	"V0.38"
	/** \file vconfig.h
	 * Software version and a brief doc for each version changes.
	    Version for 57Q84.

	 * V0.01 OPI daq slave via SPI2
	 * V0.03 add all ADC channels and clean up for SECS defines and variables
	 * V0.04 start adding the DIO devices code with speed and mode changes 4MHz and mode 1, 50KHz mode 3 for the display
	 * V0.05 have the SPI I/O routines working for the DIO devices
	 * V0.10 mainly working DO version
	 * V0.11 mainly working DI version
	 * V0.12 5 transfers for 24 bit DI SPI
	 * V0.13 fix ADC higher channels and cleanup SPI link protocols
	 * V0.14 remove serial speed switching code
	 * V0.15 update to 9 byte packets for all commands
	 * V.016 fix TIC12400 I/O bugs
	 * V.017 code cleanup
	 * V.018 more code cleanup
	 * V.020 18f57q84 version
	 * V.021 fix-up serial data protocol with STX to mark start of string
	 * V0.30 version for the FM80 MODBUS and FM80 serial
	 * V0.31 more cleanup of code
	 * V0.32 add code for FM80 state machine and call-backs
	 * V0.33 finally traced the FM80 restart code, optimize timing for all new code
	 * V0.34 testing daq_bmc CSV data link to the OPi
	 * V0.35 cleanup ISR and other misc code
	 * V0.36 LCD dimming, DI changes back to bright
	 * V0.37 still working on float restart sequencing
	 * V0.38 cleanup debugging LCD code
	 */
	/*
	 * TIC12400 testing modes
	 */
#define DIO_TEST
#define DIO_SHOW_BUF
	//#define DI_DEBUG
#define SER_DEBUG

#define DI_MC_CMD

	/* analog testing a calibration mode
	 *
	 */

#define AIO_TEST

#define SLED	MLED_LAT
#define DLED	DLED_LAT

#define TRACE

#define DEFAULT_TID	1
#define TDELAY		3000
#define SEQDELAY	10000
#define LDELAY		1000
#define SDELAY		500
#define BDELAY		300
#define DDELAY		500 // display update spacing
#define DFLIP		1500 // display info flipping spacing
#define ADCDELAY	1 // adc update rate ms

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

#define DEF_TIME        1694196350 /* default epoch time */
#define CHK_DAY_TIME	1200
#define BAT_DAY_COUNT	45	// number of reports before updates
#define BAT_NIGHT_COUNT	90

	const char log_format[] = "^,%3.1f,%3.2f,%3.2f,%3.2f,%3.2f,%3.2f,%3.2f,%d.%01d,%d.%01d,%d.%01d,%d,%d,%d,%u,1957,~EOT                                                                                                        \r\n";
#define LOG_VARS	((float) em.vl1l2) / 10.0f,((float) em.al1) / 1000.0f, ((float) em.wl1) / 10.0f, ((float) em.val1) / 10.0f, ((float) em.varl1) / 10.0f,  ((float) em.pfsys) / 10.0f, ((float) emt.hz) / 1000.0f,vw, vf, pvw, pvf, bat_amp_whole - 128, bat_amp_frac - 128, bat_amp_panel - 128,BM.FM80_online,cc_mode, BM.node_id

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

	/*
	 * switch inputs and flags, uses the IOC interrupt and the software 
	 * timing ISR for processing
	 */
	enum D_SW {
		D_SW_A = 0, // alternate LCD display
		D_SW_L, // history logging from FM80
		D_SW_M,
		D_SW_COUNT // one extra for number of switches to check
	};

	typedef struct {
		uint8_t type;
		int16_t day;
		int16_t amp_hours;
		int16_t kilowatt_hours;
		int16_t volts_peak;
		int16_t amps_peak;
		int16_t kilowatts_peak;
		int16_t bat_min;
		int16_t bat_max;
		int16_t absorb_time;
		int16_t float_time;
		uint8_t select;
	} mx_logpage_t;

	typedef struct BM_type {
		volatile bool ten_sec_flag, one_sec_flag, FM80_charged, pv_high, pv_update, once, a_switch[D_SW_COUNT], a_trigger[D_SW_COUNT], a_type[D_SW_COUNT];
		volatile uint16_t pacing, rx_count, flush, pv_prev, day_check, node_id, dim_delay;
		volatile bool FM80_online, FM80_io, LOG, display_dim, display_update, display_on, fm80_restart;
		volatile uint8_t canbus_online, modbus_online, alt_display, a_pin[D_SW_COUNT];
		float run_time, net_balance;
		uint16_t mui[10];
		uint16_t fwrev[3];
		mx_logpage_t log;
	} BM_type;

	typedef struct BF_type {
		volatile bool ten_sec_flag, one_sec_flag, FM80_charged, pv_high, pv_update, once, a_switch[D_SW_COUNT], a_trigger[D_SW_COUNT], a_type[D_SW_COUNT];
		volatile uint16_t pacing, rx_count, flush, pv_prev, day_check, node_id, dim_delay;
		volatile bool FM80_online, FM80_io, LOG, display_dim, display_update, display_on, fm80_restart;
		volatile uint8_t canbus_online, modbus_online, alt_display, a_pin[D_SW_COUNT];
		float run_time, net_balance;
		uint16_t mui[10];
		uint16_t fwrev[3];
		mx_logpage_t log;
	} BF_type;

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
		volatile uint32_t ticks, systemb, tx_total, rx_total, bt_total, br_total, brn_total, btn_total, bmc_do, bmc_di;
		volatile uint32_t utc_ticks;
		int32_t testing;
		uint8_t stream, function, error, abort, msg_error, msg_ret, alarm, event;
		UI_STATES ui_sw;
		uint16_t r_checksum, t_checksum, checksum_error, timer_error, ping, mode_pwm, equip_timeout, sequences, all_errors, ceid;
		volatile uint16_t bmc_ao;
		uint8_t rbit : 1, wbit : 1, ebit : 1, failed_send : 4, failed_receive : 4;
		terminal_type response;
		uint8_t uart, llid, sid, ping_count, euart, vterm, vterm_switch, uart_speed_fast;
		volatile uint8_t ticker;
		bool flipper, queue, debug, help, stack, help_id, rerror, speed_spin, set_sequ, log_s6f11, log_abort, log_char;
		adc_result_t v_tx_line, v_rx_line;
		int16_t tx_volts, rx_volts;
		char tx_rs232, rx_rs232;
		int16_t secs_value, cmd_value;
		time_t utc_cmd_value,
		di_fail, do_fail;
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

	extern const char text_test[];

	typedef uint16_t device_id_data_t;
	typedef uint24_t device_id_address_t;
	device_id_data_t DeviceID_Read(device_id_address_t);

	typedef struct EB_data {
		uint8_t checkmark;
		uint8_t version, alt_display;
		bool loaded;
		float FMw, FMpv, FMa, FMbv, ENw, ENva, ENvar, ENac;
		float volt_whole, bat_amp_whole;
		float bat_energy;
		uint16_t cc_mode, bat_cycles, bat_mode, time, date;
		uint32_t bat_time, fm80_time, q84_sequence;
		uint16_t crc;
	} EB_data;

	extern BM_type BM;

	extern void UART1_Initialize19200(void);
	extern void UART2_Initialize19200(void);

	extern void UART1_Initialize115200(void);
	extern void UART2_Initialize115200(void);

	void update_time(const struct tm *, EB_data *);

#ifdef	__cplusplus
}
#endif

#endif	/* VCONFIG_H */

