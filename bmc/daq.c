/** \file daq.c
 *
 */

#include <stdio.h> /* for printf() */
#include <unistd.h>
#include <stdbool.h>
#include <stdint.h>
#include <comedilib.h>
#include "daq.h"

int subdev_ai = 0; /* change this to your input subdevice */
int chan_ai = 0; /* change this to your channel */
int range_ai = 0; /* more on this later */
int aref_ai = AREF_GROUND; /* more on this later */
int maxdata_ai, ranges_ai, channels_ai;

int subdev_ao = 0; /* change this to your input subdevice */
int chan_ao = 0; /* change this to your channel */
int range_ao = 0; /* more on this later */
int aref_ao = AREF_GROUND; /* more on this later */
int maxdata_ao, ranges_ao, channels_ao;

int subdev_di = 0; /* change this to your input subdevice */
int chan_di = 0; /* change this to your channel */
int range_di = 0; /* more on this later */
int maxdata_di, ranges_di, channels_di, datain_di;

int subdev_do = 0; /* change this to your input subdevice */
int chan_do = 0; /* change this to your channel */
int range_do = 0; /* more on this later */
int maxdata_do, ranges_do, channels_do, datain_do;

int subdev_dio; /* change this to your input subdevice */
int chan_dio = 0; /* change this to your channel */
int range_dio = 0; /* more on this later */
int maxdata_dio, ranges_dio, channels_dio, datain_dio;
int aref_dio; /* more on this later */

int subdev_counter; /* change this to your input subdevice */
int chan_counter = 0; /* change this to your channel */
int range_counter = 0; /* more on this later */
int maxdata_counter, ranges_counter, channels_counter, datain_counter;

int subdev_serial0; /* change this to your input subdevice */
int chan_serial0 = 0; /* change this to your channel */
int range_serial0 = 0; /* more on this later */
int maxdata_serial0, ranges_serial0, channels_serial0, datain_serial0;

comedi_t *it;
comedi_range *ad_range, *da_range;
bool ADC_OPEN = true, DIO_OPEN = true, ADC_ERROR = false, DEV_OPEN = true,
	DIO_ERROR = false, HAS_AO = false, DAC_ERROR = false, PWM_OPEN = true,
	PWM_ERROR = false, SERIAL_OPEN = true, RX_OPEN = true;

bool DO_OPEN = true, DI_OPEN = true, DO_ERROR = false;
union dio_buf_type obits, ibits;

uint32_t datain, serial_buf, daq_bmc_data[SYSLOG_SIZ];
const char text_test[] = {"the quick brown fox jumps over the lazy dogs back"};

char *daq_text[] = {
	"daq_bmc text 0           ",
	"daq_bmc text 1           ",
	"daq_bmc text 2           ",
	"daq_bmc text 3           ",
}, *daq_text_ptr, daq_bmc_data_text[SYSLOG_SIZ], daq_bmc_data_buf[SYSLOG_SIZ];

uint8_t daq_text_index = 0, line_index = 0, daq_data_index = 0;
static uint32_t slow_data = 0;

struct bmc_buffer_type BMC4 = {
	.log_buffer = daq_bmc_data,
	.buffer = daq_bmc_data_text,
	.pos = 0,
	.len = 0,
};

