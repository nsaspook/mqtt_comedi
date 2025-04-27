
/* Parts of this code were modified from
 *  http://www.d.umn.edu/~cprince/PubRes/Hardware/SPI/
 * examples
 *
 * Fully interrupt driven SPI slave ADC for OPI
 * 8722
 * Port E is the main led diag port
 * PORT H is the LCD port
 * 25k22
 * Pins C0,C1 are the diag LED pins.
 * SPI 2 has been config'd as the slave with chip select.
 * DIP8 Pins for MCP3002 header
 * Pin 21   RB0	SPI Chip-Select	Pin 1
 * Pin 22   RB1	SPI Clock	Pin 7
 * Pin 23   RB2	SPI Data In	Pin 5
 * Pin 24   RB3	SPI Data Out	Pin 6
 * Pin 8    Vss			Pin 4
 * Pin 20   Vdd			Pin 8
 * Pin 2    RA0	ANA0		Pin 2
 * Pin 3    RA1	ANA1		Pin 3
 * The I/O and clock pins IDC connector pins
 * have been interconnected in the standard way for a PIC18F8722 chip EET Board
 *
 * Version
 *              1.0 stable version for daq_gert P25K22 4_TAD
 *		0.91 update exchange protocol
 *		0.9 add 45K80 commands and ports
 *		0.8 Add zero command for cleaner transfers and allow for no LCD code	
 *		0.7 minor software cleanups.
 *		0.06 P25K22 Set PIC speed to 64mhz and use have ADC use FOSC_64,12_TAD
 *		P8722 have ADC use FOSC_32,12_TAD
 *		0.05 Fixed the P25K22 version to work correctly.
 *		0.04 The testing hardware is mainly a pic18f8722 with a
 *		LCD display and PORTE bit leds.
 *		define the CPU type below.
 *
 *		The WatchDog and timer0 are used to check link status
 *		and to reset the chip if hung or confused.
 *
 * nsaspook@sma2.rain..com    Sept 2016
 */


#define P25K22

/*
 * bit 7 high for commands sent from the MASTER
 * bit 6 0 send lower or 1 send upper byte ADC result first
 * bits 3..0 port address
 *
 * bit 7 low  for config data sent in CMD_DUMMY per uC type
 * bits 6 config bit code always 1
 * bit	5 0=ADC ref VDD, 1=ADC rec FVR=2.048
 * bit  4 0=10bit adc, 1=12bit adc
 * bits 3..0 number of ADC channels
 * 
 */

#define	TIMEROFFSET	26474           // timer0 16bit counter value for 1 second to overflow
#define SLAVE_ACTIVE	10		// Activity counter level

/* PIC Slave commands */
#define CMD_ZERO        0b00000000
#define CMD_ADC_GO	0b10000000
#define CMD_PORT_GO	0b10100000	// send data LO_NIBBLE to port buffer
#define CMD_CHAR_GO	0b10110000	// send data LO_NIBBLE to TX buffer
#define CMD_ADC_DATA	0b11000000
#define CMD_PORT_DATA	0b11010000	// send data HI_NIBBLE to port buffer ->PORT and return input PORT data in received SPI data byte
#define CMD_CHAR_DATA	0b11100000	// send data HI_NIBBLE to TX buffer and return RX buffer in received SPI data byte
#define CMD_XXXX	0b11110000	//
#define CMD_CHAR_RX	0b00010000	// Get RX buffer
#define CMD_DUMMY_CFG	0b01000000	// stuff config data in SPI buffer
#define CMD_DEAD        0b11111111      // This is usually a bad response

#define CMD_DUMMY	0b01101110	/* 14 channels 2.048 but only 13 are ADC */
#define NUM_AI_CHAN     14

#define	HI_NIBBLE	0xf0
#define	LO_NIBBLE	0x0f
#define	ADC_SWAP_MASK	0b01000000
#define UART_DUMMY_MASK	0b01000000


/* DIO defines */
#define LOW		0			// digital output state levels, sink
#define	HIGH		1			// digital output state levels, source
#define	SON		LOW       		//
#define SOFF		HIGH			//
#define	S_ON            LOW       		// low select/on for chip/led
#define S_OFF           HIGH			// high deselect/off chip/led
#define	R_ON            HIGH       		// control relay states, relay is on when output gate is high, uln2803,omron relays need the CPU at 5.5vdc to drive
#define R_OFF           LOW			// control relay states
#define R_ALL_OFF       0x00
#define R_ALL_ON	0xff
#define NO		LOW
#define YES		HIGH

#include "slaveo.h"

volatile struct spi_link_type_ss spi_comm_ss = {false, false, false, false, false, false, false};
volatile struct spi_stat_type_ss spi_stat_ss = {0}, report_stat_ss = {0};

static volatile uint8_t data_in2, adc_buffer_ptr = 0, adc_channel = 0;

static volatile uint16_t adc_buffer[64] = {0}, adc_data_in = 0;
static uint8_t channel = 0, link, upper, command, char_rxtmp, char_txtmp, cmd_dummy = CMD_DUMMY;

