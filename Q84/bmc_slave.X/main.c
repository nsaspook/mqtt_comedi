/*
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
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/uart1.h"
#include "eadog.h"
#include "timers.h"
#include "mconfig.h"
#include "mydisplay.h"
#include "rs232.h"
#include "slaveo.h"
#include "bmcdio.h"

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

const char * BMC_TEXT [] = {
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
	.g_state = BMC_STATE_DISABLE,
	.e_types = BMC_GENERIC,
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
	.bmc_ao = 0,
	.di_fail = false,
	.do_fail = false,
};

B_type B = {
	.one_sec_flag = false,
	.display_update = false,
	.dim_delay = DIM_DELAY,
};

volatile struct spi_link_type_ss spi_comm_ss = {false, false, false, false, false, false, false, false};
volatile struct spi_stat_type_ss spi_stat_ss = {
	.raw_index = 0,
	.daq_conf = 0,
};
volatile struct serial_buffer_type_ss serial_buffer_ss = {
	.data[BMC_CMD] = CHECKBYTE,
	.raw_index = BMC_CMD,
	.r_string_index = 0,
	.r_string_chan = 3,
};

volatile uint8_t data_in2, adc_buffer_ptr = 0, adc_channel = 0, channel = 0, upper;
volatile uint16_t adc_buffer[AI_BUFFER_NUM] = {0}, adc_data_in = 0, out_buf = 0;

volatile uint16_t tickCount[TMR_COUNT] = {0};
volatile uint8_t mode_sw = false, faker;
void onesec_io(void);
void test_slave(void);
void SetBMCPriority(void);

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

	SetBMCPriority(); // ISR Priority > Peripheral Priority > Main Priority

	mconfig_init(); // zero the entire text buffer

	V.ui_state = UI_STATE_INIT;

	TMR2_StartTimer();
	TMR5_SetInterruptHandler(onesec_io);
	TMR5_StartTimer();
	TMR6_StartTimer();
	TMR0_SetInterruptHandler(test_slave);
	TMR0_StartTimer();

#ifdef FLIP_SERIAL
	/** Speed locking setup code.
	 * Use a few EEPROM bytes to cycle or lock the serial port baud rate
	 * during a power-up.
	 * 9600 and 115200 are the normal speeds for serial communications
	 */
	V.speed_spin = DATAEE_ReadByte(UART_SPEED_LOCK_EADR);
	V.uart_speed_fast = DATAEE_ReadByte(UART_SPEED_EADR);
	DATAEE_WriteByte(UART_SPEED_LOCK_EADR, temp_lock);
	DATAEE_WriteByte(UART_SPEED_EADR, V.uart_speed_fast + 1);

	if (V.uart_speed_fast % 2 == 0) { // Even/Odd selection for just two speeds
		speed_text = "Locked 9600bps";
	} else {
		speed_text = "Locked 115200bps";
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
			UART2_Initialize115200();
			UART1_Initialize115200();
			speed_text = "115200bps";
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
			UART2_Initialize115200();
			UART1_Initialize115200();
		}
	}
#else
	speed_text = "Locked 115200bps";
	UART2_Initialize115200();
	UART1_Initialize115200();
	WaitMs(SDELAY);
	RLED_SetLow(); // start complete power-up serial speed setups, LEDS OFF
	MLED_SetLow();
	DLED_SetLow();
#endif

	/*
	 * master processing I/O loop
	 */
	while (true) {
		/*
		 * check and parse logging configuration commands
		 * not used
		 */
		logging_cmds();

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
			};
			SPI_MC33996();
			if (!mc33996_init()) {
				V.do_fail = true;
				failure = true;
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
			snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, " RUN Static Display             ");
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
			snprintf(get_vterm_ptr(0, DBUG_VTERM), MAX_TEXT, " DEBUG                ");
			snprintf(get_vterm_ptr(1, DBUG_VTERM), MAX_TEXT, " Version %s           ", VER);
			snprintf(get_vterm_ptr(2, DBUG_VTERM), MAX_TEXT, " VTERM DEBUG          ");
			snprintf(get_vterm_ptr(3, DBUG_VTERM), MAX_TEXT, " %s                   ", (char *) build_date);
			refresh_lcd();
			WaitMs(TDELAY);
			if (failure) {
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

			/*
			 * send ascii data to CLCD
			 */
			if (r_string_ready) {
				char *strPtr = get_vterm_ptr(serial_buffer_ss.r_string_chan, MAIN_VTERM);
				static uint8_t upd = 0;

				set_vterm(MAIN_VTERM);
				snprintf(strPtr, MAX_TEXT, "%.2X %s", upd++, serial_buffer_ss.r_string);
				refresh_lcd();
				serial_buffer_ss.r_string_index = 0;
				r_string_ready = false;
			}
#ifdef DIS_DEBUG

#ifdef AIO_TEST
			snprintf(get_vterm_ptr(0, MAIN_VTERM), MAX_TEXT, "%u,%u,%u,%u               ",
				adc_buffer[channel_ANA0], adc_buffer[channel_ANA1], adc_buffer[channel_ANA2], adc_buffer[channel_ANA4]);

			snprintf(get_vterm_ptr(1, MAIN_VTERM), MAX_TEXT, "%u,%u,%u,%u                ",
				adc_buffer[channel_ANA5], adc_buffer[channel_ANC6], adc_buffer[channel_ANC7], adc_buffer[channel_AND5]);

			snprintf(get_vterm_ptr(2, MAIN_VTERM), MAX_TEXT, "%u,%u config 0X%.2X                     ",
				adc_buffer[channel_VSS], adc_buffer[channel_Temp], spi_stat_ss.daq_conf);

			snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, "%2u,%2u,%2u                ",
				adc_buffer[channel_DAC1], adc_buffer[channel_FVR_Buffer1], adc_buffer[channel_FVR_Buffer2]);
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
			snprintf(get_vterm_ptr(3, MAIN_VTERM), MAX_TEXT, "vc%lu fv%ld fc%ld rs%lX                   ",
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
			snprintf(get_vterm_ptr(2, INFO_VTERM), MAX_TEXT, "A1 0x%.2x, A2 0x%.2x                   ", V.v_tx_line, V.v_rx_line);
			snprintf(get_vterm_ptr(3, INFO_VTERM), MAX_TEXT, "B0 0x%.2X, B1 0x%.2X %x                  ", serial_buffer_ss.data[0], serial_buffer_ss.data[1], b_read);
			snprintf(get_vterm_ptr(0, DBUG_VTERM), MAX_TEXT, "                                       ");
			snprintf(get_vterm_ptr(1, DBUG_VTERM), MAX_TEXT, "                                       ");
			snprintf(get_vterm_ptr(2, DBUG_VTERM), MAX_TEXT, "A1 0x%.2x, A2 0x%.2x                   ", V.v_tx_line, V.v_rx_line);
			snprintf(get_vterm_ptr(3, DBUG_VTERM), MAX_TEXT, "0x%.2lx 0x%.2lx %d %d %d %x                 ",
				spi_stat_ss.spi_error_count, spi_stat_ss.txuf_bit, spi_comm_ss.CHAR_DATA, spi_comm_ss.PORT_DATA, spi_comm_ss.REMOTE_LINK, b_read);
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

/**
 End of File
 */