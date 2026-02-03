/** \file bmc.c
 * Demo code for driver testing, a simple console display of data inputs and voltage
 *
 * This file may be freely modified, distributed, and combined with
 * other software, as long as proper attribution is given in the
 * source code.
 */

#include <stdlib.h>
#include <stdio.h> /* for printf() */
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <comedilib.h>
#include "daq.h"
#include "bmc.h"
#include "bmc_mqtt.h"

volatile struct bmcdata bmc = {
	.BOARD = bmcboard,
	.BNAME = "BMCBoard",
}; /* DAQ buffer */

struct energy_type E = {
	.once_gti = true,
	.once_ac = true,
	.once_gti_zero = true,
	.iammeter = false,
	.fm80 = false,
	.dumpload = false,
	.homeassistant = false,
	.ac_low_adj = 0.0f,
	.gti_low_adj = 0.0f,
	.ac_sw_on = true,
	.gti_sw_on = true,
	.im_delay = 0,
	.gti_delay = 0,
	.im_display = 0,
	.rc = 0,
	.speed_go = 0,
	.ac_sw_status = false,
	.gti_sw_status = false,
	.solar_mode = false,
	.solar_shutdown = false,
	.startup = true,
	.ac_mismatch = false,
	.dc_mismatch = false,
	.mode_mismatch = false,
	.dl_excess = false,
	.dl_excess_adj = 0.0f,
};


// Comedi I/O device type
const char *board_name = "NO_BOARD", *driver_name = "NO_DRIVER";

FILE *fout, *calfile; // logging stream and calibration data

int usleep(uint32_t); // for C11

/* ripped from http://aquaticus.info/pwm-sine-wave */

const uint8_t sine_wave[256] = {
	0x80, 0x83, 0x86, 0x89, 0x8C, 0x90, 0x93, 0x96,
	0x99, 0x9C, 0x9F, 0xA2, 0xA5, 0xA8, 0xAB, 0xAE,
	0xB1, 0xB3, 0xB6, 0xB9, 0xBC, 0xBF, 0xC1, 0xC4,
	0xC7, 0xC9, 0xCC, 0xCE, 0xD1, 0xD3, 0xD5, 0xD8,
	0xDA, 0xDC, 0xDE, 0xE0, 0xE2, 0xE4, 0xE6, 0xE8,
	0xEA, 0xEB, 0xED, 0xEF, 0xF0, 0xF1, 0xF3, 0xF4,
	0xF5, 0xF6, 0xF8, 0xF9, 0xFA, 0xFA, 0xFB, 0xFC,
	0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFE, 0xFD,
	0xFD, 0xFC, 0xFB, 0xFA, 0xFA, 0xF9, 0xF8, 0xF6,
	0xF5, 0xF4, 0xF3, 0xF1, 0xF0, 0xEF, 0xED, 0xEB,
	0xEA, 0xE8, 0xE6, 0xE4, 0xE2, 0xE0, 0xDE, 0xDC,
	0xDA, 0xD8, 0xD5, 0xD3, 0xD1, 0xCE, 0xCC, 0xC9,
	0xC7, 0xC4, 0xC1, 0xBF, 0xBC, 0xB9, 0xB6, 0xB3,
	0xB1, 0xAE, 0xAB, 0xA8, 0xA5, 0xA2, 0x9F, 0x9C,
	0x99, 0x96, 0x93, 0x90, 0x8C, 0x89, 0x86, 0x83,
	0x80, 0x7D, 0x7A, 0x77, 0x74, 0x70, 0x6D, 0x6A,
	0x67, 0x64, 0x61, 0x5E, 0x5B, 0x58, 0x55, 0x52,
	0x4F, 0x4D, 0x4A, 0x47, 0x44, 0x41, 0x3F, 0x3C,
	0x39, 0x37, 0x34, 0x32, 0x2F, 0x2D, 0x2B, 0x28,
	0x26, 0x24, 0x22, 0x20, 0x1E, 0x1C, 0x1A, 0x18,
	0x16, 0x15, 0x13, 0x11, 0x10, 0x0F, 0x0D, 0x0C,
	0x0B, 0x0A, 0x08, 0x07, 0x06, 0x06, 0x05, 0x04,
	0x03, 0x03, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x03,
	0x03, 0x04, 0x05, 0x06, 0x06, 0x07, 0x08, 0x0A,
	0x0B, 0x0C, 0x0D, 0x0F, 0x10, 0x11, 0x13, 0x15,
	0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20, 0x22, 0x24,
	0x26, 0x28, 0x2B, 0x2D, 0x2F, 0x32, 0x34, 0x37,
	0x39, 0x3C, 0x3F, 0x41, 0x44, 0x47, 0x4A, 0x4D,
	0x4F, 0x52, 0x55, 0x58, 0x5B, 0x5E, 0x61, 0x64,
	0x67, 0x6A, 0x6D, 0x70, 0x74, 0x77, 0x7A, 0x7D
};

void led_lightshow(int speed)
{
	static int j = 0;
	static uint8_t cylon = 0xff;
	static int alive_led = 0;
	static bool LED_UP = true;

	if (j++ >= speed) { // delay a bit ok
		if (0) { // screen status feedback
			bmc.dataout.dio_buf = ~cylon; // roll leds cylon style
		} else {
			bmc.dataout.dio_buf = cylon; // roll leds cylon style (inverted)
		}

		if (LED_UP && (alive_led != 0)) {
			alive_led = alive_led * 2;
			cylon = cylon << 1;
		} else {
			if (alive_led != 0) alive_led = alive_led / 2;
			cylon = cylon >> 1;
		}
		if (alive_led < 2) {
			alive_led = 2;
			LED_UP = true;
		} else {
			if (alive_led > 128) {
				alive_led = 128;
				LED_UP = false;
			}
		}
		j = 0;
	}
}

