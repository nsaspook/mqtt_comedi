/*
 * File:   mxcmd.h
 * Author: root
 *
 * Created on June 2, 2023, 6:16 AM
 */

#ifndef MXCMD_H
#define	MXCMD_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <stdio.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"
#include "eadog.h"
#include "timers.h"

	const char build_version[] = "V3.00 FM80 interface Q84";
	/*
	 * code changes
	 * 3.00 version for the daq_bmc board
	 */

#define MAX_B_BUF	512
#define MAX_C_BUF	CANFD_BYTES*3
#define IO_TEST

#define	FM_BUFFER	32
#define BUFFER_SPACING	4
#define SPINNER_SPEED	200
#define LP_BUFFER_SIZE	9
#define ONLINE_TIMEOUT	30000

#define FM80_ID		0x03
#define FMxx_STATE	abuf[2]

#define AMP_WHOLE_ZERO	0

#define CMD_CRC_LEN	10

#define DTG_LEN		3 // normal size is 26 but we must save buffer space

	const uint16_t cmd_id[] = {0x100, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02};
	const uint16_t cmd_status[] = {0x100, 0x02, 0x01, 0xc8, 0x00, 0x00, 0x00, 0xcb};
	const uint16_t cmd_mx_status[] = {0x100, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x05};
	uint16_t cmd_mx_log[] = {0x100, 0x16, 0x00, 0x00, 0x00, 0x01, 0x00, 0x17}; // get logs, start from day 1
	const uint16_t cmd_panelv[] = {0x100, 0x02, 0x01, 0xc6, 0x00, 0x00, 0x00, 0xc9};
	const uint16_t cmd_batteryv[] = {0x100, 0x02, 0x00, 0x08, 0x00, 0x00, 0x00, 0x0a};
	const uint16_t cmd_batterya[] = {0x100, 0x02, 0x01, 0xc7, 0x00, 0x00, 0x00, 0xca};
	const uint16_t cmd_watts[] = {0x100, 0x02, 0x01, 0x6a, 0x00, 0x00, 0x00, 0x6d};
	const uint16_t cmd_misc[] = {0x100, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02}; // example FM80 command ID request
	const uint16_t cmd_fwreva[] = {0x100, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04};
	const uint16_t cmd_fwrevb[] = {0x100, 0x02, 0x00, 0x03, 0x00, 0x00, 0x00, 0x05};
	const uint16_t cmd_fwrevc[] = {0x100, 0x02, 0x00, 0x04, 0x00, 0x00, 0x00, 0x06};
	uint16_t cmd_time[] = {0x100, 0x03, 0x40, 0x04, 0x00, 0x00, 0x00, 0x00};
	uint16_t cmd_date[] = {0x100, 0x03, 0x40, 0x05, 0x00, 0x00, 0x00, 0x00};

	enum status_type {
		STATUS_SLEEPING = 0,
		STATUS_FLOATING = 1,
		STATUS_BULK = 2,
		STATUS_ABSORB = 3,
		STATUS_EQUALIZE = 4,
		STATUS_LAST,
	};

	const char state_name [][12] = {
		"Sleeping",
		"Floating",
		"Bulk",
		"Absorb",
		"Equalize",
		" Last",
	};

	const char FM80_name [][12] = {
		" Offline",
		"FM80    ",
	};

	const char canbus_name [][12] = {
		" Offline",
		"CANBUS  ",
	};

	const char modbus_name [][12] = {
		" Offline",
		"MODBUS  ",
	};

	typedef struct {
		uint8_t a[16]; // raw_ah(part2)
	} mx_status_packed_t;

	typedef struct {
		uint8_t a[16];
	} mx_log_packed_t;

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
		volatile bool FM80_online, FM80_io, LOG, display_dim, display_update, display_on;
		volatile uint8_t canbus_online, modbus_online, alt_display, a_pin[D_SW_COUNT];
		float run_time, net_balance;
		uint16_t mui[10];
		uint16_t fwrev[3];
		mx_logpage_t log;
	} BM_type;

	extern void onesec_io(void);
	extern void tensec_io(void);
	extern void FM_io(void);
	extern uint8_t FM_tx(const uint16_t *, const uint8_t);
	extern bool FM_tx_empty(void);
	extern uint8_t FM_rx(uint16_t *);
	extern bool FM_rx_ready(void);
	extern uint8_t FM_rx_count(void);
	extern void FM_restart(void);
	extern void wdtdelay(const uint32_t);
	extern float lp_filter(const float, const uint8_t, const int8_t);
	extern uint16_t calc_checksum(const uint8_t*, const uint8_t);

	extern BM_type BM;

#ifdef	__cplusplus
}
#endif

#endif	/* MXCMD_H */

