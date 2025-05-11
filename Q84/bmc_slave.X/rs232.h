/* 
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

#include "vconfig.h"
	const int16_t pos_scale = 35,
		neg_scale = 60,
		line_zero_limit = -24,
		adc_scale_zero = -2048;
	void update_rs232_line_status(void);

#ifdef	__cplusplus
}
#endif

#endif	/* RS232_H */

