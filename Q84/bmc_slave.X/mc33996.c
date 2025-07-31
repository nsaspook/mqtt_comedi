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
	.out = 0x0000,
};

mc33996init_type mc_init;

uint8_t mc33996_w_buf[8];

void mc33996_version(void)
{
	printf("\r--- MC33996 Driver Version %s %s %s ---\r\n", MC33996_DRIVER, build_date, build_time);
}

bool mc33996_init(void)
{
	mc_init.cmd[0] = 0x19;
	mc_init.cmd[1] = 0x57;
	mc_init.cmd[2] = 0x07;
	mc_init.cmd[3] = 0x00;
	mc_init.cmd[4] = 0xff;
	mc_init.cmd[5] = 0xff;

	send_spi1_mc33996_dma((void*) mc_init.cmd, 6); // powerup SPI Integrity Check
	if (mc_init.cmd[3] != 0x19 || mc_init.cmd[4] != 0x57 || mc_init.cmd[5] != 0x07) {
		return false;
	}

	send_spi1_mc33996_dma((void*) &mc_pwm, 3); // PWM
	send_spi1_mc33996_dma((void*) &mc_onoff, 3); // ALL I/O off
	send_spi1_mc33996_dma((void*) &mc_gsrc, 3); // ALL I/O off
	send_spi1_mc33996_dma((void*) &mc_sfpd, 3); // thermal only
	send_spi1_mc33996_dma((void*) &mc_olce, 3); // open load
	return true;
};

void mc33996_update(const uint16_t data)
{
	mc_onoff.out = (uint16_t) (((data & 0xff00) >> 8) & 0x00ff); // byte order swap
	mc_onoff.out |= (uint16_t) (((data & 0x00ff) << 8) & 0xff00);
	send_spi1_mc33996_dma((void*) &mc_onoff.cmd, 3);
};