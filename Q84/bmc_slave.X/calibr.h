/** \file calibr.h
 * Calibration values for host for HV inputs and 200A current sensor
 */

#ifndef CALIBR_H
#define	CALIBR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

#include "vconfig.h"

#define HV_SCALE_OFFSET         0.0f
#define HV_SCALAR4		64.2600f // defaults
#define HV_SCALAR5		64.2600f

#define HV_SCALE4_0             64.2600f
#define HV_SCALE5_0             64.2600f
#define HV_SCALE4_1             64.1890f
#define HV_SCALE5_1             64.1415f
#define HV_SCALE4_2             54.1890f
#define HV_SCALE5_2             54.1415f
#define HV_SCALE4_3             64.1890f
#define HV_SCALE5_3             64.1415f
#define HV_SCALE4_4             64.1890f
#define HV_SCALE5_4             64.1415f

#define A200_0_ZERO		2.5216f // Battery sensor zero ADC value
#define A200_0_SCALAR		133.05f // Battery Amp scalar to +- 200A

	struct ha_daq_calib_type {
		uint16_t checkmark;
		long long bmc_id;
		double offset4;
		double scaler4;
		double offset5;
		double scaler5;
		double A200_Z;
		double A200_S;
	};

	extern volatile struct spi_stat_type_ss spi_stat_ss;
	extern struct ha_daq_calib_type ha_daq_calib;

	void set_calibration(unsigned long long mui);

#ifdef	__cplusplus
}
#endif

#endif	/* CALIBR_H */

