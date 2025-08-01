/** \file slaveo.h
 * File:   slaveo.h
 * Author: root
 *
 * Created on April 26, 2025, 11:06 AM
 */

#ifndef SLAVEO_H
#define	SLAVEO_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "mconfig.h"
#include "mcc_generated_files/tmr0.h"
#include "eadog.h"
#include "timers.h"

#define SLAVEO_DRIVER "V0.8"

#define SLAVE_DEBUG

	/* PIC Slave commands */
#define CMD_ZERO        0b00000000
#define CMD_ADC_GO      0b10000000	// Read ADC data
#define CMD_DAC_GO      0x90		// Set DAC data
#define CMD_PORT_GO     0xa0		// Set DO data
#define CMD_CHAR_GO     0b10110000
#define CMD_ADC_DATA	0b11000000
#define CMD_PORT_DATA	0b11010000
#define CMD_CHAR_DATA	0b11100000	// Send serial TX buffer
#define CMD_PORT_GET    0b11110000	// Read DI data
#define CMD_CHAR_GET    0b00010000	// Get serial RX buffer
#define CMD_DUMMY_CFG	0b01000000	// stuff config data in SPI buffer
#define CMD_DEAD        0b11111111      // This is usually a bad response

#define CHECKBYTE	0x57

#define CMD_DUMMY       0b01100111	/* 7 channels 4.096 */
#define NUM_AI_CHAN     15
#define AI_BUFFER_NUM	0x40
#define AI_CHAN_FIX	5

#define NUM_AO_CHAN     1
#define AO_BUFFER_NUM	0x40

#define SPI_BUFFER_LEN	10
#define PACKET_BUF_SIZ	16

#define	HI_NIBBLE       0xf0
#define	LO_NIBBLE       0x0f
#define	ADC_SWAP_MASK	0b01000000
#define UART_DUMMY_MASK	0b01000000

#define PORT_GET_BYTES	7
#define CHAR_GET_BYTES	7
#define CFG_GET_BYTES	7
#define ADC_GET_BYTES	7
#define PORT_GO_BYTES	7
#define CHAR_GO_BYTES	7
#define DAC_GO_BYTES	7

	struct spi_link_type_ss { // internal state table
		uint8_t SPI_DATA : 1;
		uint8_t ADC_DATA : 1;
		uint8_t PORT_DATA : 1;
		uint8_t CHAR_DATA : 1;
		uint8_t REMOTE_LINK : 1;
		uint8_t REMOTE_DATA_DONE : 1;
		uint8_t LOW_BITS : 1;
		uint8_t ADC_RUN : 1;
	};

	struct spi_stat_type_ss {
		volatile uint32_t adc_count, dac_count,
		port_count, port_error_count, port_data_count, zombie_count,
		char_count, char_error_count, rxof_bit, txdone_bit, txuf_bit,
		slave_int_count, last_slave_int_count, slave_tx_count,
		comm_count, idle_count, spi_error_count, spi_noerror_count;
		volatile uint8_t comm_ok, raw_index, daq_conf;
	};

	struct serial_buffer_type_ss {
		volatile uint8_t data[PACKET_BUF_SIZ], adcl, adc2, adch, command, raw_index;
		volatile uint32_t place;
		volatile bool make_value, get_value, dac_value, adc_value, cfg_value, cmake_value, cget_value;
	};

	enum daqbmc_packet_index {
		BMC_CMD = 0,
		BMC_D0,
		BMC_D1,
		BMC_D2,
		BMC_D3,
		BMC_D4,
		BMC_EXT = 6,
		BMC_CKSUM = 7,
		BMC_DUMMY,
	};

	extern volatile struct spi_link_type_ss spi_comm_ss;
	extern volatile struct serial_buffer_type_ss serial_buffer_ss;
	extern volatile struct spi_stat_type_ss spi_stat_ss, report_stat_ss;
	extern volatile uint8_t data_in2, adc_buffer_ptr, adc_channel, channel, upper;
	extern volatile uint16_t adc_buffer[AI_BUFFER_NUM], adc_data_in, dac_buffer[AO_BUFFER_NUM], dac_data_in;
	extern V_data V;
	volatile bool failure;
	extern volatile uint8_t in_buf1, in_buf2, in_buf3;
	extern volatile uint8_t tmp_buf;

	void check_slaveo(void);
	void init_slaveo(void);

	void slaveo_rx_isr(void);
	void slaveo_spi_isr(void);
	void slaveo_time_isr(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SLAVEO_H */

