/** \file main.c.c
 * Orange PI SPI DAQ add on board
 * Led1 RED	SPI command activity (short) or errors (long) blinks
 * LED2 GREEN	1 second CPU running blinker
 * LED3 GREEN	I/O indicator
 * LED4 GREEN	5VDC Power
 *
 * SPI1 MODE 3 MASTER to DISPLAY and MODE 1 to DIO SLAVE devices
 * SPI2 MODE 3 SLAVE to OPi controller MASTER SPI port
 */

/*
 * UART1 RS-232 comms, ANALOG header, TX-PIN 7 , RX-PIN 6 , SV1 serial bus, TX-PIN 8, RX-PIN 7
 * UART2 RS-232 comms, TTL serial HEADER, TX-PIN 2 , RX-PIN 3 , SV1 serial bus, TX-PIN 10, RX-PIN 9
 */

// PIC18F57Q84 Configuration Bit Settings
// 'C' source line config statements
// CONFIG1
#pragma config FEXTOSC = ECH    // External Oscillator Selection (EC (external clock) above 8 MHz)
#pragma config RSTOSC = EXTOSC_4PLL// Reset Oscillator Selection (EXTOSC with 4x PLL, with EXTOSC operating per FEXTOSC bits)

// CONFIG2
#pragma config CLKOUTEN = ON    // Clock out Enable bit (CLKOUT function is enabled)
#pragma config PR1WAY = OFF     // PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be set and cleared repeatedly)
#pragma config CSWEN = ON       // Clock Switch Enable bit (Writing to NOSC and NDIV is allowed)
#pragma config JTAGEN = OFF     // JTAG Enable bit (Disable JTAG Boundary Scan mode, JTAG pins revert to user functions)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor enabled)
#pragma config FCMENP = ON      // Fail-Safe Clock Monitor -Primary XTAL Enable bit (FSCM timer will set FSCMP bit and OSFIF interrupt on Primary XTAL failure)
#pragma config FCMENS = ON      // Fail-Safe Clock Monitor -Secondary XTAL Enable bit (FSCM timer will set FSCMS bit and OSFIF interrupt on Secondary XTAL failure)

// CONFIG3
#pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
#pragma config PWRTS = PWRT_OFF // Power-up timer selection bits (PWRT is disabled)
#pragma config MVECEN = ON      // Multi-vector enable bit (Multi-vector enabled, Vector table used for interrupts)
#pragma config IVT1WAY = OFF     // IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared and set only once)
#pragma config LPBOREN = OFF    // Low Power BOR Enable bit (Low-Power BOR disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

// CONFIG4
#pragma config BORV = VBOR_1P9  // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 1.9V)
#pragma config ZCD = OFF        // ZCD Disable bit (ZCD module is disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
#pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be set and cleared repeatedly (subject to the unlock sequence))
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (HV on MCLR/VPP must be used for programming)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode disabled)

// CONFIG5
#pragma config WDTCPS = WDTCPS_31// WDT Period selection bits (Divider ratio 1:65536; software control of WDTPS)
#pragma config WDTE = OFF       // WDT operating mode (WDT Disabled; SWDTEN is ignored)

// CONFIG6
#pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
#pragma config WDTCCS = SC      // WDT input clock selector (Software Control)

// CONFIG7
#pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
#pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
#pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
#pragma config DEBUG = OFF      // Background Debugger (Background Debugger disabled)

// CONFIG8
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not Write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not Write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not Write protected)
#pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
#pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

// CONFIG9
#pragma config BOOTPINSEL = RC5 // CRC on boot output pin selection (CRC on boot output pin is RC5)
#pragma config BPEN = OFF       // CRC on boot output pin enable bit (CRC on boot output pin disabled)
#pragma config ODCON = OFF      // CRC on boot output pin open drain bit (Pin drives both high-going and low-going signals)

// CONFIG10
#pragma config CP = ON         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection enabled)

// CONFIG11
#pragma config BOOTSCEN = OFF   // CRC on boot scan enable for boot area (CRC on boot will not include the boot area of program memory in its calculation)
#pragma config BOOTCOE = HALT   // CRC on boot Continue on Error for boot areas bit (CRC on boot will stop device if error is detected in boot areas)
#pragma config APPSCEN = OFF    // CRC on boot application code scan enable (CRC on boot will not include the application area of program memory in its calculation)
#pragma config SAFSCEN = OFF    // CRC on boot SAF area scan enable (CRC on boot will not include the SAF area of program memory in its calculation)
#pragma config DATASCEN = OFF   // CRC on boot Data EEPROM scan enable (CRC on boot will not include data EEPROM in its calculation)
#pragma config CFGSCEN = OFF    // CRC on boot Config fuses scan enable (CRC on boot will not include the configuration fuses in its calculation)
#pragma config COE = HALT       // CRC on boot Continue on Error for non-boot areas bit (CRC on boot will stop device if error is detected in non-boot areas)
#pragma config BOOTPOR = OFF    // Boot on CRC Enable bit (CRC on boot will not run)

// CONFIG12
#pragma config BCRCPOLT = hFF   // Boot Sector Polynomial for CRC on boot bits 31-24 (Bits 31:24 of BCRCPOL are 0xFF)

// CONFIG13
#pragma config BCRCPOLU = hFF   // Boot Sector Polynomial for CRC on boot bits 23-16 (Bits 23:16 of BCRCPOL are 0xFF)

// CONFIG14
#pragma config BCRCPOLH = hFF   // Boot Sector Polynomial for CRC on boot bits 15-8 (Bits 15:8 of BCRCPOL are 0xFF)

// CONFIG15
#pragma config BCRCPOLL = hFF   // Boot Sector Polynomial for CRC on boot bits 7-0 (Bits 7:0 of BCRCPOL are 0xFF)

// CONFIG16
#pragma config BCRCSEEDT = hFF  // Boot Sector Seed for CRC on boot bits 31-24 (Bits 31:24 of BCRCSEED are 0xFF)

// CONFIG17
#pragma config BCRCSEEDU = hFF  // Boot Sector Seed for CRC on boot bits 23-16 (Bits 23:16 of BCRCSEED are 0xFF)

// CONFIG18
#pragma config BCRCSEEDH = hFF  // Boot Sector Seed for CRC on boot bits 15-8 (Bits 15:8 of BCRCSEED are 0xFF)

// CONFIG19
#pragma config BCRCSEEDL = hFF  // Boot Sector Seed for CRC on boot bits 7-0 (Bits 7:0 of BCRCSEED are 0xFF)

// CONFIG20
#pragma config BCRCEREST = hFF  // Boot Sector Expected Result for CRC on boot bits 31-24 (Bits 31:24 of BCRCERES are 0xFF)

