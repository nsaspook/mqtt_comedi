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

    struct spi_link_type_ss { // internal state table
        uint8_t SPI_DATA : 1;
        uint8_t ADC_DATA : 1;
        uint8_t PORT_DATA : 1;
        uint8_t CHAR_DATA : 1;
        uint8_t REMOTE_LINK : 1;
        uint8_t REMOTE_DATA_DONE : 1;
        uint8_t LOW_BITS : 1;
    };

    struct spi_stat_type_ss {
        volatile uint32_t adc_count, adc_error_count,
        port_count, port_error_count,
        char_count, char_error_count,
        slave_int_count, last_slave_int_count,
        comm_count, idle_count;
        volatile uint8_t comm_ok;
    };

    struct serial_bounce_buffer_type_ss {
        uint8_t data[2];
        uint32_t place;
    };

    extern volatile struct spi_link_type_ss spi_comm_ss;
    extern volatile struct spi_stat_type_ss spi_stat_ss, report_stat_ss;

    void check_slaveo(void);
    void init_slaveo(void);

    void slaveo_rx_isr(void);
    void slaveo_tx_isr(void);
    void slaveo_spi_isr(void);
    void slaveo_adc_isr(void);
    void slaveo_time_isr(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SLAVEO_H */

