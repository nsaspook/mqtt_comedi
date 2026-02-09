/** \file calibr.h
 * Calibration values for host for HV inputs and 200A current sensor
 * 
 * return physical voltage units for channels 4 & 5
 */

#ifndef CALIBR_H
#define	CALIBR_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>

#include "vconfig.h"
#include "mcc_generated_files/crc.h"

#define TATE		0x42
#define EE_CHECKMARK	0x1957
#define  NORMAL  0
#define  REVERSE 1

	static const float HV_SCALE_OFFSET = 0.0f;
	static const float HV_SCALAR4 = 64.2600f; // defaults
	static const float HV_SCALAR5 = 64.2600f;

	static const float HV_SCALE4_0 = 64.2695f;
	static const float HV_SCALE5_0 = 64.2695f;
	static const float HV_SCALE4_1 = 64.1890f;
	static const float HV_SCALE5_1 = 64.1415f;
	static const float HV_SCALE4_2 = 54.1890f;
	static const float HV_SCALE5_2 = 54.1415f;
	static const float HV_SCALE4_3 = 55.6000f;
	static const float HV_SCALE5_3 = 55.6500f;
	static const float HV_SCALE4_4 = 64.3590f;
	static const float HV_SCALE5_4 = 64.3850f;
	static const float HV_SCALE4_5 = 64.3590f;
	static const float HV_SCALE5_5 = 64.3800f;
	static const float HV_SCALE4_6 = 64.3590f;
	static const float HV_SCALE5_6 = 64.3800f;

	static const float A200_0_ZERO = 2.5216f; // Battery sensor zero ADC value
	static const float A200_0_SCALAR = 133.05f; // Battery Amp scalar to +- 200A

	struct ha_daq_calib_type {
		unsigned long long bmc_id;
		double offset4;
		double scaler4;
		double offset5;
		double scaler5;
		double A200_Z;
		double A200_S;
		bool done;
		uint8_t c_zero_cal : 1;
		uint8_t c_scale_cal : 1;
		uint16_t checkmark;
		uint8_t crc; // must be last item in the structure
	};

	extern volatile struct spi_stat_type_ss spi_stat_ss;
	extern struct ha_daq_calib_type ha_daq_calib;

	void set_calibration(unsigned long long);
	float phy_chan4(uint16_t);
	float phy_chan5(uint16_t);

	bool read_cal_data(void);
	void write_cal_data(void);
	void update_cal_data(void);

#ifdef	__cplusplus
}
#endif

#endif	/* CALIBR_H */