// CONFIG21
#pragma config BCRCERESU = hFF  // Boot Sector Expected Result for CRC on boot bits 23-16 (Bits 23:16 of BCRCERES are 0xFF)

// CONFIG22
#pragma config BCRCERESH = hFF  // Boot Sector Expected Result for CRC on boot bits 15-8 (Bits 15:8 of BCRCERES are 0xFF)

// CONFIG23
#pragma config BCRCERESL = hFF  // Boot Sector Expected Result for CRC on boot bits 7-0 (Bits 7:0 of BCRCERES are 0xFF)

// CONFIG24
#pragma config CRCPOLT = hFF    // Non-Boot Sector Polynomial for CRC on boot bits 31-24 (Bits 31:24 of CRCPOL are 0xFF)

// CONFIG25
#pragma config CRCPOLU = hFF    // Non-Boot Sector Polynomial for CRC on boot bits 23-16 (Bits 23:16 of CRCPOL are 0xFF)

// CONFIG26
#pragma config CRCPOLH = hFF    // Non-Boot Sector Polynomial for CRC on boot bits 15-8 (Bits 15:8 of CRCPOL are 0xFF)

// CONFIG27
#pragma config CRCPOLL = hFF    // Non-Boot Sector Polynomial for CRC on boot bits 7-0 (Bits 7:0 of CRCPOL are 0xFF)

// CONFIG28
#pragma config CRCSEEDT = hFF   // Non-Boot Sector Seed for CRC on boot bits 31-24 (Bits 31:24 of CRCSEED are 0xFF)

// CONFIG29
#pragma config CRCSEEDU = hFF   // Non-Boot Sector Seed for CRC on boot bits 23-16 (Bits 23:16 of CRCSEED are 0xFF)

// CONFIG30
#pragma config CRCSEEDH = hFF   // Non-Boot Sector Seed for CRC on boot bits 15-8 (Bits 15:8 of CRCSEED are 0xFF)

// CONFIG31
#pragma config CRCSEEDL = hFF   // Non-Boot Sector Seed for CRC on boot bits 7-0 (Bits 7:0 of CRCSEED are 0xFF)

// CONFIG32
#pragma config CRCEREST = hFF   // Non-Boot Sector Expected Result for CRC on boot bits 31-24 (Bits 31:24 of CRCERES are 0xFF)

// CONFIG33
#pragma config CRCERESU = hFF   // Non-Boot Sector Expected Result for CRC on boot bits 23-16 (Bits 23:16 of CRCERES are 0xFF)

// CONFIG34
#pragma config CRCERESH = hFF   // Non-Boot Sector Expected Result for CRC on boot bits 15-8 (Bits 15:8 of CRCERES are 0xFF)

// CONFIG35
#pragma config CRCERESL = hFF   // Non-Boot Sector Expected Result for CRC on boot bits 7-0 (Bits 7:0 of CRCERES are 0xFF)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#pragma warning disable 520
#pragma warning disable 1090
#pragma warning disable 1498
#pragma warning disable 2053

#ifndef __DEFINED_int24_t
typedef signed long long int24_t;
#define __DEFINED_int24_t
#endif

#include <stdio.h>
#include <string.h>
#include <time.h>
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/uart1.h"
#include "eadog.h"
#include "timers.h"
#include "mconfig.h"
#include "mydisplay.h"
#include "rs232.h"
#include "slaveo.h"
#include "bmcdio.h"
#include "mxcmd.h"
#include "modbus_master.h"
#include "calibr.h"

#ifdef TRACE
#define M_TRACE		TP1_Toggle()
#define M_TRACEL	TP1_SetLow()
#define M_TRACEH	TP1_SetHigh()
#else
#define M_TRACE		""
#define M_TRACEL	""
#define M_TRACEH	""
#endif

extern volatile struct spi_link_type spi_link;
static const char *build_date = __DATE__, *build_time = __TIME__;
const char text_test[] = {"the quick brown fox jumps over the lazy dogs back"};

const char * BMC_TEXT [] = {
	"DISABLE",
	"COMM   ",
	"OFFLINE",
	"ONLINE ",
	"REMOTE ",
	"ERROR  "
};

V_data V = {
	.checksum_error = 0,
	.all_errors = 0,
	.timer_error = 0,
	.response.info = DIS_STR,
	.response.log_num = 0,
	.response.log_seq = 0,
	.response.host_display_ack = false,
	.queue = false,
	.stack = 0, // 0 no messages, 1-10 messages in queue
	.sid = 1,
	.help_id = 0,
	.ping_count = 0,
	.sequences = 0,
	.set_sequ = false,
	.euart = EQUIP_UART,
	.tx_total = 0,
	.rx_total = 0,
	.vterm = MAIN_VTERM,
	.tx_rs232 = 'O',
	.rx_rs232 = 'O',
	.debug = true,
	.rerror = false,
	.help = false,
	.secs_value = 0,
	.cmd_value = 0,
	.utc_cmd_value = 0,
	.utc_ticks = DEF_TIME,
	.log_s6f11 = true,
	.log_abort = false,
	.log_char = false,
	.bmc_ao = 0,
	.di_fail = false,
	.do_fail = false,
};

BM_type BM = {
	.one_sec_flag = false,
	.ten_sec_flag = false,
	.pacing = 0,
	.rx_count = 0,
	.flush = 0,
	.canbus_online = 0,
	.modbus_online = 0,
	.log.select = 1,
	.pv_high = false,
	.pv_prev = STATUS_SLEEPING,
	.pv_update = false,
	.once = false,
	.log.type = 1, // mxlog type
	.display_dim = false,
	.display_update = false,
	.dim_delay = DIM_DELAY,
	.display_on = true,
	.fm80_restart = false,
};

volatile struct spi_link_type_ss spi_comm_ss = {false, false, false, false, false, false, false, false};
volatile struct spi_stat_type_ss spi_stat_ss = {
	.raw_index = 0,
	.daq_conf = 0,
	.deviceid = 0,
	.devicerev = 0,
};
volatile struct serial_buffer_type_ss serial_buffer_ss = {
	.data[BMC_CMD] = CHECKBYTE,
	.raw_index = BMC_CMD,
	.r_string_index = 0,
	.r_string_chan = 3,
};

struct ha_daq_calib_type ha_daq_calib = {
	.checkmark = CHECKMARK,
	.scaler4 = HV_SCALAR4,
	.scaler5 = HV_SCALAR5,
	.offset4 = HV_SCALE_OFFSET,
	.offset5 = HV_SCALE_OFFSET,
	.A200_Z = A200_0_ZERO,
	.A200_S = A200_0_SCALAR,
};

