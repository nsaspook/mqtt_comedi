/** \file pzem_rtu.h
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
#include "iammeter_rtu.h"

#define PZ_DATA_LEN1	0X40	// 16-bit words returned
#define PZ_DATA_DIR1	0X40	// 16-bit words returned
#define PZ_DATA_HZ1	0x01

#define PZMADDR		0x02 // modbus client address
#define MB_PZEM_ID_H	PZMADDR	// slave ID
#define MB_PZEM_ID_L	0x06	// slave baudrate

#define WRITE_PZEM_REGISTER		0x10

	// Input register addresses
#define PZEM_VOLTAGE_REG                  0x0000 // A=0x0000, B=0x0001, C=0x0002
#define PZEM_CURRENT_REG                  0x0003 // A=0x0003, B=0x0004, C=0x0005
#define PZEM_FREQUENCY_REG                0x0006 // A=0x0006, B=0x0007, C=0x0008
#define PZEM_VOLTAGE_PHASE_REG            0x0009 // B=0x0009, C=0x000A (A is reference)
#define PZEM_CURRENT_PHASE_REG            0x000B // A=0x000B, B=0x000C, C=0x000D
#define PZEM_ACTIVE_POWER_REG             0x000E // A=0x000E/0x000F, B=0x0010/0x0011, C=0x0012/0x0013
#define PZEM_REACTIVE_POWER_REG           0x0014 // A=0x0014/0x0015, B=0x0016/0x0017, C=0x0018/0x0019
#define PZEM_APPARENT_POWER_REG           0x001A // A=0x001A/0x001B, B=0x001C/0x001D, C=0x001E/0x001F
#define PZEM_ACTIVE_POWER_COMBINED_REG    0x0020 // combined=0x0020/0x0021
#define PZEM_REACTIVE_POWER_COMBINED_REG  0x0022 // combined=0x0022/0x0023
#define PZEM_APPARENT_POWER_COMBINED_REG  0x0024 // combined=0x0024/0x0025
#define PZEM_POWER_FACTOR_A_B_REG         0x0026 // A=hi, B=lo
#define PZEM_POWER_FACTOR_C_COMBINED_REG  0x0027 // C=hi, combined=lo
#define PZEM_ACTIVE_ENERGY_REG            0x0028 // A=0x0028/0x0029, B=0x002A/0x002B, C=0x002C/0x002D
#define PZEM_REACTIVE_ENERGY_REG          0x002E // A=0x002E/0x002F, B=0x0030/0x0031, C=0x0032/0x0033
#define PZEM_APPARENT_ENERGY_REG          0x0034 // A=0x0034/0x0035, B=0x0036/0x0037, C=0x0038/0x0039
#define PZEM_ACTIVE_ENERGY_COMBINED_REG   0x003A // combined=0x003A/0x003B
#define PZEM_REACTIVE_ENERGY_COMBINED_REG 0x003C // combined=0x003C/0x003D
#define PZEM_APPARENT_ENERGY_COMBINED_REG 0x003E ///< Apparent energy combined register address

	// Parameter registers
#define PZEM_ADDRESS_REG            0x0000  ///< Address register (addr=hi, addr type=lo)
#define PZEM_BAUDRATE_TYPE_REG      0x0001  ///< Baudrate/connection type register (connection type=hi, baudrate=lo)
#define PZEM_FREQUENCY_SYSTEM_REG   0x0002  ///< Frequency system register (reserved=hi, frequency=lo)
	/** @} */

	/**
	 * @defgroup PZEM6L24Resolutions PZEM-6L24 Resolutions
	 * @brief Resolution factors for converting raw register values to physical units
	 * @{
	 */
#define PZEM_VOLTAGE_RESOLUTION      0.1f
#define PZEM_CURRENT_RESOLUTION      0.01f
#define PZEM_FREQUENCY_RESOLUTION    0.01f
#define PZEM_POWER_RESOLUTION        0.1f
#define PZEM_POWER_FACTOR_RESOLUTION 0.01f
#define PZEM_ENERGY_RESOLUTION       0.1f
#define PZEM_PHASE_RESOLUTION        0.01f  ///< Phase angle resolution (degrees per LSB)
	/** @} */

	/**
	 * @defgroup PZEM6L24ResetOptions Reset Energy Options
	 * @brief Options for selective energy counter reset
	 * @{
	 */
#define PZEM_RESET_ENERGY_A        0x00  ///< Reset phase A energy
#define PZEM_RESET_ENERGY_B        0x01  ///< Reset phase B energy
#define PZEM_RESET_ENERGY_C        0x02  ///< Reset phase C energy
#define PZEM_RESET_ENERGY_COMBINED 0x03  ///< Reset combined energy
#define PZEM_RESET_ENERGY_ALL      0x0F  ///< Reset all energy counters
	/** @} */

	/**
	 * @defgroup PZEM6L24Baudrates Baudrate Options
	 * @brief Supported baudrate values
	 * @{
	 */
#define PZEM_BAUDRATE_2400   0x00
#define PZEM_BAUDRATE_4800   0x01
#define PZEM_BAUDRATE_9600   0x02
#define PZEM_BAUDRATE_19200  0x03
#define PZEM_BAUDRATE_38400  0x04
#define PZEM_BAUDRATE_57600  0x05
#define PZEM_BAUDRATE_115200 0x06  ///< 115200 baud
	/** @} */

	/**
	 * @defgroup PZEM6L24ConnectionTypes Connection Type Options
	 * @brief Three-phase connection type options
	 * @{
	 */
#define PZEM_CONNECTION_3PHASE_4WIRE 0x00  ///< 3-phase 4-wire connection
#define PZEM_CONNECTION_3PHASE_3WIRE 0x01  ///< 3-phase 3-wire connection
	/** @} */

	/**
	 * @defgroup PZEM6L24Frequencies Frequency Options
	 * @brief AC frequency system options
	 * @{
	 */
#define PZEM_FREQUENCY_50HZ 0x00  ///< 50 Hz AC system
#define PZEM_FREQUENCY_60HZ 0x01  ///< 60 Hz AC system

	/** @} */

	typedef enum cmd_pz_type {
		P_DATA1 = 0,
		P_HZ1,
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

		volatile int32_t
		pap1s, pap2s, pap3s,
		prp1s, prp2s, prp3s,
		papp1s, papp2s, papp3s,
		caps, c1ps, capps;

		volatile uint16_t // power factor
		p1p2pf, p3cpf;

		volatile uint32_t // energy in kWh, kVarh, KVah
		pae1s, pae2s, pae3s,
		pre1s, pre2s, pre3s,
		pappe1s, pappe2s, pappe3s,
		caes, cres, cappes;
	} PZ_data1;

	void pz_my_modbus_rx_32(void);
	void init_pz_mb_master_timers(void);
	int8_t pzem_controller_work(C_data *);
	void pzem_version(void);

#ifdef	__cplusplus
}
#endif

#endif	/* PZEM_RTU_H */

