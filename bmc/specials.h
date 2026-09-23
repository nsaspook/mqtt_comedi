/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cFiles/file.h to edit this template
 */

/* 
 * File:   specials.h
 * Author: root
 *
 * Created on September 14, 2026, 10:09 AM
 * 
 * handle BMC boards special data connection configurations
 */

#ifndef SPECIALS_H
#define SPECIALS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bmc_mqtt.h"

	enum specials_type {
		ADC_SPECIALS_bsensor1,
	};

	uint32_t adc_specials(enum specials_type);

#ifdef __cplusplus
}
#endif

#endif /* SPECIALS_H */

