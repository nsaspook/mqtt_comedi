/* 
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

    /* PIC Slave commands */
#define CMD_ZERO        0b00000000
#define CMD_ADC_GO      0b10000000
#define CMD_PORT_GO     0b10100000	// send data LO_NIBBLE to port buffer
#define CMD_CHAR_GO     0b10110000	// send data LO_NIBBLE to TX buffer
#define CMD_ADC_DATA	0b11000000
#define CMD_PORT_DATA	0b11010000	// send data HI_NIBBLE to port buffer ->PORT and return input PORT data in received SPI data byte
#define CMD_CHAR_DATA	0b11100000	// send data HI_NIBBLE to TX buffer and return RX buffer in received SPI data byte
#define CMD_XXXX        0b11110000	//
#define CMD_CHAR_RX     0b00010000	// Get RX buffer
#define CMD_DUMMY_CFG	0b01000000	// stuff config data in SPI buffer
#define CMD_DEAD        0b11111111      // This is usually a bad response

#define CMD_DUMMY       0b01100111	/* 7 channels 4.096 */
#define NUM_AI_CHAN     13
#define AI_BUFFER_NUM	0x40

#define	HI_NIBBLE       0xf0
#define	LO_NIBBLE       0x0f
#define	ADC_SWAP_MASK	0b01000000
#define UART_DUMMY_MASK	0b01000000

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
        volatile uint32_t adc_count,
        port_count, port_error_count,
        char_count, char_error_count,
        slave_int_count, last_slave_int_count, slave_tx_count,
        comm_count, idle_count, spi_error_count;
        volatile uint8_t comm_ok;
    };

    struct serial_buffer_type_ss {
        volatile uint8_t data[4], tx_buffer, adcl, adc2, adch, command;
        volatile uint32_t place;
    };

    extern volatile struct spi_link_type_ss spi_comm_ss;
    extern volatile struct serial_buffer_type_ss serial_buffer_ss;
    extern volatile struct spi_stat_type_ss spi_stat_ss, report_stat_ss;
    extern volatile uint8_t data_in2, adc_buffer_ptr, adc_channel, channel, upper;
    extern volatile uint16_t adc_buffer[AI_BUFFER_NUM], adc_data_in;

    void check_slaveo(void);
    void init_slaveo(void);

    void slaveo_rx_isr(void);
    void slaveo_spi_isr(void);
    void slaveo_time_isr(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SLAVEO_H */