volatile uint8_t data_in2, adc_buffer_ptr = 0, adc_channel = 0, channel = 0, upper;
volatile uint16_t adc_buffer[AI_BUFFER_NUM] = {0}, adc_data_in = 0, out_buf = 0;
uint16_t volt_whole, bat_amp_whole = AMP_WHOLE_ZERO, bat_amp_frac = AMP_WHOLE_ZERO, bat_amp_panel = AMP_WHOLE_ZERO, panel_watts, volt_fract, vf, vw, pvf, pvw, af, aw;
uint8_t fw_state = 0;

volatile uint16_t tickCount[TMR_COUNT] = {0};
volatile uint8_t mode_sw = false, faker;
void onesec_io(void);
void test_slave(void);
void SetBMCPriority(void);

/*
 * FM80 send/recv functions
 */
enum state_type {
	state_init,
	state_status,
	state_panel,
	state_batteryv,
	state_batterya,
	state_watts,
	state_fwrev,
	state_time,
	state_date,
	state_mx_log,
	state_misc,
	state_mx_status,
	state_last,
};

#define PACE            8000	// commands delay in count units
#define CMD_LEN         8
#define REC_LEN         5
#define REC_STATUS_LEN	16
#define REC_LOG_LEN     17

static uint16_t abuf[FM_BUFFER], cbuf[FM_BUFFER + 2];
volatile enum state_type state = state_init;
volatile uint16_t cc_mode = STATUS_LAST, mx_code = 0x00;
volatile char buffer[MAX_B_BUF], log_buffer[MAX_B_BUF];
volatile struct bmc_buffer_type BMC4 = {
	.log_buffer = log_buffer,
	.buffer = buffer,
	.d_id = DC1_CMD,
};

struct tm *bmc_newtime;

static void send_mx_cmd(const uint16_t *);
static void rec_mx_cmd(void (* DataHandler)(void), const uint8_t);
/*
 * callbacks to handle FM80 register data
 */
void state_init_cb(void);
void state_status_cb(void);
void state_panelv_cb(void);
void state_batteryv_cb(void);
void state_batterya_cb(void);
void state_watts_cb(void);
void state_misc_cb(void);
void state_mx_status_cb(void);
void state_mx_log_cb(void);
static void state_fwrev_cb(void);
static void state_time_cb(void);
static void state_date_cb(void);
static void state_restart_cb(void);

void bmc_logger(void);

/** \file main.c
 * Lets get going with the code.
 * Main application
 */
void main(void)
{
	char * s, * speed_text;
#ifdef FLIP_SERIAL
	uint8_t temp_lock = false;
#endif
	SPI2STATUSbits.SPI2CLRBF;

	// Initialize the device
	SYSTEM_Initialize();

	PIE1bits.ADIE = 0; // lock ADC interrupts off
	SPI_OPI();
	// Enable high priority global interrupts
	INTERRUPT_GlobalInterruptHighEnable();

	// Enable low priority global interrupts.
	INTERRUPT_GlobalInterruptLowEnable();

	MLED_SetHigh();
	RLED_SetHigh();
	DLED_SetHigh();

	SetBMCPriority(); // ISR Priority > Peripheral Priority > Main Priority

	mconfig_init(); // zero the entire text buffer

	V.ui_state = UI_STATE_INIT;

	/*
	 * read and store the MUI for PCB tracing
	 */
	BM.node_id = 0;
	for (uint8_t i = 0; i <= 8; i++) {
		BM.mui[i] = DeviceID_Read(DIA_MUI + (i * 2)); // Read Microchip Unique Identifier from memory and store in array
		BM.node_id += BM.mui[i];
	}
	spi_stat_ss.mui = BM.node_id;
	spi_stat_ss.deviceid = DeviceID_Read(0x3ffffe);
	if (spi_stat_ss.deviceid != F57Q84) {
		spi_stat_ss.daq_conf |= 0x04; // Not a 57Q84 controller, 47Q84 likely
	}
	spi_stat_ss.devicerev = DeviceID_Read(0x3ffffc);

	/*
	 * calibration scalar selection using MUI from controller
	 */
	set_calibration(spi_stat_ss.mui);

#ifdef MB_MASTER
	UART3_SetRxInterruptHandler(my_modbus_rx_32); // install custom serial receive ISR
	init_mb_master_timers(); // pacing, spacing and timeouts

	StartTimer(TMR_MBTEST, 20);
	StartTimer(TMR_RESTART, 20000);

#ifdef NO_NODE_ID
	BM.node_id = 0; // set to zero to only use EMON type number as the CAN packet ID
#else
	BM.node_id = BM.node_id & 0xffffffff; // using full number
#endif

#endif
	TMR2_StartTimer();
	// timer3 2ms in init_mb_master_timers
	// timer4 500ms in init_mb_master_timers
	TMR5_SetInterruptHandler(onesec_io);
	TMR5_StartTimer();
	TMR6_StartTimer(); // software timer and FM80 I/O
	TMR6_SetInterruptHandler(FM_io);
	TMR0_SetInterruptHandler(test_slave);
	TMR0_StartTimer();
	TMR1_SetInterruptHandler(FM_tensec_io); // really TWO seconds
	TMR1_StartTimer();

	speed_text = "Locked 115200bps";
	//    UART2_Initialize115200();
	UART1_Initialize115200();
	WaitMs(SDELAY);
	RLED_SetLow(); // start complete power-up serial speed setups, LEDS OFF
	MLED_SetLow();
	DLED_SetLow();

	/*
	 * master processing I/O loop
	 */
	while (true) {
		TP1_SetHigh();
		/*
		 * check and parse logging configuration commands
		 * not used
		 */
		logging_cmds();

		if ((spi_stat_ss.daq_conf & 0x04) == 0x00) { // the 47Q84 board has no EM540 or FM80 support
#ifdef MB_MASTER
			TP1_SetLow();
			master_controller_work(&C); // master MODBUS processing
			TP1_SetHigh();
#endif
#ifdef MX_MATE
			/*
			 * FM80 processing state machine
			 */
			switch (state) {
			case state_init:
				send_mx_cmd(cmd_id);
				rec_mx_cmd(state_init_cb, REC_LEN);
				break;
			case state_status:
				if (!BM.fm80_restart) {
					send_mx_cmd(cmd_status);
					rec_mx_cmd(state_status_cb, REC_LEN);
				} else {
					send_mx_cmd(cmd_restart);
					rec_mx_cmd(state_restart_cb, REC_LEN);
				}
				break;
			case state_panel:
				send_mx_cmd(cmd_panelv);
				rec_mx_cmd(state_panelv_cb, REC_LEN);
				break;
			case state_batteryv:
				send_mx_cmd(cmd_batteryv);
				rec_mx_cmd(state_batteryv_cb, REC_LEN);
				break;
			case state_batterya:
				send_mx_cmd(cmd_batterya);
				rec_mx_cmd(state_batterya_cb, REC_LEN);
				break;
			case state_watts:
				send_mx_cmd(cmd_watts);
				rec_mx_cmd(state_watts_cb, REC_LEN);
				break;
			case state_mx_status: // wait for ten second flag in this state for logging
				send_mx_cmd(cmd_mx_status);
				rec_mx_cmd(state_mx_status_cb, REC_STATUS_LEN);
				break;
			case state_fwrev:
				switch (fw_state) {
				case 0:
					send_mx_cmd(cmd_fwreva);
					rec_mx_cmd(state_fwrev_cb, REC_LEN);
					break;
				case 1:
					send_mx_cmd(cmd_fwrevb);
					rec_mx_cmd(state_fwrev_cb, REC_LEN);
					break;
				case 2:
					send_mx_cmd(cmd_fwrevc);
					rec_mx_cmd(state_fwrev_cb, REC_LEN);
				default:
					fw_state = 0;
					break;
				}
				break;
			case state_mx_log: // FM80 log data
				send_mx_cmd(cmd_mx_log);
				rec_mx_cmd(state_mx_log_cb, REC_LOG_LEN);
				break;
			case state_time: // FM80 send time data
				send_mx_cmd(cmd_time);
				rec_mx_cmd(state_time_cb, REC_LEN);
				break;
			case state_date: // FM80 send date data
				send_mx_cmd(cmd_date);
				rec_mx_cmd(state_date_cb, REC_LEN);
				break;
			case state_misc:
				send_mx_cmd(cmd_misc);
				rec_mx_cmd(state_misc_cb, REC_LEN);
				break;
			default:
				send_mx_cmd(cmd_id);
				rec_mx_cmd(state_init_cb, REC_LEN);
				break;
			}

			if (TimerDone(TMR_RESTART)) {
				StartTimer(TMR_RESTART, 30000);
				BM.fm80_restart = false;
			}
#endif
		}
		/*
		 * protocol state machine
		 */
		switch (V.ui_state) {
		case UI_STATE_INIT:
			init_display();
			eaDogM_CursorOff();

			set_vterm(V.vterm); // set to buffer 0
			snprintf(get_vterm_ptr(0, MAIN_VTERM), MAX_TEXT, "Port %s             ", speed_text);
			snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, "Port %s             ", speed_text);
			snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, "Port %s             ", speed_text);
			snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, "Port %s             ", speed_text);
			refresh_lcd();
			WaitMs(LDELAY);

			SPI_TIC12400();
			tic12400_reset();
			if (!tic12400_init()) {
				V.di_fail = true;
				failure = true;
				spi_stat_ss.daq_conf |= 0x01; // fail DI
			};
			SPI_MC33996();
			if (!mc33996_init()) {
				V.do_fail = true;
				failure = true;
				spi_stat_ss.daq_conf |= 0x02; // fail DO
			};
			init_slaveo();
			SPI_EADOG();

			V.ui_state = UI_STATE_HOST;
			srand(1957);
			set_vterm(V.vterm); // set to buffer 0
			snprintf(V.info, MAX_INFO, " Terminal Info               ");
			snprintf(get_vterm_ptr(0, MAIN_VTERM), MAX_TEXT, " OPI DAQ_BMC %s        ", VER);
