
/* Parts of this code were modified from
 *  http://www.d.umn.edu/~cprince/PubRes/Hardware/SPI/
 * examples
 *
 * Fully interrupt driven SPI slave ADC for OPI
 *
 * Version
 *              0.03 beta version
 *
 * nsaspook@sma2.rain..com    Sept 2016
 */
/** \file slaveo.c
 * BMCboard SPI DAQ slave for the Orange PI
 */

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

#include <xc.h>
#include "slaveo.h"

void check_slaveo(void) /* SPI Slave error check */
{
	if (SPI2STATUSbits.TXWE) { // check for overruns/collisions
		spi_stat_ss.spi_error_count++;
	}
}

/*
 * setup the interrupt call backs and data structures
 */
void init_slaveo(void)
{
	uint8_t val = 0xff;
	SPI2_SetRxInterruptHandler(slaveo_rx_isr);
	TMR0_StartTimer();
	TMR0_SetInterruptHandler(slaveo_time_isr);
	SPI2_SetInterruptHandler(slaveo_spi_isr);
	SPI2CON0bits.EN = 0;
	while (!SPI2STATUSbits.RXRE) { // clear the FIFO of data
		val = SPI2RXB;
	}
	serial_buffer_ss.data[2] = val;
	SPI2STATUSbits.RXRE = 0;
	SPI2CON0bits.EN = 0;
	SPI2CON2bits.TXR = 1; // FIFO required for transmit
	ADC_SelectContext(CONTEXT_1);
}

void slaveo_rx_isr(void)
{
	uint8_t command, char_rxtmp, char_txtmp, cmd_dummy = CMD_DUMMY;
	/* we only get this when the master wants data, the slave never generates one */
	// SPI port #2 SLAVE receiver
	MLED_SetHigh();
	spi_stat_ss.slave_int_count++;
	MLED_SetLow();
	data_in2 = SPI2RXB;
	serial_buffer_ss.data[0] = data_in2;
	command = data_in2 & HI_NIBBLE;
	serial_buffer_ss.command = command;

	if (UART1_is_rx_ready()) { // we need to read the buffer in sync with the *_CHAR_* commands so it's polled
		MLED_SetLow();
		char_rxtmp = UART1_Read();
		serial_buffer_ss.data[1] = char_rxtmp;
		cmd_dummy |= UART_DUMMY_MASK; // We have real USART data waiting
		spi_comm_ss.CHAR_DATA = true;
	}

	if (command == CMD_CHAR_GO) {
		char_txtmp = (data_in2 & LO_NIBBLE); // read lower 4 bits
		//		serial_buffer_ss.tx_buffer = char_rxtmp;
		spi_stat_ss.char_count++;
	}

	if (command == CMD_CHAR_DATA) { // get upper 4 bits send bits and send the data
		if (UART1_is_tx_ready()) { // The USART send buffer is ready
			UART1_Write((uint8_t) ((uint8_t) ((uint8_t) data_in2 & (uint8_t) LO_NIBBLE) << (uint8_t) 4) | (uint8_t) char_txtmp);
		} else {
		}
		serial_buffer_ss.tx_buffer = cmd_dummy;
		cmd_dummy = CMD_DUMMY; // clear rx bit
		spi_comm_ss.CHAR_DATA = false;
		spi_comm_ss.REMOTE_LINK = true;
		/* reset link data timer if we are talking */
		TMR0_Reload();
	}

	if (command == CMD_ADC_GO) { // Found a GO for a conversion command
		spi_comm_ss.ADC_RUN = false;
		spi_stat_ss.adc_count++;
		channel = data_in2 & LO_NIBBLE;
		if (channel > AI_BUFFER_NUM) {
			channel = channel_ANA0; // invalid channel so set to analog 0
		}

		SPI2TXB = ((adc_buffer[channel] >> 8)&0x00ff);
		SPI2TXB = (adc_buffer[channel] &0x00ff);

		spi_comm_ss.REMOTE_LINK = true;
		while (!SPI2STATUSbits.RXRE) { // clear the FIFO of data
			data_in2 = SPI2RXB;
		}
		TMR0_Reload();
	}

	if (data_in2 == CMD_ADC_DATA) {
		spi_stat_ss.slave_tx_count++;
		spi_stat_ss.last_slave_int_count = spi_stat_ss.slave_int_count;
	}

	if (command == CMD_CHAR_RX) {
		serial_buffer_ss.tx_buffer = char_rxtmp;
		cmd_dummy = CMD_DUMMY; // clear rx bit
	}

}

void slaveo_spi_isr(void)
{
	MLED_SetLow();
	spi_stat_ss.spi_error_count++;
	SPI2INTF = 0;
}

void slaveo_time_isr(void)
{
	if (SPI2STATUSbits.TXWE || SPI2STATUSbits.RXRE) { // check for overruns/collisions
		spi_stat_ss.spi_error_count++;
	}
	MLED_SetLow();
	DLED_SetLow();
}