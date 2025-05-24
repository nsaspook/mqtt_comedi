/** \file tic12400.c
 * TC12400 driver for Q84 v0.1
 * uses SPI5 mode1 at 4MHz no interrupts
 * external interrupt 2 is used to detect chip switch events
 */

#include "tic12400.h"

/*
 * command structure data
 * the parity bit must be set correctly for the command to execute on the chip
 */
const ticbuf_type setup32 = {
	.data3 = 0xe4,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x01,
};
const ticbuf_type setup21 = {
	.data3 = 0xc2,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x00,
};
const ticbuf_type setup1c = {
	.data3 = 0xb8,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x01,
};
const ticbuf_type setup1b = {
	.data3 = 0xb7,
	.data2 = 0xff,
	.data1 = 0xff,
	.data0 = 0xfe,
};
const ticbuf_type setup1a = {
	.data3 = 0xb4,
	.data2 = 0x01,
	.data1 = 0x80,
	.data0 = 0x01,
};
const ticbuf_type setup1a_trigger = {
	.data3 = 0xb4,
	.data2 = 0x00,
	.data1 = 0x14,
	.data0 = 0x01,
};
const ticbuf_type setup22 = {
	.data3 = 0xc5,
	.data2 = 0xff,
	.data1 = 0xff,
	.data0 = 0xfe,
};
const ticbuf_type setup23 = {
	.data3 = 0xc7,
	.data2 = 0xff,
	.data1 = 0xff,
	.data0 = 0xff,
};
const ticbuf_type setup24 = {
	.data3 = 0xc8,
	.data2 = 0x00,
	.data1 = 0x1f,
	.data0 = 0xfe,
};
const ticbuf_type setup25 = {
	.data3 = 0xc9,
	.data2 = 0xff,
	.data1 = 0xff,
	.data0 = 0xff,
};
const ticbuf_type setup1d = {
	.data3 = 0xba,
	.data2 = 0x49,
	.data1 = 0x24,
	.data0 = 0x92,
};
const ticbuf_type ticread05 = {
	.data3 = 0x0a,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x01,
};
const ticbuf_type ticdevid01 = {
	.data3 = 0x02,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x00,
};
const ticbuf_type ticstat02 = {
	.data3 = 0x04,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x02,
};
const ticbuf_type ticreset1a = {
	.data3 = 0xb4,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x02,
};

/*
 * global status and value registers
 */
volatile uint32_t tic12400_status = 0, tic12400_counts = 0, tic12400_value_counts = 0, tic12400_id = 0;
volatile uint32_t tic12400_value = 0, tic12400_switch, ticvalue = 0, tic12400_fail_count, tic12400_parity_count;
volatile bool tic12400_init_fail = true, tic12400_event = false; // chip error detection flag
volatile bool tic12400_parity_status = false, tic12400_read_error = false;
volatile int32_t tic12400_fail_value = 0;

static const char *build_date = __DATE__, *build_time = __TIME__;

static void tic_int_handler(void);

void tic12400_version(void)
{
	printf("\r--- TIC12400 Driver Version %s %s %s ---\r\n", TIC12400_DRIVER, build_date, build_time);
}
/*
 * software reset of the chip using SPI
 * all registers set to their default values
 */

/*
 * chip setup via SPI data transfers
 */
void tic12400_reset(void)
{
	tic12400_wr(&ticreset1a, 4);
	tic12400_wr(&ticreset1a, 4);
	tic12400_fail_value = 1;
}

/*
 * chip detection and configuration for all inputs with interrupts for
 * switch state changes with debounce
 * returns false on configuration failure
 */

/*
 * chip setup via SPI data transfers
 */
