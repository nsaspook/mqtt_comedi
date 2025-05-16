/* 
 * File:   tic12400.h
 * Author: root
 *
 * Created on May 12, 2025, 12:56 PM
 */

#ifndef TIC12400_H
#define	TIC12400_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "mconfig.h"
#include "timers.h"
#include "vconfig.h"
#include "eadog.h"

	/*
	 * 32-bits SPI mode 1, 4MHz SCK
	 */
#define TIC12400_DRIVER "V0.6"

#define por_bit_s		0b00111110000000000000000000000000
#define spi_fail_bit_s		0b01000000000000000000000000000000
#define ssc_bit_s		0b00010000000000000000000000000000
#define spi_fail_bit_v		0b01000000000000000000000000000000
#define parity_fail_v		0b00100000000000000000000000000000
#define id_mask_d		0b00000000000000000000011111111111
#define switch_mask_d		0b011111111111111111111111
	/*
	 * switch bit masks in the raw 32-bit register from the TIC12400
	 */
#define raw_mask_0 0b010
#define raw_mask_11 0b100000000000000

	extern volatile struct spi_link_type spi_link;
	/*
	 * TIC12400 command structure
	 */
	//typedef struct __attribute__((packed))
	//{
	//	uint32_t par : 1;
	//	uint32_t data : 24;
	//	uint32_t addr : 6;
	//	uint32_t wr : 1;
	//}
	//ticbuf_type;

	typedef struct __attribute__((packed))
	{
		uint8_t data3;
		uint8_t data2;
		uint8_t data1;
		uint8_t data0;
	}
	ticbuf_type;


	/*
	 * TIC12400 response structure
	 */
	//typedef struct __attribute__((packed))
	//{
	//	uint32_t par : 1;
	//	uint32_t data : 24;
	//	uint32_t oi : 1;
	//	uint32_t temp : 1;
	//	uint32_t vs_th : 1;
	//	uint32_t ssc : 1;
	//	uint32_t parity_fail : 1;
	//	uint32_t spi_fail : 1;
	//	uint32_t por : 1;
	//}
	//ticread_type;

	/*
	 * TIC12400 response structure
	 */
	typedef struct __attribute__((packed))
	{
		uint8_t data3;
		uint8_t data2;
		uint8_t data1;
		uint8_t data0;
	}
	ticread_type;

	void tic12400_version(void);
	void tic12400_reset(void);
	bool tic12400_init(void);
	uint32_t tic12400_wr(const ticbuf_type *, uint16_t);
	uint32_t tic12400_get_sw(void);
	void tic12400_read_sw(uint32_t, uintptr_t);
	bool tic12400_parity(uint32_t);

	extern volatile uint32_t tic12400_status, tic12400_counts, tic12400_value_counts, tic12400_id;
	extern volatile uint32_t tic12400_value, tic12400_switch, tic12400_fail_count, tic12400_parity_count;
	extern volatile bool tic12400_init_fail, tic12400_event;
	extern volatile bool tic12400_parity_status, tic12400_read_error;
	extern volatile int32_t tic12400_fail_value;

#ifdef	__cplusplus
}
#endif

#endif	/* TIC12400_H */

