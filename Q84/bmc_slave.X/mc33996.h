/** \file mc33996.h
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
#define MC33996_DRIVER "V0.6"   

	/*
	 * MC33996 command structure
	 */
	typedef struct __attribute__((packed))
	{
		uint8_t cmd;
		uint16_t out;
	}
	mc33996buf_type;

	/*
	 * MC33996 response structure
	 */
	typedef struct __attribute__((packed))
	{
		uint8_t faults;
		uint16_t out_faults;
	}
	mc33996read_type;

#define mc33996_onoff           0b00000000
#define mc33996_olce            0b00000100
#define mc33996_gsrc            0b00001011
#define mc33996_sfpd            0b00001100
#define mc33996_pwm             0b00010000
#define mc33996_andor           0b00010100	
#define mc33996_reset           0b00011000
#define mc33996_magic_h         0x19
#define mc33996_magic_l         0x57       

#define MC33996_DATA            6
#define MC33996_DATA_LEN        8

	void mc33996_version(void);
	bool mc33996_init(void);
	void mc33996_update(uint16_t);


#ifdef	__cplusplus
}
#endif

#endif	/* MC33996_H */

