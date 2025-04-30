/*
 * SECS-II RS232 link tester
 */

/*
 * UART1 HOST RS-232 comms, ANALOG header, TX-PIN 7 MB_TX, RX-PIN 6 MB_RX
 * UART2 Equipment Testing RS-232 comms, TTL serial HEADER, TX-PIN 2 PC_TX, RX-PIN 3 PC_RX
 */

// PIC18F47Q84 Configuration Bit Settings
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
#pragma config IVT1WAY = ON     // IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared and set only once)
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
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/uart1.h"
#include "eadog.h"
#include "gemsecs.h"
#include "timers.h"
#include "mconfig.h"
#include "mydisplay.h"
#include "rs232.h"
#include "slaveo.h"

#ifdef TRACE
#define M_TRACE	do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define I_TRACE	RB7_Toggle()
#else
#define M_TRACE	""
#define I_TRACE	""
#endif

extern struct spi_link_type spi_link;
const char *build_date = __DATE__, *build_time = __TIME__;

const char * GEM_TEXT [] = {
	"DISABLE",
	"COMM   ",
	"OFFLINE",
	"ONLINE ",
	"REMOTE ",
	"ERROR  "
};

V_data V = {
	.error = LINK_ERROR_NONE,
	.abort = LINK_ERROR_NONE,
	.msg_error = MSG_ERROR_RESET,
	.uart = HOST_UART,
	.g_state = GEM_STATE_DISABLE,
	.e_types = GEM_GENERIC,
	.ticker = TICKER_ZERO,
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
	.failed_receive = RECV_ERROR_NONE,
	.failed_send = SEND_ERROR_NONE,
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
};

B_type B = {
	.one_sec_flag = false,
	.display_update = false,
	.dim_delay = DIM_DELAY,
};

volatile struct spi_link_type_ss spi_comm_ss = {false, false, false, false, false, false, false, false};
volatile struct spi_stat_type_ss spi_stat_ss = {0}, report_stat_ss = {0};
volatile struct serial_buffer_type_ss serial_buffer_ss = {
	.tx_buffer = 0x81,
	.data[0] = 0x57,
};

volatile uint8_t data_in2, adc_buffer_ptr = 0, adc_channel = 0, channel = 0, upper;
volatile uint16_t adc_buffer[64] = {0}, adc_data_in = 0;

volatile uint16_t tickCount[TMR_COUNT] = {0};
volatile uint8_t mode_sw = false, faker;
void onesec_io(void);
int8_t test_slave(void);

/** \file main.c
 * Lets get going with the code.
 * Main application
 */
