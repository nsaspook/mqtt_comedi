#include "specials.h"

/*
 * special data handling for BMC boards
 */
uint32_t adc_specials(enum specials_type s_type)
{
	uint32_t ret = 0;

	switch (s_type) {
	case ADC_SPECIALS_bsensor1: // select AND5 for board index 6
		if (ha_daq_host.bindex == 10) { // testing
			R.bsensor1 = lp_filter((E.adc[channel_AND5] - ha_daq_host.calib.A100_Z[ha_daq_host.bindex]) * ha_daq_host.calib.A100_S[ha_daq_host.bindex], BSENSOR1, true);
		} else {
			R.bsensor1 = lp_filter((E.adc[channel_ANA1] - ha_daq_host.calib.A100_Z[ha_daq_host.bindex]) * ha_daq_host.calib.A100_S[ha_daq_host.bindex], BSENSOR1, true);
		}
		ret = ha_daq_host.bindex;
		break;
	}
	return ret;
}