int init_daq(double min_range, double max_range, int range_update)
{
	int i = 0;

	it = comedi_open("/dev/comedi0");
	if (it == NULL) {
		comedi_perror("comedi_open");
		DEV_OPEN = false;
		return -1;
	}

	subdev_ai = comedi_find_subdevice_by_type(it, COMEDI_SUBD_AI, subdev_ai);
	if (subdev_ai < 0) {
		ADC_OPEN = false;
	}


	subdev_ao = comedi_find_subdevice_by_type(it, COMEDI_SUBD_AO, subdev_ao);
	if (subdev_ao < 0) {
		HAS_AO = false;
	} else {
		HAS_AO = true;
	}

	fprintf(fout, "Comedi DAQ Board Name: %s, Driver: %s\r\n", comedi_get_board_name(it), comedi_get_driver_name(it));
	if (strcmp(comedi_get_board_name(it), BMCBoard) == 0) {
		bmc.BOARD = bmcboard;
		bmc.BNAME = BMCBoard;
	}
	if (strcmp(comedi_get_board_name(it), USBBoard) == 0) {
		bmc.BOARD = usbboard;
		bmc.BNAME = USBBoard;
	}

	fprintf(fout, "Subdev AI  %i ", subdev_ai);
	channels_ai = comedi_get_n_channels(it, subdev_ai);
	fprintf(fout, "Analog  Channels %i ", channels_ai);
	maxdata_ai = comedi_get_maxdata(it, subdev_ai, i);
	fprintf(fout, "Maxdata %i ", maxdata_ai);
	ranges_ai = comedi_get_n_ranges(it, subdev_ai, i);
	fprintf(fout, "Ranges %i ", ranges_ai);
	ad_range = comedi_get_range(it, subdev_ai, i, range_ai);
	if (range_update) {
		ad_range->min = min_range;
		ad_range->max = max_range;
	}
	fprintf(fout, ": ad_range .min = %.3f, max = %.3f\r\n", ad_range->min,
		ad_range->max);

	if (HAS_AO) {
		fprintf(fout, "Subdev AO  %i ", subdev_ao);
		channels_ao = comedi_get_n_channels(it, subdev_ao);
		fprintf(fout, "Analog  Channels %i ", channels_ao);
		maxdata_ao = comedi_get_maxdata(it, subdev_ao, i);
		fprintf(fout, "Maxdata %i ", maxdata_ao);
		ranges_ao = comedi_get_n_ranges(it, subdev_ao, i);
		fprintf(fout, "Ranges %i ", ranges_ao);
		da_range = comedi_get_range(it, subdev_ao, i, range_ao);
		fprintf(fout, ": da_range .min = %.3f, max = %.3f\r\n", da_range->min,
			da_range->max);
	}

	ADC_OPEN = true;
	comedi_set_global_oor_behavior(COMEDI_OOR_NUMBER);

	daq_text_ptr = daq_text[0];
	return 0;
}

int init_dac(double min_range, double max_range, int range_update)
{
	int i = 0;

	if (!DEV_OPEN) {
		it = comedi_open("/dev/comedi0");
		if (it == NULL) {
			comedi_perror("comedi_open");
			ADC_OPEN = false;
			DEV_OPEN = false;
			return -1;
		}
		DEV_OPEN = true;
	}

	subdev_ao = comedi_find_subdevice_by_type(it, COMEDI_SUBD_AO, subdev_ao);
	if (subdev_ao < 0) {
		HAS_AO = false;
	} else {
		HAS_AO = true;
	}

	if (HAS_AO) {
		fprintf(fout, "Subdev AO  %i ", subdev_ao);
		channels_ao = comedi_get_n_channels(it, subdev_ao);
		fprintf(fout, "Analog  Channels %i ", channels_ao);
		maxdata_ao = comedi_get_maxdata(it, subdev_ao, i);
		fprintf(fout, "Maxdata %i ", maxdata_ao);
		ranges_ao = comedi_get_n_ranges(it, subdev_ao, i);
		fprintf(fout, "Ranges %i ", ranges_ao);
		da_range = comedi_get_range(it, subdev_ao, i, range_ao);
		fprintf(fout, ": da_range .min = %.3f, max = %.3f\r\n", da_range->min,
			da_range->max);
	}

	comedi_set_global_oor_behavior(COMEDI_OOR_NUMBER);
	return 0;
}

int adc_range(double min_range, double max_range)
{
	if (ADC_OPEN) {
		ad_range->min = min_range;
		ad_range->max = max_range;
		return 0;
	} else {
		return -1;
	}
}

int dac_range(double min_range, double max_range)
{
	if (ADC_OPEN) {
		da_range->min = min_range;
		da_range->max = max_range;
		return 0;
	} else {
		return -1;
	}
}