#ifdef DIS_DEBUG
			if (V.di_fail) {
				snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, " TIC %ld 0x%lX 0x%lX         ", tic12400_fail_value, tic12400_id & 0xffff, tic12400_read_status);
			}
			if (V.do_fail) {
				snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, " MC 0x%X 0x%X 0x%X         ", mc_init.cmd[3], mc_init.cmd[4], mc_init.cmd[5]);
			}
#else
			snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, " %llx Run Display             ", spi_stat_ss.mui);
#endif
			if (failure) {
				snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, " NSASPOOK Analog Dev  ");
			} else {
				snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, " NSASPOOK All Dev     ");
			}
			snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, " %s                   ", (char *) build_date);
			snprintf(get_vterm_ptr(0, INFO_VTERM), MAX_TEXT, " INFO                 ");
			snprintf(get_vterm_ptr(1, INFO_VTERM), MAX_TEXT, " Version %s           ", VER);
			snprintf(get_vterm_ptr(2, INFO_VTERM), MAX_TEXT, " VTERM INFO           ");
			snprintf(get_vterm_ptr(3, INFO_VTERM), MAX_TEXT, " %s                   ", (char *) build_date);
			snprintf(get_vterm_ptr(0, HELP_VTERM), MAX_TEXT, " HELP Build %s        ", VER);
			snprintf(get_vterm_ptr(1, HELP_VTERM), MAX_TEXT, " Version %s           ", VER);
			snprintf(get_vterm_ptr(2, HELP_VTERM), MAX_TEXT, " VTERM HELP           ");
			snprintf(get_vterm_ptr(3, HELP_VTERM), MAX_TEXT, " %s                   ", (char *) build_date);
			snprintf(get_vterm_ptr(0, DBUG_VTERM), MAX_TEXT, "MUI %llX PIC %X                ", spi_stat_ss.mui, spi_stat_ss.deviceid);
			snprintf(get_vterm_ptr(1, DBUG_VTERM), MAX_TEXT, "OF %lu ERR %lu                        ", spi_stat_ss.rxof_bit, spi_stat_ss.spi_error_count);
			snprintf(get_vterm_ptr(2, DBUG_VTERM), MAX_TEXT, "BMC %lu                               ", spi_stat_ss.bmc_counts);
			snprintf(get_vterm_ptr(3, DBUG_VTERM), MAX_TEXT, "%s Ver %s                       ", (char *) build_date, VER);
			refresh_lcd();
			WaitMs(TDELAY);
			if (failure) { // DIO not working or missing
				WaitMs(SEQDELAY);
			}
			StartTimer(TMR_DISPLAY, DDELAY);
			StartTimer(TMR_INFO, TDELAY);
			StartTimer(TMR_FLIPPER, DFLIP);
			StartTimer(TMR_HELPDIS, TDELAY);
			StartTimer(TMR_SEQ, SEQDELAY);
			StartTimer(TMR_HELP, TDELAY);
			StartTimer(TMR_ADC, ADCDELAY);
			break;
		case UI_STATE_HOST:
			set_display_info(DIS_STR);
			s = get_vterm_ptr(0, MAIN_VTERM);
			s[MAX_LINE] = 0;
			s[SPIN_CHAR] = spinners(3, false);
			break;
		default:
			V.ui_state = UI_STATE_INIT;
