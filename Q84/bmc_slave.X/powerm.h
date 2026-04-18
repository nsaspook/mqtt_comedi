/* 
 * File:   powerm.h
 * Author: root
 *
 * Created on April 17, 2026, 8:02 AM
 */

#ifndef POWERM_H
#define	POWERM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "modbus_master.h"
#include "iammeter_rtu.h"
	
	/*
	 * GTI Inverter power control routines via RS485
	 */
	void powerm_my_modbus_rx_32(void);
	void init_powerm_mb_master_timers(void);
	int8_t powerm_controller_work(C_data *);
	void powerm_version(void);

#ifdef	__cplusplus
}
#endif

#endif	/* POWERM_H */