int set_dac_volts(int chan, double voltage)
{
	lsampl_t data;
	int retval;

	data = comedi_from_phys(voltage, da_range, maxdata_ao);
	bmc.dac_sample[chan] = data;
	retval = comedi_data_write(it, subdev_ao, chan, range_ao, aref_ao, data);
	if (retval < 0) {
		comedi_perror("comedi_data_write in set_dac_volts");
		DAC_ERROR = true;
	}
	return retval;
}

int set_dac_raw(int chan, lsampl_t voltage)
{
	int retval;

	retval = comedi_data_write(it, subdev_ao, chan, range_ao, aref_ao, voltage);
	if (retval < 0) {
		comedi_perror("comedi_data_write in set_dac_raw");
		DAC_ERROR = true;
	}
	return retval;
}

double get_adc_volts(int chan)
{
	lsampl_t data[16];
	int retval;

	retval = comedi_data_read_n(it, subdev_ai, chan, range_ai, aref_ai, &data[0], 1);
	if (retval < 0) {
		comedi_perror("comedi_data_read in get_adc_volts");
		ADC_ERROR = true;
		return 0.0;
	}
	bmc.adc_sample[chan] = data[0];

	ad_range->min = 0.0f;
	if (bmc.BOARD == bmcboard) {
		if (chan == channel_ANA4 || chan == channel_ANA5) {
			if (chan == channel_ANA4) {
				ad_range->max = ha_daq_host.calib.scaler4[ha_daq_host.bindex];
			} else {
				ad_range->max = ha_daq_host.calib.scaler5[ha_daq_host.bindex];
			}
			ad_range->min = 0.0f;
		} else {
			ad_range->max = HV_SCALE_RAW;
			ad_range->min = 0.0f;
		}
	} else {
		ad_range->max = ha_daq_host.scaler[ha_daq_host.hindex];
	}

	return comedi_to_phys(data[0], ad_range, maxdata_ai);
}

int set_dio_output(int chan)
{
	return comedi_dio_config(it,
		subdev_dio,
		chan,
		COMEDI_OUTPUT);
}

int set_dio_input(int chan)
{
	return comedi_dio_config(it,
		subdev_dio,
		chan,
		COMEDI_INPUT);
}

int get_dio_bit(int chan)
{
	lsampl_t data;
	int retval;

	retval = comedi_data_read(it, subdev_di, chan, range_di, aref_dio, &data);
	if (retval < 0) {
		comedi_perror("comedi_data_read in get_di_bits");
		DIO_ERROR = true;
		return 0;
	}
	return data;
}

int put_dio_bit(int chan, int bit_data)
{
	lsampl_t data = bit_data;
	int retval;

	retval = comedi_data_write(it, subdev_do, chan, range_do, aref_dio, data);
	if (retval < 0) {
		comedi_perror("comedi_data_write in put_do_bits");
		DIO_ERROR = true;
		return -1;
	}
	return 0;
}