#ifdef DIS_DEBUG
			refresh_lcd();
#endif
			WaitMs(TDELAY);
			break;
		}

		if (TimerDone(TMR_ADC)) {
			StartTimer(TMR_ADC, ADCDELAY);

			if (!V.do_fail) {
				SPI_MC33996();
				/*
				 * interrupt lock for 16-bit atomic updates of out_buf
				 */
				INTERRUPT_GlobalInterruptHighDisable();
				if (serial_buffer_ss.make_value == false) {
					out_buf = (uint16_t) 0xffff & V.bmc_do;
				}
				INTERRUPT_GlobalInterruptHighEnable();
				mc33996_update(out_buf);
			}

			SPI_EADOG();
			spi_stat_ss.adc_count++; // just keep count

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_ANA0);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_ANA0] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_ANA1);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				V.v_tx_line = ADC_GetConversionResult();
				adc_buffer[channel_ANA1] = V.v_tx_line;
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_ANA2);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				V.v_rx_line = ADC_GetConversionResult();
				adc_buffer[channel_ANA2] = V.v_rx_line;
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_ANA4);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_ANA4] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_ANA5);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_ANA5] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_ANC6);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_ANC6] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_ANC7);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_ANC7] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_AND5);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_AND5] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_VSS);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_VSS] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_Temp);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_Temp] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_DAC1);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_DAC1] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_FVR_Buffer1);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_FVR_Buffer1] = ADC_GetConversionResult();
			};

			ADC_DischargeSampleCapacitor();
			ADC_StartConversion(channel_FVR_Buffer2);
			while (!ADC_IsConversionDone()) {
			};
			if (ADC_IsConversionDone()) {
				adc_buffer[channel_FVR_Buffer2] = ADC_GetConversionResult();
			};
			DAC1DATL = (uint8_t) V.bmc_ao; // update DAC1 output

			if (!V.di_fail) {
				SPI_TIC12400();
				/*
				 * spi_link.rxbuf has interrupt lock for 24-bit atomic updates inside tic12400_read_sw
				 */
				tic12400_read_sw(0, (uintptr_t) NULL);
				if (((spi_link.rxbuf[0] & por_bit_s_v))) { // check for POR bit set
					tic12400_init();
				}
			}
		}

		if (TimerDone(TMR_DISPLAY)) { // limit update rate
			static uint8_t switcher = INFO_VTERM;

			SPI_EADOG();
			StartTimer(TMR_DISPLAY, DDELAY);

			if (TimerDone(TMR_HELPDIS)) {
				set_display_info(DIS_STR);
			}
			check_lcd_dim(false);
			/*
			 * send ascii data to CLCD
			 */
			if (r_string_ready) {
				bmc_logger();
				snprintf(get_vterm_ptr(0, MAIN_VTERM), MAX_TEXT, "%s                         ", &BMC4.log_buffer[2]);
				snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, "%s Vac%ld A%3.2f           ", modbus_name[C.id_ok], em.vl3l1 / 10, ((float) em.al1) / 1000.0f);
				snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, "%s %s A%d.%01d A%d              ", FM80_name[BM.FM80_online], state_name[cc_mode], bat_amp_whole - 128, bat_amp_frac - 128, bat_amp_panel - 128);
				snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, "BAT V%d.%01d PV V%d.%01d                 ", vw, vf, pvw, pvf);

				snprintf(get_vterm_ptr(0, INFO_VTERM), MAX_TEXT, "kwh %u float %u                       ", BM.log.kilowatt_hours, BM.log.float_time);
				snprintf(get_vterm_ptr(1, INFO_VTERM), MAX_TEXT, "Bmax %u Bmin %u                      ", BM.log.bat_max, BM.log.bat_min);
				snprintf(get_vterm_ptr(2, INFO_VTERM), MAX_TEXT, "V%ld A%3.2f                        ", em.vl1l2, ((float) em.al1) / 1000.0f);
				snprintf(get_vterm_ptr(3, INFO_VTERM), MAX_TEXT, "W%ld VA%ld P%d             ", em.wl1, em.val1, em.pfsys);

				snprintf(get_vterm_ptr(0, DBUG_VTERM), MAX_TEXT, "MUI %llX PIC %X                ", spi_stat_ss.mui, spi_stat_ss.deviceid);
				snprintf(get_vterm_ptr(1, DBUG_VTERM), MAX_TEXT, "4 %6.3f,5 %6.3f                      ", phy_chan4(adc_buffer[channel_ANA4]), phy_chan5(adc_buffer[channel_ANA5]));
				snprintf(get_vterm_ptr(2, DBUG_VTERM), MAX_TEXT, "BMC %lu                               ", spi_stat_ss.bmc_counts);
				snprintf(get_vterm_ptr(3, DBUG_VTERM), MAX_TEXT, "%s %s                       ", (char *) build_date, VER);

				refresh_lcd();
				serial_buffer_ss.r_string_index = 0;
				r_string_ready = false;
				set_lcd_dim(false);
			}
#ifdef DIS_DEBUG

#ifdef AIO_TEST // analog 12-bit values
			snprintf(get_vterm_ptr(0, MAIN_VTERM), MAX_TEXT, "%u,%u,%u,%u               ",
				adc_buffer[channel_ANA0], adc_buffer[channel_ANA1], adc_buffer[channel_ANA2], adc_buffer[channel_ANA4]);
			snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, "%u,%u,%u,%u                ",
				adc_buffer[channel_ANA5], adc_buffer[channel_ANC6], adc_buffer[channel_ANC7], adc_buffer[channel_AND5]);
			snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, "CFG%.2X D%X R%X                   ", // bmc config, deviceid, devicerev
				spi_stat_ss.daq_conf, spi_stat_ss.deviceid, spi_stat_ss.devicerev);
#ifdef SHOW_DAC
			snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, "DAC %2u                           ", // 8-bit dax value
				adc_buffer[channel_DAC1]);
#else
			snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, "4 %6.3fV, 5 %6.3fV                      ", phy_chan4(adc_buffer[channel_ANA4]), phy_chan5(adc_buffer[channel_ANA5]));
#endif
#else
#ifdef DIO_TEST
#ifdef DIO_SHOW_BUF
			if (spi_stat_ss.slave_tx_count < 0x2fff) { // clear startup counts of empty fifo
				spi_stat_ss.spi_noerror_count = 0;
			}
			snprintf(get_vterm_ptr(0, MAIN_VTERM), MAX_TEXT, "%.4lx %.6lx %lx %lx            ",
				V.bmc_do, V.bmc_di, spi_stat_ss.port_count, spi_stat_ss.slave_tx_count);

			snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, "0x%.2x 0x%.2x 0x%.2x 0x%.2x                  ",
