/** \file rs232.h
 * File:   rs232.h
 * Author: root
 *
 * Created on February 10, 2025, 3:52 PM
 */

#ifndef RS232_H
#define	RS232_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
 * RS232 ADC configuration values for Mark, Space, Open and Voltage calibration
 */
	
#define LINE_OPEN_V     0x8c0 // open wire
#define LINE_RECV_V     LINE_OPEN_V // connected to recever input only
#define LINE_MARK_V     1425 // xmit -8 volts
#define LINE_SPACE_V	2390 // xmit +9 volts
#define LINE_LIMIT_H	500
#define LINE_LIMIT_LOW	100
#define LINE_LIMIT_MARK	300
#define LINE_LIMIT_OPEN	50

#include "vconfig.h"
	const int16_t pos_scale = 35,
		neg_scale = 60,
		line_zero_limit = -24,
		adc_scale_zero = -LINE_OPEN_V;
	void update_rs232_line_status(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RS232_H */