int init_dio(void)
{
	int i = 0;

	if (!DEV_OPEN) {
		it = comedi_open("/dev/comedi0");
		if (it == NULL) {
			comedi_perror("comedi_open");
			DIO_OPEN = false;
			DO_OPEN = false;
			DI_OPEN = false;
			DEV_OPEN = false;
			PWM_OPEN = false;
			SERIAL_OPEN = false;
			RX_OPEN = false;
			return -1;
		}
		DEV_OPEN = true;
	}

	subdev_di = comedi_find_subdevice_by_type(it, COMEDI_SUBD_DI, subdev_di);
	if (subdev_di < 0) {
		DI_OPEN = false;
	}

	subdev_do = comedi_find_subdevice_by_type(it, COMEDI_SUBD_DO, subdev_do);
	if (subdev_do < 0) {
		DO_OPEN = false;
	}

	subdev_dio = comedi_find_subdevice_by_type(it, COMEDI_SUBD_DIO, subdev_dio);
	if (subdev_dio < 0) {
		DIO_OPEN = false;
	}

	subdev_counter = comedi_find_subdevice_by_type(it, COMEDI_SUBD_COUNTER, subdev_counter);
	if (subdev_counter < 0) {
		PWM_OPEN = false;
	}

	subdev_serial0 = comedi_find_subdevice_by_type(it, COMEDI_SUBD_MEMORY, subdev_serial0);
	if (subdev_serial0 < 0) {
		SERIAL_OPEN = false;
	}

	if (DO_OPEN) {
		fprintf(fout, "Subdev DO  %i ", subdev_do);
		channels_do = comedi_get_n_channels(it, subdev_do);
		fprintf(fout, "Digital Channels %i ", channels_do);
		maxdata_do = comedi_get_maxdata(it, subdev_do, i);
		fprintf(fout, "Maxdata %i ", maxdata_do);
		ranges_do = comedi_get_n_ranges(it, subdev_do, i);
		fprintf(fout, "Ranges %i \r\n", ranges_do);
	}

	if (DI_OPEN) {
		fprintf(fout, "Subdev DI  %i ", subdev_di);
		channels_di = comedi_get_n_channels(it, subdev_di);
		fprintf(fout, "Digital Channels %i ", channels_di);
		maxdata_di = comedi_get_maxdata(it, subdev_di, i);
		fprintf(fout, "Maxdata %i ", maxdata_di);
		ranges_di = comedi_get_n_ranges(it, subdev_di, i);
		fprintf(fout, "Ranges %i \r\n", ranges_di);
	}


	if (DIO_OPEN) {
		fprintf(fout, "Subdev DIO  %i ", subdev_dio);
		channels_dio = comedi_get_n_channels(it, subdev_dio);
		fprintf(fout, "Digital Channels %i ", channels_dio);
		maxdata_dio = comedi_get_maxdata(it, subdev_dio, i);
		fprintf(fout, "Maxdata %i ", maxdata_dio);
		ranges_dio = comedi_get_n_ranges(it, subdev_dio, i);
		fprintf(fout, "Ranges %i \r\n", ranges_dio);
	}

	if (PWM_OPEN) {
		fprintf(fout, "Subdev COU %i ", subdev_counter);
		channels_counter = comedi_get_n_channels(it, subdev_counter);
		fprintf(fout, "Digital Channels %i ", channels_counter);
		maxdata_counter = comedi_get_maxdata(it, subdev_counter, i);
		fprintf(fout, "Maxdata %i ", maxdata_counter);
		ranges_counter = comedi_get_n_ranges(it, subdev_counter, i);
		fprintf(fout, "Ranges %i \r\n", ranges_counter);
	}

	if (SERIAL_OPEN) {
		fprintf(fout, "Subdev SER %i ", subdev_serial0);
		channels_serial0 = comedi_get_n_channels(it, subdev_serial0);
		fprintf(fout, "Digital Channels %i ", channels_serial0);
		maxdata_serial0 = comedi_get_maxdata(it, subdev_serial0, i);
		fprintf(fout, "Maxdata %i ", maxdata_serial0);
		ranges_serial0 = comedi_get_n_ranges(it, subdev_serial0, i);
		fprintf(fout, "Ranges %i \r\n", ranges_serial0);
	}
	return 0;
}