#ifdef SER_DEBUG
				serial_buffer_ss.data[3], serial_buffer_ss.data[2], serial_buffer_ss.data[1], serial_buffer_ss.data[0]);
#else
#ifndef DI_MC_CMD
				mc_init.cmd[3], mc_init.cmd[4], mc_init.cmd[5], serial_buffer_ss.data[0]);
#else
				tic_rw.cmd[0], tic_rw.cmd[1], tic_rw.cmd[2], tic_rw.cmd[3]);
#endif
#endif

			snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, "TX %lx, RX %lx, %d %d %d         ",
				spi_stat_ss.txdone_bit, spi_stat_ss.rxof_bit, SPI2INTFbits.TCZIF, SPI2STATUSbits.SPI2RXRE, SPI2STATUSbits.SPI2TXWE);

#else
			snprintf(get_vterm_ptr(0, MAIN_VTERM), MAX_TEXT, "%.2x %.2x %.2x %.2x %lu %lu %lx             ", spi_link.rxbuf[3], spi_link.rxbuf[2], spi_link.rxbuf[1], spi_link.rxbuf[0], tic12400_parity_count, tic12400_fail_value,
				tic12400_id);
			snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, "%lx %lx %lx %lx %lx                   ", tic12400_fail_count, spi_link.des_bytes, tic12400_value_counts, tic12400_switch, tic12400_status);

#endif
#else
			snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, "%lu %lu %lu %lu                    ", spi_stat_ss.spi_error_count, spi_stat_ss.adc_count, spi_stat_ss.slave_tx_count, spi_stat_ss.slave_int_count);
			snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, "A1 0x%.2x, A2 0x%.2x               ", V.v_tx_line, V.v_rx_line);

#endif


#ifdef DI_DEBUG
			snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, "vc%u fv%d fc%u rs%X                   ",
				tic12400_value_counts, tic12400_fail_value, tic12400_fail_count, tic12400_read_status);
#else
			snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, "0x%.2lx 0x%.2lx %x %lu 0X%.2X                 ",
				spi_stat_ss.spi_error_count, spi_stat_ss.txuf_bit, V.bmc_ao, tic12400_value_counts, spi_stat_ss.daq_conf);
#endif
#endif
			// convert ADC values to char for display
			update_rs232_line_status();
#endif

			if (V.vterm_switch++ > (SWITCH_VTERM)) {
				set_vterm(switcher);
				if (V.vterm_switch > (SWITCH_VTERM + V.ticker + SWITCH_DURATION)) {
					switcher++;
					if ((switcher & 0x03) == HELP_VTERM) { // mask [0..3]]
						switcher = INFO_VTERM; // skip short display of the main vterm
					}
					V.vterm_switch = 0;
				}
			} else {
				set_vterm(V.vterm);
			}
			/*
			 * update info screen data points
			 */
#ifdef DIS_DEBUG
			snprintf(get_vterm_ptr(0, INFO_VTERM), MAX_TEXT, "RS232 TX %3dV:%c                       ", V.tx_volts, V.tx_rs232);
			snprintf(get_vterm_ptr(1, INFO_VTERM), MAX_TEXT, "RS232 RX %3dV:%c                       ", V.rx_volts, V.rx_rs232);
			snprintf(get_vterm_ptr(2, INFO_VTERM), MAX_TEXT, "LT 0x%.2x, LR 0x%.2x                   ", V.v_tx_line, V.v_rx_line);
			snprintf(get_vterm_ptr(3, INFO_VTERM), MAX_TEXT, "B0 0x%.2X, B1 0x%.2X                    ", serial_buffer_ss.data[0], serial_buffer_ss.data[1]);

			snprintf(get_vterm_ptr(0, DBUG_VTERM), MAX_TEXT, "MUI %llX                                  ", spi_stat_ss.mui);
			snprintf(get_vterm_ptr(1, DBUG_VTERM), MAX_TEXT, "Dev %X Rev %X                            ", spi_stat_ss.deviceid, spi_stat_ss.devicerev);
			snprintf(get_vterm_ptr(2, DBUG_VTERM), MAX_TEXT, "4 %6.3fV, 5 %6.3fV                      ", ((float) adc_buffer[channel_ANA4] / 4096.0f) * ha_daq_calib.scaler4, ((float) adc_buffer[channel_ANA5] / 4096.0f) * ha_daq_calib.scaler5);
			snprintf(get_vterm_ptr(3, DBUG_VTERM), MAX_TEXT, "SPI2 errors 0x%.2lx                                 ",
				spi_stat_ss.spi_error_count);
#endif
			/*
			 * don't default update the LCD when displaying HELP text
			 */
			if (!V.set_sequ) {
#ifdef DIS_DEBUG
				refresh_lcd();
#endif
			}
		}

		/*
		 * show help messages if flag is set for timer duration
		 */
		if (V.set_sequ) {
			if (TimerDone(TMR_HELP)) {
				V.set_sequ = false;
				set_vterm(V.vterm);
#ifdef DIS_DEBUG
				refresh_lcd();
#endif
			} else {
				set_vterm(HELP_VTERM);
#ifdef DIS_DEBUG
				refresh_lcd();
#endif
			}
		}

		if (V.help && TimerDone(TMR_SEQ)) {
			StartTimer(TMR_SEQ, SEQDELAY);
			StartTimer(TMR_HELP, TDELAY);
			V.set_sequ = true;
			check_help(false);
		}
		TP1_SetLow();
	}
}

/*
 * run LED and status indicators
 */
void onesec_io(void)
{
	RLED_Toggle();
	MLED_SetLow();
	DLED_SetLow();
	V.utc_ticks++;
	BM.one_sec_flag = true;
}

/* Misc ACSII spinner character generator, stores position for each shape */
char spinners(uint8_t shape, const uint8_t reset)
{
	static uint8_t s[MAX_SHAPES];
	char c;

	if (shape > (MAX_SHAPES - 1))
		shape = 0;
	if (reset)
		s[shape] = 0;
	c = spin[shape][s[shape]];
	if (++s[shape] >= strlen(spin[shape]))
		s[shape] = 0;

	return c;
}

/*
 * Master activity reset timer, no SPI comms for 1 second causes system restart
 */
void test_slave(void)
{
	MCZ_PWM_SetLow();
}

/*
 * ISR Priority > Peripheral Priority > Main Priority
 * '0' being the highest priority selection and the maximum value being the lowest priority
 */