bool tic12400_init(void)
{
	if (tic12400_init_fail) {
		tic12400_status = tic12400_wr(&ticstat02, 0); // get status to check for proper operation
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_reset();
			tic12400_fail_value = -1;
			tic12400_fail_count++;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup32, 0); //all set to compare mode, 0x32
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -2;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup21, 0); //Compare threshold all bits 2V, 0x21
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -3;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup1c, 0); //all set to GND switch type, 0x1c
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -4;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup1b, 0); //all channels are enabled, 0x1b
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -5;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup22, 0); //set switch interrupts, 0x22
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -6;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup23, 0); //set switch interrupts, 0x23
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -7;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup24, 0); // enable interrupts, 0x24
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -8;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup25, 0); // enable interrupts, 0x25
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -9;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup1d, 0); // set wetting currents, 0x1d
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -10;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup1a, 0); // set switch debounce to max 4 counts, 0x1a
		if (((tic12400_status & por_bit_s))) { // check for any high bits beyond POR bits set
			tic12400_fail_count++;
			tic12400_fail_value = -11;
			goto fail;
		}
		tic12400_status = tic12400_wr(&setup1a_trigger, 2); // trigger switch detections & CRC, 0x1a
		if (tic12400_status & spi_fail_bit_v) {
			tic12400_fail_value = -12;
			tic12400_fail_count++;
			goto fail;
		}
		tic12400_id = tic12400_wr(&ticdevid01, 0);
		if (tic12400_id & spi_fail_bit_v) {
			tic12400_fail_value = -13;
			tic12400_fail_count++;
			goto fail;
		}
		IOCBF6_SetInterruptHandler(tic_int_handler);
		tic12400_init_fail = false;
		tic12400_fail_value = 0;
	}
fail:
	return !tic12400_init_fail; // flip to return true if NO configuration failures
}

/*
 * send tic12400 commands to SPI port 1 with possible delay after transfer
 * returns 32-bit spi response from the tic12400
 */
uint32_t tic12400_wr(const ticbuf_type * buffer, uint16_t del)
{
	send_spi1_tic12400_dma((void*) buffer, 4);
	ticvalue = (uint32_t) spi_link.rxbuf[0]&0x000000ff;
	ticvalue += (uint32_t) (spi_link.rxbuf[1] << 8) & 0x0000ff00;
	ticvalue += (uint32_t) ((uint32_t) spi_link.rxbuf[2] << (uint32_t) 16)&0x00ff0000;
	ticvalue += (uint32_t) ((uint32_t) spi_link.rxbuf[3] << (uint32_t) 24)&0xff000000;
	tic12400_read_error = false;

	if (ticvalue & parity_fail_v) { // check for command parity errors
		tic12400_parity_status = true;
		tic12400_read_error = true;
		tic12400_parity_count++;
		send_spi1_tic12400_dma((void*) &ticstat02, 4);
	};
	if (ticvalue & spi_fail_bit_v) {
		tic12400_read_error = true;
		tic12400_fail_count++;
		send_spi1_tic12400_dma((void*) &ticstat02, 4);
	}
	if (del) {
		WaitMs(1);
	}
	return(uint32_t) ticvalue; // raw 32-bit switch register value from the device SO pins
}

/*
 * switch data reading testing routine
 * tic12400 value is updated in external interrupt #2 ISR
 */
uint32_t tic12400_get_sw(void)
{
	if (tic12400_init_fail) { // Trouble in River City
		return 0;
	}
	if (tic12400_switch & (raw_mask_0)) {
		//		BSP_LED1_Clear();
	} else {
		//		BSP_LED1_Set();
	}
	if (tic12400_switch & (raw_mask_11)) {
		//		BSP_LED2_Clear();
	} else {
		//		BSP_LED2_Set();
	}
	tic12400_event = false;
	return tic12400_switch;
}

/*
 * 32-bit 1's parity check
 * https://graphics.stanford.edu/~seander/bithacks.html#ParityNaive
 */
bool tic12400_parity(uint32_t v)
{
	v ^= v >> 16;
	v ^= v >> 8;
	v ^= v >> 4;
	v &= 0xf;
	return(0x6996 >> v) & 1;
}

/*
 * switch SPI status and switch data updates
 * sets event flag for user application notification
 */
void tic12400_read_sw(uint32_t a, uintptr_t b)
{
	tic12400_value = tic12400_wr(&ticread05, 0); // read switch

	if ((tic12400_value & ssc_bit_s) && tic12400_parity(tic12400_value)) { // only trigger on switch state change
		tic12400_event = true;
		tic12400_value_counts++;
	}
	tic12400_switch = (tic12400_value >> 1) & switch_mask_d;
	tic12400_counts++;
}

void tic_int_handler(void)
{
	tic12400_read_sw(0, (uintptr_t) NULL);
}