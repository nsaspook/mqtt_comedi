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

#define IM_DATA_LEN1	74	// 16-bit words returned
#define IM_DATA_LEN2	64	// 16-bit words returned

#define MB_IAMMETER_ID_H	0x01	// slave ID
#define MB_IAMMETER_ID_L	0x06	// slabe baudrate

	/*
	 * maps the IAMMETER MODBUS registers to int32_t, uint32_t and uint16_t values
	 */
	typedef __pack struct IM_data1 {
		volatile uint16_t vl1n, al1, wl1;
		volatile uint32_t aef1;
		volatile uint16_t pfl1;
		volatile uint32_t aer1;
		volatile uint16_t pdi1;

		volatile uint16_t vl2n, al2, wl2;
		volatile uint32_t aef2;
		volatile uint16_t pfl2;
		volatile uint32_t aer2;
		volatile uint16_t pdi2;

		volatile uint16_t vl3n, al3, wl3;
		volatile uint32_t aef3;
		volatile uint16_t pfl3;
		volatile uint32_t aer3;
		volatile uint16_t pdi3;

		volatile uint32_t taef;
		volatile uint16_t hz;
		volatile uint32_t taer;

		volatile uint32_t
		aep1f, aep1r,
		aep2f, aep2r,
		aep3f, aep3r,
		taefd, taerd;

		volatile int32_t
		tps, ap1s, ap2s, ap3s,
		rpp1s, rpp2s, rpp3s;

		volatile uint32_t
		frei1, rrec1,
		frei2, rrec2,
		frei3, rrec3;
	} IM_data1;

	typedef struct IM_tmp {
		volatile float
		vl1n, vl2n, vl3n,
		vl1l2, vl2l3, vl3l1,
		al1, al2, al3,
		wl1, wl2, wl3,
		val1, val2, val3,
		varl1, varl2, varl3,
		vlnsys, vllsys, wsys, vasys, varsys,
		pfl1, pfl2, pfl3, pfsys,
		phaseseq, hz;
	} IM_tmp;

	typedef __pack struct IM_data2 {
		volatile int64_t
		kwhpt, kvarhpt, kwhpp, kvarhpp,
		kwhpl1, kwhpl2, kwhpl3,
		kwhnt, kvarhnt, kwhnp, kvarhnp,
		kvaht, kvahp;
		volatile int32_t
		rhm, rhmk, rhmp, rhmkp,
		hz, rhlc;
	} IM_data2;

	typedef __pack struct IM_serial {
		volatile char serial[14];
		volatile int16_t year;
	} IM_serial;

	typedef __pack struct IM_version {
		volatile int16_t firmware;
	} IM_version;

	void init_im_mb_master_timers(void);
	int8_t iammeter_controller_work(C_data *);
	int8_t reset_iammeter_kwh(C_data *);
	uint16_t modbus_dcu_send_msg_im(void *, const void *, const uint16_t);
	void iammeter_version(void);

#ifdef	__cplusplus
}
#endif

#endif	/* IAMMETER_RTU_H */