void SetBMCPriority(void)
{
	INTCON0bits.GIE = 0; // Disable Interrupts;
	PRLOCK = 0x55;
	PRLOCK = 0xAA;
	PRLOCKbits.PRLOCKED = 0; // Allow changing priority settings;
	INTCON0bits.GIE = 1; // Enable Interrupts;
	ISRPR = 1;
	DMA1PR = 2;
	MAINPR = 7;
	INTCON0bits.GIE = 0; // Disable Interrupts;
	PRLOCK = 0x55;
	PRLOCK = 0xAA;
	PRLOCKbits.PRLOCKED = 1; // Grant memory access to peripherals;
	INTCON0bits.GIE = 1; // Enable Interrupts;
}

device_id_data_t DeviceID_Read(device_id_address_t address)
{
	device_id_data_t deviceID;

	//Save the table pointer
	uint32_t tablePointer = ((uint32_t) TBLPTRU << 16) | ((uint32_t) TBLPTRH << 8) | ((uint32_t) TBLPTRL);

	//Load table pointer with Device ID address
	TBLPTRU = (uint8_t) (address >> 16);
	TBLPTRH = (uint8_t) (address >> 8);
	TBLPTRL = (uint8_t) address;

	//Execute table read and increment table pointer
	asm("TBLRD*+");

	deviceID = (device_id_data_t) TABLAT;

	//Execute table read
	asm("TBLRD*");

	deviceID |= (device_id_data_t) (TABLAT << 8);

	//Restore the table pointer
	TBLPTRU = (uint8_t) (tablePointer >> 16);
	TBLPTRH = (uint8_t) (tablePointer >> 8);
	TBLPTRL = (uint8_t) tablePointer;

	return deviceID;
}

/*
 * pack UNIX tm time/date into FM80 compatible 16-bit values
 */
void update_time(const struct tm * ts, EB_data * EB)
{
	EB->time = (
		(uint16_t) ((ts->tm_hour & 0x1F) << 11) |
		(uint16_t) ((ts->tm_min & 0x3F) << 5) |
		(uint16_t) ((ts->tm_sec & 0x1F) >> 1)
		);
	EB->date = (
		(uint16_t) (((ts->tm_year - 2000) & 0x7F) << 9) |
		(uint16_t) ((ts->tm_mon & 0x0F) << 5) |
		(uint16_t) (ts->tm_mday & 0x1F)
		);
}

/*
 * transmit the cmd data
 */
static void send_mx_cmd(const uint16_t * cmd)
{
	if (FM_tx_empty()) {
		if (BM.pacing++ > PACE) {
			FM_tx(cmd, CMD_LEN); // send 9-bit command data stream
			BM.pacing = 0;
		}
	}
}

/*
 * process received data from the FM80 9n1 serial in abuf 16-bit buffer array with callbacks
 */
static void rec_mx_cmd(void (* DataHandler)(void), const uint8_t rec_len)
{
	static uint16_t online_count = 0;

	if (FM_rx_ready()) {
		if (FM_rx_count() >= rec_len) {
			online_count = 0;
			if (rec_len == REC_LOG_LEN) {
				FM_rx(cbuf);
			} else {
				FM_rx(abuf);
			}
			BM.FM80_io = false;
			DataHandler(); // execute callback to process data in abuf
		} else {
			if (online_count++ > ONLINE_TIMEOUT) {
				online_count = 0;
				BM.FM80_online = false;
				BM.FM80_io = false;
				cc_mode = STATUS_LAST;
				state = state_init;
			}
		}
	}
	if ((BM.FM80_online == false) && online_count++ > ONLINE_TIMEOUT) {
		online_count = 0;
		BM.FM80_online = false;
		BM.FM80_io = false;
		cc_mode = STATUS_LAST;
		state = state_watts;
		mx_code = 0x0;
		DataHandler();
	}
}

/*
 * format data to CSV format
 */
void bmc_logger(void)
{
	static uint8_t d_id = DC1_CMD;

	bmc_newtime = localtime((void *) &V.utc_ticks);
	snprintf((char*) buffer, 25, "%s", asctime(bmc_newtime)); // the log_buffer uses this string in LOG_VARS
	buffer[DTG_LEN] = 0; // remove newline
	switch (d_id) {
	case STX:
	case DC1_CMD:
		BMC4.d_id = d_id;
		snprintf((char*) log_buffer, MAX_B_BUF, log_format1, LOG_VARS1);
		d_id = DC2_CMD;
		break;
	case DC2_CMD:
		BMC4.d_id = d_id;
		snprintf((char*) log_buffer, MAX_B_BUF, log_format2, LOG_VARS2);
		d_id = DC_NEXT;
		break;
	case DC3_CMD:
		BMC4.d_id = d_id;
		snprintf((char*) log_buffer, MAX_B_BUF, log_format3, LOG_VARS3);
		d_id = DC4_CMD;
		break;
	case DC4_CMD:
		BMC4.d_id = d_id;
		snprintf((char*) log_buffer, MAX_B_BUF, log_format4, LOG_VARS4);
		d_id = DC1_CMD;
		break;
	default:
		d_id = DC1_CMD;
		BMC4.d_id = d_id;
		snprintf((char*) log_buffer, MAX_B_BUF, log_format1, LOG_VARS1);
		break;
	}

	BMC4.log_buffer = &log_buffer[0];
	BMC4.len = 512;
	BMC4.pos = 0;
	BMC4.bmc_flag = true;
	bmc_string_ready = true; // CHAR_GO_BYTES
}

/*
 * display  div 10 integer to fraction without FP
 * vw  volt_whole, volt_fract
 */
static void volt_f(const uint16_t voltage)
{
	volt_fract = (uint16_t) abs(voltage % 10);
	volt_whole = voltage / 10;
}

void state_init_cb(void)
{
	float Soc;
	static uint8_t off_delay = 0;

	mx_code = abuf[2]&0xf;
	if (mx_code == FM80_ID) {
		BM.FM80_online = true;
		off_delay = 0;
	} else {
		if (off_delay++ > 3) {
			BM.FM80_online = false;
			cc_mode = STATUS_LAST;
		}
	}
	state = state_status;
}

void state_batteryv_cb(void)
{
	volt_f((abuf[2] + (abuf[1] << 8)));
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x   DATA: Battery Voltage %d.%01dVDC\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], volt_whole, volt_fract);
#endif
	state = state_batterya;
}

void state_batterya_cb(void)
{
	volt_f((abuf[2] + (abuf[1] << 8)));
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x   DATA: Battery Amps %dADC\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], abuf[2] - 128);
#endif
	state = state_watts;
}

static void state_fwrev_cb(void)
{
	BM.fwrev[fw_state++] = abuf[2];
	if (!C.tm_ok) {
		state = state_misc;
	} else {
		C.tm_ok = false;
		state = state_time;
	}
}