void slaveo_adc_isr(void) {

    PIR1bits.ADIF = LOW;
    spi_stat_ss.adc_count++; // just keep count
    adc_buffer[channel] = (uint16_t) ADRES; // data is ready but must be written to the SPI buffer before a master command is received 
    if (upper) { /* same as CMD_ZERO */
        SPI2TXB = ADRESH;
    } else {
        SPI2TXB = ADRESL; // stuff with lower 8 bits
    }
    spi_comm_ss.ADC_DATA = true; // so the transmit buffer will not be overwritten
}

void check_slaveo(void) /* SPI Master/Slave loopback */ {
    if (SPI2STATUSbits.TXWE || SPI2STATUSbits.RXRE) { // check for overruns/collisions
        spi_stat_ss.adc_error_count++;
    }
}

/*
 * setup the interrupt call backs and data structures
 */
void init_slaveo(void) {
    SPI2_SetInterruptHandler(slaveo_spi_isr);
    SPI2_SetRxInterruptHandler(slaveo_rx_isr);
    SPI2_SetTxInterruptHandler(slaveo_tx_isr);
    TMR0_StartTimer();
    TMR0_SetInterruptHandler(slaveo_time_isr);
}

void slaveo_rx_isr(void) {

    /* we only get this when the master  wants data, the slave never generates one */
    { // SPI port #2 SLAVE receiver
        spi_stat_ss.slave_int_count++;
        data_in2 = SPI2RXB;
        command = data_in2 & HI_NIBBLE;

        if (UART1_is_rx_ready()) { // we need to read the buffer in sync with the *_CHAR_* commands so it's polled
            char_rxtmp = UART1_Read();
            cmd_dummy |= UART_DUMMY_MASK; // We have real USART data waiting
            spi_comm_ss.CHAR_DATA = true;
        }

        if (command == CMD_CHAR_GO) {
            char_txtmp = (data_in2 & LO_NIBBLE); // read lower 4 bits
            SPI2TXB = char_rxtmp; // send current receive data to master
            spi_stat_ss.char_count++;
        }

        if (command == CMD_CHAR_DATA) { // get upper 4 bits send bits and send the data
            if (UART1_is_tx_ready()) { // The USART send buffer is ready
                UART1_Write(((data_in2 & LO_NIBBLE) << 4) | char_txtmp);
            } else {
            }
            SPI2TXB = cmd_dummy; // send rx status first, the next SPI transfer will contain it.
            cmd_dummy = CMD_DUMMY; // clear rx bit
            spi_comm_ss.CHAR_DATA = false;
            spi_comm_ss.REMOTE_LINK = true;
            /* reset link data timer if we are talking */
            TMR0_Reload();
        }

        if (command == CMD_ADC_GO) { // Found a GO for a conversion command
            spi_stat_ss.adc_count++;
            spi_comm_ss.ADC_DATA = false;
            if (data_in2 & ADC_SWAP_MASK) {
                upper = true;
            } else {
                upper = false;
            }
            channel = data_in2 & LO_NIBBLE;
            if (channel >= 5) channel += 6; // skip missing channels
            if (channel == 12 || channel > 19) channel = 0; // invalid so set to 0

            if (!ADCON0bits.GO) {
                ADCON0 = ((channel << 2) & 0b00111100) | (ADCON0 & 0b11000011);
                adc_buffer[channel] = 0xffff; // fill with bits
                ADCON0bits.GO = HIGH; // start a conversion
            } else {
                ADCON0bits.GO = LOW; // stop a conversion
                SPI2TXB = CMD_DUMMY; // Tell master  we are here
                spi_stat_ss.adc_error_count++;
            }
            spi_comm_ss.REMOTE_LINK = true;
            link = true;
            TMR0_Reload();
        }
        if (data_in2 == CMD_DUMMY_CFG) {
            SPI2TXB = CMD_DUMMY; // Tell master  we are here
        }

        if ((data_in2 == CMD_ZERO) && spi_comm_ss.ADC_DATA) { // don't sent unless we have valid data
            spi_stat_ss.last_slave_int_count = spi_stat_ss.slave_int_count;
            if (upper) {
                SPI2TXB = ADRESH;
            } else {
                SPI2TXB = ADRESL; // stuff with lower 8 bits
            }
        }
        if (data_in2 == CMD_ADC_DATA) {
            if (spi_comm_ss.ADC_DATA) {
                if (upper) {
                    SPI2TXB = ADRESL; // stuff with lower 8 bits
                } else {
                    SPI2TXB = ADRESH;
                }
                spi_stat_ss.last_slave_int_count = spi_stat_ss.slave_int_count;
            } else {
                SPI2TXB = CMD_DUMMY;
            }
        }
        if (command == CMD_CHAR_RX) {
            SPI2TXB = char_rxtmp; // Send current RX buffer contents
            cmd_dummy = CMD_DUMMY; // clear rx bit
        }
    }
}

void slaveo_tx_isr(void) {

}

void slaveo_spi_isr(void) {

}

void slaveo_time_isr(void) {
    if (SPI2STATUSbits.TXWE || SPI2STATUSbits.RXRE) { // check for overruns/collisions
        spi_stat_ss.adc_error_count++;
    }
    DLED_SetLow();
}