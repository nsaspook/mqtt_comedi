#include "calibr.h"

/*
 * use MUI to find calibration data for each board
 */
void set_calibration(unsigned long long mui)
{
	/*
	 * calibration scalar selection using MUI from controller
	 */
	switch (mui) {
	case 0x589B6: //
		ha_daq_calib.scaler4 = HV_SCALE4_0;
		ha_daq_calib.scaler5 = HV_SCALE5_0;
		break;
	case 0: // USBBoard
		ha_daq_calib.scaler4 = HV_SCALE4_1;
		ha_daq_calib.scaler5 = HV_SCALE5_1;
		break;
	case 0x598F3: //
		ha_daq_calib.scaler4 = HV_SCALE4_2;
		ha_daq_calib.scaler5 = HV_SCALE5_2;
		break;
	case 0x5ABB6: //
		ha_daq_calib.scaler4 = HV_SCALE4_3;
		ha_daq_calib.scaler5 = HV_SCALE5_3;
		break;
		// dead chip/replaced	case 0x55AF3:
	case 0x4DE66:
		ha_daq_calib.scaler4 = HV_SCALE4_4;
		ha_daq_calib.scaler5 = HV_SCALE5_4;
		break;
	default:
		ha_daq_calib.scaler4 = HV_SCALAR4;
		ha_daq_calib.scaler5 = HV_SCALAR5;
		break;
	}
}

/*
 * return scaled voltages
 */
float phy_chan4(uint16_t value)
{
	if (value) {
		return((((float) value)) / 4096.0f) * ha_daq_calib.scaler4;
	} else {
		return 0.00001f;
	}
}

float phy_chan5(uint16_t value)
{
	if (value) {
		return((((float) value)) / 4096.0f) * ha_daq_calib.scaler5;
	} else {
		return 0.00001f;
	}
}