void main(void)
{
	UI_STATES mode; /* link configuration host/equipment/etc ... */
	char * s, * speed_text;
	uint8_t temp_lock = false;
	static uint8_t looper = 0;

	SPI2STATUSbits.SPI2CLRBF;

	// Initialize the device
	SYSTEM_Initialize();


	// Enable high priority global interrupts
	INTERRUPT_GlobalInterruptHighEnable();

	// Enable low priority global interrupts.
	INTERRUPT_GlobalInterruptLowEnable();

	mconfig_init(); // zero the entire text buffer

	V.ui_state = UI_STATE_INIT;
	mode = UI_STATE_HOST;

	TMR2_StartTimer();
	TMR5_SetInterruptHandler(onesec_io);
	TMR5_StartTimer();
	TMR6_StartTimer();
	ADC_SelectContext(CONTEXT_1);

	/** Speed locking setup code.
	 * Use a few EEPROM bytes to cycle or lock the serial port baud rate
	 * during a power-up.
	 * 9600 and 19200 are the normal speeds for SECS-I serial communications
	 */
	V.speed_spin = DATAEE_ReadByte(UART_SPEED_LOCK_EADR);
	V.uart_speed_fast = DATAEE_ReadByte(UART_SPEED_EADR);
	DATAEE_WriteByte(UART_SPEED_LOCK_EADR, temp_lock);
	DATAEE_WriteByte(UART_SPEED_EADR, V.uart_speed_fast + 1);

	if (V.uart_speed_fast % 2 == 0) { // Even/Odd selection for just two speeds
		speed_text = "Locked 9600bps";
	} else {
		speed_text = "Locked 19200bps";
	}
	// as soon as you see all LEDS ON, power down, quickly POWER CYCLE to LOCK baud rate
	MLED_SetHigh();
	RLED_SetHigh();
	DLED_SetHigh();
	WaitMs(TDELAY);
	RLED_SetLow(); // start complete power-up serial speed setups, LEDS OFF
	MLED_SetLow();
	DLED_SetLow();
	temp_lock = true;
	if (V.speed_spin) { // update the speed lock status byte
		DATAEE_WriteByte(UART_SPEED_LOCK_EADR, temp_lock);
	}
	DATAEE_WriteByte(UART_SPEED_EADR, V.uart_speed_fast); // update the speed setting byte

	if (V.speed_spin) { // serial speed with alternate with every power cycle
		/*
		 * get saved state of serial speed flag
		 */
		V.uart_speed_fast = DATAEE_ReadByte(UART_SPEED_EADR);
		if (V.uart_speed_fast == 0xFF) { // programmer fill number
			V.uart_speed_fast = 0;
			DATAEE_WriteByte(UART_SPEED_EADR, V.uart_speed_fast); // start at zero
		}
		if (V.uart_speed_fast % 2 == 0) {
			UART2_Initialize19200();
			UART1_Initialize19200();
			speed_text = "19200bps";
		} else {
			UART2_Initialize();
			UART1_Initialize();
			speed_text = "9600bps";
		}
		/*
		 * ALternate the speed setting with each restart
		 */
		DATAEE_WriteByte(UART_SPEED_EADR, ++V.uart_speed_fast);
		DATAEE_WriteByte(UART_SPEED_LOCK_EADR, V.speed_spin);
	} else { // serial port speed is locked
		V.uart_speed_fast = DATAEE_ReadByte(UART_SPEED_EADR); // just read and set the speed setting
		if (V.uart_speed_fast % 2 == 0) {
			UART2_Initialize();
			UART1_Initialize();
		} else {
			UART2_Initialize19200();
			UART1_Initialize19200();
		}
	}

	init_slaveo();
	/*
	 * master processing I/O loop
	 */
	while (true) {
		M_TRACE;
		if (!faker++) {
		}

		/*
		 * check and parse logging configuration commands on UART3
		 */
		logging_cmds();

		/*
		 * protocol state machine for HOST emulation
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

			V.ui_state = mode;
			V.s_state = SEQ_STATE_INIT;
			srand(1957);
			set_vterm(V.vterm); // set to buffer 0
			snprintf(V.info, MAX_INFO, " Terminal Info               ");
			snprintf(get_vterm_ptr(0, MAIN_VTERM), MAX_TEXT, " OPI DAQ         %u   ", V.uart_speed_fast & 0x01);
			snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, " Version %s           ", VER);
			snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, " NSASPOOK             ");
			snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, " %s                   ", (char *) build_date);
			snprintf(get_vterm_ptr(0, INFO_VTERM), MAX_TEXT, " INFO                 ");
			snprintf(get_vterm_ptr(1, INFO_VTERM), MAX_TEXT, " Version %s           ", VER);
			snprintf(get_vterm_ptr(2, INFO_VTERM), MAX_TEXT, " VTERM INFO           ");
			snprintf(get_vterm_ptr(3, INFO_VTERM), MAX_TEXT, " %s                   ", (char *) build_date);
			snprintf(get_vterm_ptr(0, HELP_VTERM), MAX_TEXT, " HELP Build %s        ", VER);
			snprintf(get_vterm_ptr(1, HELP_VTERM), MAX_TEXT, " Version %s           ", VER);
			snprintf(get_vterm_ptr(2, HELP_VTERM), MAX_TEXT, " VTERM HELP           ");
			snprintf(get_vterm_ptr(3, HELP_VTERM), MAX_TEXT, " %s                   ", (char *) build_date);
			snprintf(get_vterm_ptr(0, DBUG_VTERM), MAX_TEXT, " DEBUG                ");
			snprintf(get_vterm_ptr(1, DBUG_VTERM), MAX_TEXT, " Version %s           ", VER);
			snprintf(get_vterm_ptr(2, DBUG_VTERM), MAX_TEXT, " VTERM DEBUG          ");
			snprintf(get_vterm_ptr(3, DBUG_VTERM), MAX_TEXT, " %s                   ", (char *) build_date);
			refresh_lcd();
			WaitMs(TDELAY);
			StartTimer(TMR_DISPLAY, DDELAY);
			StartTimer(TMR_SEQ, 10000);
			StartTimer(TMR_INFO, TDELAY);
			StartTimer(TMR_FLIPPER, DFLIP);
			StartTimer(TMR_HELPDIS, TDELAY);
			StartTimer(TMR_SEQ, SEQDELAY);
			StartTimer(TMR_HELP, TDELAY);
			break;
		case UI_STATE_HOST: // equipment starts communications to host
			switch (V.s_state) {
			case SEQ_STATE_INIT:
				V.r_l_state = LINK_STATE_IDLE;
				V.t_l_state = LINK_STATE_IDLE;
				V.s_state = SEQ_STATE_RX;
				if ((V.error == LINK_ERROR_NONE) && (V.abort == LINK_ERROR_NONE)) {
					if (V.debug) {
					} else {
					}
				}
				break;
			case SEQ_STATE_RX:
				/*
				 * receive message from equipment
				 */
				V.s_state = SEQ_STATE_TRIGGER;
				if (V.r_l_state == LINK_STATE_ERROR)
					V.s_state = SEQ_STATE_ERROR;
				break;
			case SEQ_STATE_TX:
				/*
				 * send response message to equipment
				 */
				V.s_state = SEQ_STATE_RX;
				if (V.t_l_state == LINK_STATE_ERROR)
					V.s_state = SEQ_STATE_ERROR;
				break;
			case SEQ_STATE_TRIGGER:
				set_display_info(DIS_STR);
				s = get_vterm_ptr(0, MAIN_VTERM);
				if (V.queue) {
					V.r_l_state = LINK_STATE_IDLE;
					V.t_l_state = LINK_STATE_IDLE;
					V.s_state = SEQ_STATE_TX;
				} else {
					V.s_state = SEQ_STATE_DONE;
				}

				s[MAX_LINE] = 0;
				s[SPIN_CHAR] = spinners(3, false);
				break;
			case SEQ_STATE_DONE:
				V.s_state = SEQ_STATE_INIT;
				if (++looper == 0) {
					//test_slave();
				}
				break;
			case SEQ_STATE_ERROR:
			default:
				V.s_state = SEQ_STATE_INIT;
				refresh_lcd();
				WaitMs(TDELAY);
				break;
			}
			if ((V.error == LINK_ERROR_NONE) && (V.abort == LINK_ERROR_NONE)) {
				if (TimerDone(TMR_DISPLAY)) { // limit update rate
				}
			}
			break;
		case UI_STATE_LOG: // monitor
			switch (V.s_state) {
			case SEQ_STATE_INIT:
				V.m_l_state = LINK_STATE_IDLE;
				V.s_state = SEQ_STATE_RX;
				break;
			case SEQ_STATE_RX:
				/*
				 * receive rx and tx messages from comm link
				 */
				V.s_state = SEQ_STATE_TRIGGER;
				if (V.m_l_state == LINK_STATE_ERROR)
					V.s_state = SEQ_STATE_ERROR;
				break;
			case SEQ_STATE_TRIGGER:
				V.s_state = SEQ_STATE_DONE;
				break;
			case SEQ_STATE_DONE:
			case SEQ_STATE_ERROR:
			default:
				V.s_state = SEQ_STATE_INIT;
				break;
			}
			snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, "                      ");
			break;
		case UI_STATE_ERROR:
		default:
			V.ui_state = UI_STATE_INIT;
			break;
		}
		if (V.ticks) {
			if (V.failed_receive != RECV_ERROR_NONE) {
				if (V.error == LINK_ERROR_CHECKSUM) {
				}
			} else {
			}
			if (V.failed_send != SEND_ERROR_NONE) {
				if (V.error == LINK_ERROR_CHECKSUM) {
				}
			} else {
			}
		}

		if (mode != UI_STATE_LOG) {
			if (TimerDone(TMR_DISPLAY)) { // limit update rate
				static uint8_t switcher = INFO_VTERM;
				if (TimerDone(TMR_HELPDIS)) {
					set_display_info(DIS_STR);
				}
				snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, "%lu %lu %lu %lu  0x%.2X 0x%.2X                  ", spi_stat_ss.adc_count, spi_stat_ss.comm_count, spi_stat_ss.slave_int_count, spi_stat_ss.idle_count,
					serial_buffer_ss.data[0], serial_buffer_ss.data[2]);
				snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, "%d %d %d %d %d %d %d %d %d             ", adc_buffer[channel], spi_comm_ss.ADC_DATA, spi_comm_ss.CHAR_DATA, spi_comm_ss.PORT_DATA, spi_comm_ss.LOW_BITS,
					spi_comm_ss.REMOTE_DATA_DONE, spi_comm_ss.REMOTE_LINK, spi_comm_ss.SPI_DATA, spi_comm_ss.ADC_RUN);
				snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, "RS232 Volts %d                  ", V.vterm_switch);

				PIE1bits.ADIE = 0; // lock ADC interrupts off
				if (!spi_comm_ss.ADC_RUN) { // check if we have a ADC conversion in progress
					ADC_DischargeSampleCapacitor();
					ADC_StartConversion(channel_ANA1);
					while (!ADC_IsConversionDone()) {
					};
					if (ADC_IsConversionDone()) {
						V.v_tx_line = ADC_GetConversionResult();
					};
					ADC_DischargeSampleCapacitor();
					ADC_StartConversion(channel_ANA2);
					while (!ADC_IsConversionDone()) {
					};
					if (ADC_IsConversionDone()) {
						V.v_rx_line = ADC_GetConversionResult();
					};
					PIR1bits.ADIF = LOW;
					// convert ADC values to char for display
					update_rs232_line_status();
				}

				PIE1bits.ADIE = 1; // allow ADC interrupts

				StartTimer(TMR_DISPLAY, DDELAY);
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
				snprintf(get_vterm_ptr(0, INFO_VTERM), MAX_TEXT, "RS232 RX %3dV:%c                       ", V.rx_volts, V.rx_rs232);
				snprintf(get_vterm_ptr(1, INFO_VTERM), MAX_TEXT, "RS232 TX %3dV:%c                       ", V.tx_volts, V.tx_rs232);
				snprintf(get_vterm_ptr(2, INFO_VTERM), MAX_TEXT, "                                       ");
				snprintf(get_vterm_ptr(3, INFO_VTERM), MAX_TEXT, "                                       ");
				snprintf(get_vterm_ptr(0, DBUG_VTERM), MAX_TEXT, "                                       ");
				snprintf(get_vterm_ptr(1, DBUG_VTERM), MAX_TEXT, "                                       ");
				snprintf(get_vterm_ptr(2, DBUG_VTERM), MAX_TEXT, "                                       ");
				snprintf(get_vterm_ptr(3, DBUG_VTERM), MAX_TEXT, "                                       ");

				/*
				 * don't default update the LCD when displaying HELP text
				 */
				if (!V.set_sequ) {
					refresh_lcd();
					test_slave();
				}
			}
		}

		/*
		 * show help messages if flag is set for timer duration
		 */
		if (V.set_sequ) {
			if (TimerDone(TMR_HELP)) {
				V.set_sequ = false;
				set_vterm(V.vterm);
				refresh_lcd();
			} else {
				set_vterm(HELP_VTERM);
				refresh_lcd();
			}
		}

		if (V.help && TimerDone(TMR_SEQ)) {
			StartTimer(TMR_SEQ, SEQDELAY);
			StartTimer(TMR_HELP, TDELAY);
			V.set_sequ = true;
			check_help(false);
		}
		check_slaveo();
		M_TRACE;
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
	B.one_sec_flag = true;
	V.utc_ticks++;
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

