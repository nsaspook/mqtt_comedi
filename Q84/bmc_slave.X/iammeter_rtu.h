/*
 * File:   iammeter_rtu.h
 * Author: root
 *
 * Created on March 4, 2026, 8:59 PM
 */

#ifndef IAMMETER_RTU_H
#define	IAMMETER_RTU_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "modbus_master.h"

	int8_t iammeter_controller_work(C_data *);
	int8_t reset_iammeter_kwh(C_data *);
	uint16_t modbus_dcu_send_msg_im(void *, const void *, const uint16_t);
	void iammeter_version(void);

#ifdef	__cplusplus
}
#endif

#endif	/* IAMMETER_RTU_H */

