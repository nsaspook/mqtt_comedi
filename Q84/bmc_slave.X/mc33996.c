/** \file mc33996.c
 */
#include "mc33996.h"

static const char *build_date = __DATE__, *build_time = __TIME__;

mc33996buf_type mc_onoff = {
	.cmd = mc33996_onoff,
	.out = 0x0000,
};

const mc33996buf_type mc_pwm = {
	.cmd = mc33996_pwm,
	.out = 0xffff,
};

const mc33996buf_type mc_reset = {
	.cmd = mc33996_reset,
	.out = 0x0000,
};

const mc33996buf_type mc_gsrc = {
	.cmd = mc33996_gsrc,
	.out = 0x0000,
};

const mc33996buf_type mc_sfpd = {
	.cmd = mc33996_sfpd,
	.out = 0xffff,
};

const mc33996buf_type mc_olce = {
	.cmd = mc33996_olce,
	.out = 0xffff,
};

void mc33996_version(void)
{
	printf("\r--- MC33996 Driver Version %s %s %s ---\r\n", MC33996_DRIVER, build_date, build_time);
}

bool mc33996_init(void)
{
	MCZ_PWM_SetLow();
	send_spi1_mc33996_dma((void*) &mc_pwm, 3); // NO PWM
	send_spi1_mc33996_dma((void*) &mc_onoff, 3); // ALL I/O off
	send_spi1_mc33996_dma((void*) &mc_gsrc, 3); // ALL I/O off
	send_spi1_mc33996_dma((void*) &mc_sfpd, 3); // thermal only
	send_spi1_mc33996_dma((void*) &mc_olce, 3); // open load
	if (spi_link.rxbuf[2] != 0xff) {
		return true;
	} else {
		return false;
	}
};

void mc33996_update(uint16_t data)
{
	mc_onoff.out = (uint16_t) ((data & 0xff00) >> 8); // byte order swap
	mc_onoff.out |= (uint16_t) ((data & 0x00ff) << 8);
	send_spi1_mc33996_dma((void*) &mc_onoff, 3);
};