int8_t test_slave(void)
{
	uint8_t ret = 0;

	DLED_SetHigh();
	wait_lcd_done();
	SPI2CON0bits.EN = 1;
	CS_SetHigh();
	SPI2_ReadByte();
	SPI2_ReadByte();
	send_spi2_data_dma(CMD_ADC_GO | ADC_SWAP_MASK | 4);
	//send_spi2_data_dma(CMD_ADC_GO | 4);
	wait_lcd_done();
	ret = SPI1_ReadByte();

	wait_lcd_done();
	SPI2CON0bits.EN = 1;
	CS_SetHigh();
	SPI2_ReadByte();
	SPI2_ReadByte();
	send_spi2_data_dma(CMD_ADC_DATA);
	wait_lcd_done();
	ret = SPI1_ReadByte();


	wait_lcd_done();
	SPI2CON0bits.EN = 1;
	CS_SetHigh();
	SPI2_ReadByte();
	SPI2_ReadByte();
	send_spi2_data_dma(CMD_ADC_DATA);
	wait_lcd_done();
	ret = SPI1_ReadByte();


	wait_lcd_done();
	SPI2CON0bits.EN = 1;
	CS_SetHigh();
	SPI2_ReadByte();
	SPI2_ReadByte();
	send_spi2_data_dma(CMD_ZERO);
	wait_lcd_done();
	ret = SPI1_ReadByte();
	serial_buffer_ss.data[2] = ret;
	SPI2CON0bits.EN = 0;

	return(int8_t) ret;
}


/**
 End of File
 */