int get_data_sample(void)
{
	if (DI_OPEN) {
		if (bmc.BOARD == bmcboard) {
			comedi_dio_bitfield2(it, subdev_di, 0x00, &datain, 0x00);
		} else {
			comedi_dio_bitfield2(it, subdev_di, 0x00, &datain, 0x00);
		}
	}

	if (DO_OPEN) {
		uint32_t tmp_data = (~datain & 0x3fffff);

		// send I/O as a byte mask
		if (bmc.BOARD == bmcboard) {
				if (tmp_data == B_NONE) {
					obits.bytes[0] = bmc.dataout.bytes[0]; // buffer output
					obits.bytes[1] = ~bmc.dataout.bytes[0];
					obits.bytes[2] = ~bmc.dataout.bytes[0];
					obits.bytes[3] = ~bmc.dataout.bytes[0];
				} else {
					if ((tmp_data == B_0) || (tmp_data == B_15)) {
						obits.bytes[0] = ~bmc.dataout.bytes[0]; // buffer output
						obits.bytes[1] = bmc.dataout.bytes[0];
						obits.bytes[2] = bmc.dataout.bytes[0];
						obits.bytes[3] = bmc.dataout.bytes[0];
					} else {
						obits.bytes[0] = bmc.dataout.bytes[0]; // buffer output
						obits.bytes[1] = bmc.dataout.bytes[0];
						obits.bytes[2] = bmc.dataout.bytes[0];
						obits.bytes[3] = bmc.dataout.bytes[0];
					}
				}
				comedi_dio_bitfield2(it, subdev_do, obits.dio_buf, &obits.dio_buf, 0);
			} else {
			obits.bytes[0] = bmc.dataout.bytes[0]; // buffer output
			comedi_dio_bitfield2(it, subdev_do, 0xff, &obits.dio_buf, 0);
		}
	}

	if (SERIAL_OPEN) {
#ifdef SEND_TEXT
		uint32_t slow_text = 0;

		if (++slow_text > SLOW_TEXT) {
			slow_text = 0;
			serial_buf = daq_text_ptr[daq_text_index++];
			if (daq_text_index > MAX_STRLEN) {
				comedi_data_write(it, subdev_serial0, 0, range_ao, AREF_GROUND, STX);
				daq_text_index = 0;
				serial_buf = 0;
				line_index++;
				daq_text_ptr = daq_text[line_index & 0x03];
			} else {
				comedi_data_write(it, subdev_serial0, line_index & 0x03, range_ao, AREF_GROUND, serial_buf);
				comedi_data_read(it, subdev_serial0, line_index & 0x03, range_ao, AREF_GROUND, &serial_buf);
			}
		}
#endif
		if (BMC4.bmc_flag) {
			if (++slow_data > SLOW_DATA) {
				slow_data = 0;

				if ((daq_data_index > MAX_STRLEN) || (daq_bmc_data_text[BMC4.pos] == '^')) {
					comedi_data_write(it, subdev_serial0, 4, range_ao, AREF_GROUND, STX); // update daq_bmc data buffer
					daq_data_index = 0;
					strncpy(daq_bmc_data_buf, daq_bmc_data_text, SYSLOG_SIZ);
					BMC4.pos = 0;
					BMC4.bmc_flag = false;
				} else {
					comedi_data_read(it, subdev_serial0, 4, range_ao, AREF_GROUND, &daq_bmc_data[BMC4.pos]);
					daq_bmc_data_text[BMC4.pos] = (char) (daq_bmc_data[BMC4.pos] >> 8);
					serial_buf = daq_bmc_data_text[BMC4.pos];
					BMC4.pos++;
					daq_data_index++;
				}
			}
		}
	}
	return 0;
}

double lp_filter(double new, int bn, int slow) // low pass filter, slow rate of change for new, LPCHANC channels, slow/fast select (-1) to zero channel
{
	static double smooth[LPCHANC] = {0};
	double lp_speed, lp_x;

	if ((bn >= LPCHANC) || (bn < 0)) // check for proper array position
		return new;
	if (slow) {
		lp_speed = 0.033;
	} else {
		lp_speed = 0.125;
	}
	lp_x = ((smooth[bn]*100.0) + (((new * 100.0)-(smooth[bn]*100.0)) * lp_speed)) / 100.0;
	smooth[bn] = lp_x;
	if (slow == (-1)) { // reset and return zero
		lp_x = 0.0;
		smooth[bn] = 0.0;
	}
	return lp_x;
}

/*
 * cleanup mainly noise values near zero value
 */
double calc_fixups(double data, FIX_CODES fixup)
{
	if ((fixup & SKIP))
		return data;

	if (fixup & WIDE_ZERO) {
		if ((data < 0.05) && (data >-0.05))
			data = 0.0;
	}
	if (fixup & NO_NEG) {
		if (data < 0.0)
			data = 0.0;
	}
	if (fixup & POS_ONLY) {
		data = fabs(data);
	}
	return data;
}
