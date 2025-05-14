/* 
 * File:   mc33996.h
 * Author: root
 *
 * Created on May 12, 2025, 12:56 PM
 */

#ifndef MC33996_H
#define	MC33996_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "mconfig.h"

	/*
	 * 24-bits SPI mode 1, 4MHz SCL
	 */
#define MC33996_DRIVER "V0.3"   

	/*
	 * MC33996 command structure
	 */
	typedef struct __attribute__((packed))
	{
		uint16_t out;
		uint8_t cmd;
	}
	mc33996buf_type;

	/*
	 * MC33996 response structure
	 */
	typedef struct __attribute__((packed))
	{
		uint16_t out_faults;
		uint8_t faults;
	}
	mc33996read_type;

#define mc33996_control         0x00
#define mc33996_load            0x04
#define mc33996_reset           0x18
#define mc33996_magic_h         0x19
#define mc33996_magic_l         0x57       

#define MC33996_DATA            6
#define MC33996_DATA_LEN        8

	void mc33996_version(void);




#ifdef	__cplusplus
}
#endif

#endif	/* MC33996_H */