void relay_set(uint16_t relays)
{
	bmc.dataout.dio_buf = relays;
}

bool get_set_config(void)
{
	static const char *output_file = "/etc/daq_bmc/bmc_config.cfg"; // will only read data from here
	static const char *output_file_tmp = "/tmp/bmc_config.cfg"; // if the correct directory is missing
	config_t cfg;
	config_setting_t *setting, *group;
	bool config_status = false;

	/*
	 * read configuration file settings
	 */
	config_init(&cfg);
	/* Read the file. If there is an error, create a new file */
	if (!config_read_file(&cfg, output_file)) {
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&cfg),
			config_error_line(&cfg), config_error_text(&cfg));
		config_destroy(&cfg);

		config_init(&cfg);
		group = config_root_setting(&cfg);
		setting = config_setting_add(group, "BENERGYV", CONFIG_TYPE_FLOAT);
		config_setting_set_float(setting, S.BENERGYV);
		setting = config_setting_add(group, "BVOLTAGEV", CONFIG_TYPE_FLOAT);
		config_setting_set_float(setting, S.BVOLTAGEV);
		setting = config_setting_add(group, "PVENERGYV", CONFIG_TYPE_FLOAT);
		config_setting_set_float(setting, S.PVENERGYV);
		setting = config_setting_add(group, "PVVOLTAGEV", CONFIG_TYPE_FLOAT);
		config_setting_set_float(setting, S.PVVOLTAGEV);
		setting = config_setting_add(group, "SOC_MODEV", CONFIG_TYPE_FLOAT);
		config_setting_set_float(setting, S.SOC_MODEV);

		/* Write out the new configuration. */
		if (!config_write_file(&cfg, output_file)) {
			if (!config_write_file(&cfg, output_file_tmp)) {
				fprintf(stderr, "Error while writing file to %s and %s.\n", output_file, output_file_tmp);
#ifdef EXIT_CONFIG_WRITE_FAIL
				config_destroy(&cfg);
				return(EXIT_FAILURE);
#endif
			} else {
				fprintf(stderr, "Testing configuration successfully written to: %s\n",
					output_file_tmp);
			}
		} else {
			fprintf(stderr, "New configuration successfully written to: %s\n",
				output_file);
			config_status = true;
		}
		config_destroy(&cfg);
	} else {

		if (config_lookup_float(&cfg, "BENERGYV", &S.BENERGYV)
			&& config_lookup_float(&cfg, "BVOLTAGEV", &S.BVOLTAGEV)
			&& config_lookup_float(&cfg, "PVENERGYV", &S.PVENERGYV)
			&& config_lookup_float(&cfg, "PVVOLTAGEV", &S.PVVOLTAGEV)
			&& config_lookup_float(&cfg, "SOC_MODEV", &S.SOC_MODEV)) {
			fprintf(stderr, "Configuration successfully read %4.1f %4.1f %4.1f %4.1f %4.1f from: %s\n",
				S.BENERGYV, S.BVOLTAGEV, S.PVENERGYV, S.PVVOLTAGEV, S.SOC_MODEV, output_file);
			config_status = true;
		} else {
			fprintf(stderr, "No/Incorrect settings in configuration file.\n");
		}
		config_destroy(&cfg);
	}
	return config_status;
}

int main(int argc, char *argv[])
{
	int do_ao_only = false;
	uint32_t i = 0, j = 75;

	/*
	 * read configuration file settings
	 */
	get_set_config();

	/*
	 * start the MQTT processing
	 */
	bmc_mqtt_init();

	if (do_ao_only) {
		if (init_dac(0.0, 25.0, false) < 0) {
			fprintf(fout, "Missing Analog AO subdevice\n");
			return -1;
		}


		while (true) {
			set_dac_raw(0, sine_wave[255 - i++] << 4);
			set_dac_raw(1, sine_wave[255 - j++] << 4);
		}
	} else {
#ifndef DIGITAL_ONLY
		if (init_daq(0.0, 25.0, false) < 0) {
			fprintf(fout, "Missing Analog subdevice(s)\n");
			return -1;
		}
#endif
		if (init_dio() < 0) {
			fprintf(fout, "Missing Digital subdevice(s)\n");
		}

		E.dac[0] = 1.23f;
		E.dac[1] = 3.21f;

		E.do_16b = 0x01;
		E.di_16b = 0x10;

		fflush(fout);
		while (true) {
			usleep(MAIN_DELAY); // sample rate ~1 msec
			//			set_dac_raw(0, sine_wave[i++]);
			get_data_sample();

			if (bmc.BOARD == bmcboard) {
#ifdef USE_RELAYS
				relay_set(0b01010101);
#else
				led_lightshow(10);
#endif
			} else {
				if (ha_daq_host.hindex == 4) {
					led_lightshow(2);
				} else {
					led_lightshow(10);
				}
			}

			if (ha_flag_vars_ss.runner) { // timer or trigger from mqtt
				comedi_push_mqtt(); // send json formatted data to the mqtt server
				ha_flag_vars_ss.runner = false;
			}
		}

	}
	return 0;
}


