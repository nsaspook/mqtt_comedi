
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
#include <pic18f47q84.h>

static void spi2_tx_wait(void);
static void spi2_tx_busy(void);
static void spi2_pace_write(const uint8_t);

void check_slaveo(void) /* SPI Slave error check */
{
	if (SPI2STATUSbits.TXWE) { // check for overruns/collisions
		//		spi_stat_ss.spi_error_count++;
	}
}

/*
 * setup the interrupt call backs and data structures
 * For SPI2 using MODE 0, ONLY USE MODE 0 with Orange PI SPI links
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
	bool zombie = true;
	/* we only get this when the master wants data, the slave never generates one */
	// SPI port #2 SLAVE receiver
	if (SPI2INTFbits.RXOIF) {
		SPI2INTFbits.RXOIF = 0;
		spi_stat_ss.rxof_bit++;
	}
	if (SPI2INTFbits.TXUIF) {
		SPI2INTFbits.TXUIF = 0;
	}
	MLED_SetHigh();
	spi_stat_ss.slave_int_count++;
	MLED_SetLow();

	if (SPI2STATUSbits.SPI2RXRE) {
	} else {
		if (spi_comm_ss.PORT_DATA) {
			spi_stat_ss.spi_noerror_count++;
		}
	}

	data_in2 = SPI2RXB;
	serial_buffer_ss.data[serial_buffer_ss.raw_index] = data_in2;

	if (serial_buffer_ss.make_value) {
		if (serial_buffer_ss.raw_index == PORT_GO_BYTES) {
			V.bmc_do = serial_buffer_ss.data[1];
			V.bmc_do += (uint32_t) (serial_buffer_ss.data[2] << 8u)&0x0000ff00;
			data_in2 = 0;
			serial_buffer_ss.make_value = false;
			serial_buffer_ss.raw_index = 0;
			while (!SPI2STATUSbits.RXRE) { // clear the FIFO of data
				data_in2 = SPI2RXB;
			}
			spi_stat_ss.txdone_bit++; // number of DO completed packets
			spi_stat_ss.slave_tx_count++;
		} else {
			spi_stat_ss.slave_tx_count++;
			data_in2 = 0;
		}
	}
	if (serial_buffer_ss.get_value) {
		if (serial_buffer_ss.raw_index == PORT_GET_BYTES) {
			SPI2TXB = (uint8_t) V.bmc_di >> ((uint8_t) 8 * (uint8_t) (serial_buffer_ss.raw_index));
			data_in2 = 0;
			serial_buffer_ss.get_value = false;
			serial_buffer_ss.raw_index = 0;
			while (!SPI2STATUSbits.RXRE) { // clear the FIFO of data
				data_in2 = SPI2RXB;
			}
			spi_stat_ss.txdone_bit++; // number of DO completed packets
		} else {
			spi_stat_ss.slave_tx_count++;
			SPI2TXB = (uint8_t) V.bmc_di >> ((uint8_t) 8 * (uint8_t) (serial_buffer_ss.raw_index));
			data_in2 = 0;
		}
	}
	if (++serial_buffer_ss.raw_index > 8) {
		serial_buffer_ss.raw_index = 0;
	}

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
		spi_comm_ss.PORT_DATA = false;
		zombie = false;
		spi_stat_ss.char_count++;
	}

	if (command == CMD_CHAR_DATA) {
		zombie = false;
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
		zombie = false;
		spi_comm_ss.ADC_RUN = false;
		spi_comm_ss.PORT_DATA = false;
		spi_stat_ss.adc_count++;
		channel = data_in2 & LO_NIBBLE;
		if (channel > AI_BUFFER_NUM) {
			channel = channel_ANA0; // invalid channel so set to analog 0
		}

		spi_stat_ss.slave_tx_count++;
		SPI2TXB = ((adc_buffer[channel] >> 8)&0x00ff);
		spi_stat_ss.slave_tx_count++;
		SPI2TXB = (adc_buffer[channel] &0x00ff);

		spi_comm_ss.REMOTE_LINK = true;
		while (!SPI2STATUSbits.RXRE) { // clear the FIFO of data
			data_in2 = SPI2RXB;
		}
	}

	if (command == CMD_PORT_GET) { // send the V.bmc_di buffer
		zombie = false;
		spi_comm_ss.ADC_RUN = false;
		spi_comm_ss.PORT_DATA = true;
		spi_stat_ss.port_count++;
		serial_buffer_ss.raw_index = 0;
		serial_buffer_ss.get_value = true;

		spi_comm_ss.REMOTE_LINK = true;
		while (!SPI2STATUSbits.RXRE) { // clear the FIFO of data
			data_in2 = SPI2RXB;
		}
		TMR0_Reload();
	}

	if (command == CMD_PORT_GO) { // Found a GO for a DO command
		zombie = false;
		spi_comm_ss.ADC_RUN = false;
		spi_comm_ss.PORT_DATA = true;
		spi_stat_ss.port_count++;
		channel = data_in2;
		serial_buffer_ss.raw_index = 0;
		serial_buffer_ss.make_value = true;
		spi_stat_ss.slave_tx_count++;
		serial_buffer_ss.raw_index++;

		spi_comm_ss.REMOTE_LINK = true;
		while (!SPI2STATUSbits.RXRE) { // clear the FIFO of data
			data_in2 = SPI2RXB;
		}
		TMR0_Reload();
	}

	if (command == CMD_CHAR_RX) {
		spi_comm_ss.PORT_DATA = false;
		serial_buffer_ss.tx_buffer = char_rxtmp;
		cmd_dummy = CMD_DUMMY; // clear rx bit
	}

	if (zombie) {
		spi_stat_ss.zombie_count++;
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
	}
	MLED_SetLow();
	DLED_SetLow();
}

static void spi2_tx_wait(void)
{
	uint32_t timeout = 0;

	while (SPI2STATUSbits.SPI2TXBE) {
		if (timeout++ > 10000) {
			break;
		}
	}
}

static void spi2_tx_busy(void)
{
	uint32_t timeout = 0;

	while (SPI2CON2bits.BUSY) {
		if (timeout++ > 10000) {
			break;
		}
	}
}

static void spi2_pace_write(const uint8_t data)
{
	//	spi2_tx_wait();
	SPI2_ExchangeByte(data);
	spi2_tx_busy();
}