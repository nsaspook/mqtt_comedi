#include "specials.h"

/*
 * special data handling for BMC boards
 */
bool adc_specials(enum specials_type s_type)
{
	switch (s_type) {
	case ADC_SPECIALS_bsensor1: // select AND5 for board index 6
		if (ha_daq_host.bindex == 16) { // testing
			R.bsensor1 = lp_filter((E.adc[channel_AND5] - ha_daq_host.calib.A100_Z[ha_daq_host.bindex]) * ha_daq_host.calib.A100_S[ha_daq_host.bindex], BSENSOR1, true);
		} else {
			R.bsensor1 = lp_filter((E.adc[channel_ANA1] - ha_daq_host.calib.A100_Z[ha_daq_host.bindex]) * ha_daq_host.calib.A100_S[ha_daq_host.bindex], BSENSOR1, true);
		}
		return true;
		break;
	}
	return false;
}