static void state_time_cb(void)
{
#ifdef SDEBUG
	char s_buffer[22];
	snprintf(s_buffer, 21, "Time CSum %X        ", calc_checksum((uint8_t *) & cmd_time[1], 10));
	eaDogM_Scroll_String(s_buffer);
#endif
	state = state_date;
}

static void state_date_cb(void)
{
#ifdef SDEBUG
	char s_buffer[22];
	snprintf(s_buffer, 21, "Date CSum %X        ", calc_checksum((uint8_t *) & cmd_date[1], 10));
	eaDogM_Scroll_String(s_buffer);
#endif
	state = state_misc;
}

static void state_restart_cb(void)
{
#ifdef SDEBUG
	char s_buffer[22];
	snprintf(s_buffer, 21, "Date CSum %X        ", calc_checksum((uint8_t *) & cmd_date[1], 10));
	eaDogM_Scroll_String(s_buffer);
#endif
	state = state_init;
}

/*
 * testing online status while waiting for 10 second flag callback
 */
void state_misc_cb(void)
{
	if (mx_code == FM80_ID) { // only set FM80 offline here
	} else {
		BM.FM80_online = false;
		cc_mode = STATUS_LAST;
		state = state_init;
		return;
	}
	if (!BM.ten_sec_flag) {
		state = state_misc;
	} else {
		state = state_status;
	}
}

void state_mx_log_cb(void)
{
	BM.log.volts_peak = (int16_t) cbuf[5];
	BM.log.day = (int16_t) cbuf[14];
	BM.log.kilowatt_hours = (int16_t) (((uint16_t) (cbuf[3] & 0xF0) >> 4) | (uint16_t) (cbuf[4] << 4));
	BM.log.kilowatts_peak = (int16_t) (((uint16_t) (cbuf[13] & 0xFC) >> 2) | (uint16_t) (cbuf[12] << 6));
	BM.log.bat_max = (int16_t) (((uint16_t) (cbuf[2] & 0xFC) >> 2) | (uint16_t) ((cbuf[3] & 0x0F) << 6));
	BM.log.bat_min = (int16_t) (((uint16_t) (cbuf[10] & 0xC0) >> 6) | (uint16_t) ((cbuf[11] << 2) | ((cbuf[12] & 0x03) << 10)));
	BM.log.amps_peak = (int16_t) (cbuf[1] | ((cbuf[2] & 0x03) << 8));
	BM.log.amp_hours = (int16_t) (cbuf[9] | ((cbuf[10] & 0x3F) << 8));
	BM.log.absorb_time = (int16_t) (cbuf[6] | ((cbuf[7] & 0x0F) << 8));
	BM.log.float_time = (int16_t) (((cbuf[7] & 0xF0) >> 4) | (cbuf[8] << 4));

	cmd_mx_log[5] = BM.log.select;
	cmd_mx_log[7] = 0x16 + BM.log.select; // update the checksum

	state = state_mx_status;
}

void state_watts_cb(void)
{
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x   DATA: Panel Watts %iW\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], (abuf[2] + (abuf[1] << 8)));
#endif
	panel_watts = (abuf[2] + (abuf[1] << 8));
	if (BM.FM80_online) {
		state = state_mx_log; // only get log data once state_init_cb has run
	} else {
		state = state_mx_status;
	}
}

void state_mx_status_cb(void)
{
	volt_f((abuf[11] + (abuf[10] << 8))); // set battery voltage here in volt_whole and volt_frac
	vw = volt_whole;
	vf = volt_fract;
	volt_f((abuf[13] + (abuf[12] << 8))); // set panel voltage here in volt_whole and volt_frac
	pvw = volt_whole;
	pvf = volt_fract;
	if ((abuf[1] &0x0f) > 9) { // check for whole Amp
		abuf[2]++; // add extra Amp for fractional overflow.
		abuf[1] = (abuf[1]&0x0f) - 10;
	}
	if (BM.FM80_online) { // don't update when offline
		bat_amp_whole = abuf[3];
		bat_amp_panel = abuf[2];
		bat_amp_frac = abuf[1];
	}
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x  SDATA: FM80 Data mode %3x %3x %3x %3x %3x %3x %3x %3x %3x\r\n",
		rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], abuf[5], abuf[6], abuf[7], abuf[8], abuf[9], abuf[10], abuf[11], abuf[12], abuf[13]);
#endif

	if (BM.ten_sec_flag) {
		BM.ten_sec_flag = false;
		if (BM.FM80_online || BM.modbus_online) { // log for MX80 and EM540
			MM_ERROR_C;
			/*
			 * not used
			 * log CSV values to the comm ports for data storage and processing
			 */
			BM.run_time = lp_filter(BM.run_time, F_run, false); // smooth run-time
		}
	}
	state = state_fwrev;
}

void state_status_cb(void)
{
	static uint16_t day_clocks = 0;
	static uint8_t status_prev = STATUS_SLEEPING;

#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x STATUS: FM80 %s mode\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], state_name[abuf[2]]);
#endif

	/*
	 * check for the start and end of a solar production day
	 * sets update flag if day state changes and has day change charge controllers
	 * status in pv_prev variable
	 */

	/*
	 * once a day/night event has happened wait for a long while until the next change
	 * clear event counter timer 10s ticks
	 */
	if (BM.day_check++ > CHK_DAY_TIME) {
		BM.day_check = 0;
		BM.once = false;
	}

	if (FMxx_STATE != STATUS_SLEEPING) {
		if (++day_clocks > BAT_DAY_COUNT) {
			day_clocks = 0;
			if (!BM.once && (BM.pv_prev == STATUS_SLEEPING)) { // check sun on PV and trigger a daily energy update
				BM.day_check = 0;
				BM.pv_update = true;
				BM.pv_prev = FMxx_STATE;
				BM.once = true;
			}
			BM.pv_high = true;
		}
	} else {
		if (++day_clocks > BAT_NIGHT_COUNT) {
			day_clocks = 0;
			if (!BM.once && (BM.pv_prev != STATUS_SLEEPING)) { // check for night and update day totals
				BM.day_check = 0;
				BM.pv_update = true;
				BM.pv_prev = FMxx_STATE;
				BM.once = true;
			}
			BM.pv_high = false;
		}

	}
	if (BM.FM80_online) { // don't update when offline
		cc_mode = FMxx_STATE;
	} else {
		cc_mode = STATUS_LAST;
	}
	state = state_watts;
}

void state_panelv_cb(void)
{
#ifdef debug_data
	printf("%5d: %3x %3x %3x %3x %3x   DATA: Panel Voltage %iVDC\r\n", rx_count++, abuf[0], abuf[1], abuf[2], abuf[3], abuf[4], (abuf[2] + (abuf[1] << 8)));
#endif
	state = state_batteryv;
}
/**
 End of File
 */