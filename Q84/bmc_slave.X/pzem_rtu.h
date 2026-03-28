/*
 * File:   pzem_rtu.h
 * Author: root
 *
 * Created on March 23, 2026, 8:59 PM
 */

#ifndef PZEM_RTU_H
#define	PZEM_RTU_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "modbus_master.h"

#define PZ_DATA_LEN1	0X40	// 16-bit words returned
#define PZ_DATA_DIR1	0X40	// 16-bit words returned

#define PZMADDR		0x02 // modbus client address
#define MB_PZEM_ID_H	PZMADDR	// slave ID
#define MB_PZEM_ID_L	0x06	// slave baudrate

	typedef enum cmd_pz_type {
		P_DATA1 = 0,
		P_DIR1,
		P_LAST,
	} cmd_pz_type;

	/*
	 * maps the PZEM MODBUS registers to int32_t, uint32_t and uint16_t values
	 */
	typedef __pack struct PZ_data1 {
		volatile uint16_t
		vl1n, vl2n, vl3n,
		al1, al2, al3,
		hz1, hz2, hz3,
		pvp2, pvp3,
		pcp1, pcp2, pcp3;
		volatile uint32_t
		aep1f, aep1r,
		aep2f, aep2r,
		aep3f, aep3r,
		taefd, taerd;

		volatile int32_t
		pap1s, pap2s, pap3s,
		prp1s, prp2s, prp3s,
		papp1, papp2, papp3s,
		caps, c1ps, capps;

		volatile uint16_t
		p1p2pf, p3cpf;

		volatile int32_t
		pae1s, pae2s, pae3s,
		pre1s, pre2s, pre3s,
		pappe1s, pappe2s, pappe3s,
		caes, cres, cappes;
	} PZ_data1;

	typedef struct PZ_tmp {
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
	} PZ_tmp;

	typedef struct PZD_tmp {
		volatile float
		tps, ap1s, ap2s, ap3s,
		rpp1s, rpp2s, rpp3s;
	} PZD_tmp;

	typedef __pack struct PZ_data2 {
		volatile int32_t
		hz;
	} PZ_data2;

	/*
	 * map 16-bit registers to bytes to extract information
	 */
	typedef __pack struct PZ_dir1 {
		volatile uint16_t
		vl1n, vl2n, vl3n,
		al1, al2, al3,
		hz1, hz2, hz3,
		pvp2, pvp3,
		pcp1, pcp2, pcp3;
	} PZ_dir1;

	extern PZ_tmp pz_tmp;
	extern PZD_tmp pzd_tmp;

	void pz_my_modbus_rx_32(void);
	void init_pz_mb_master_timers(void);
	int8_t pzem_controller_work(C_data *);
	void pzem_version(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PZEM_RTU_H */

