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
const ticbuf_type setup32 = {// input trigger mode setup, comparator mode
	.data3 = 0x64 + write_bit,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x01,
};
const ticbuf_type setup21 = {// comparator threshold set to 2V
	.data3 = 0x42 + write_bit,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x00,
};
const ticbuf_type setup1c = {// Current Source (CSO) selected
	.data3 = 0x38 + write_bit,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x01,
};
const ticbuf_type setup1b = {// enable all inputs
	.data3 = 0x37 + write_bit,
	.data2 = 0x7f,
	.data1 = 0xff,
	.data0 = 0xff,
};
const ticbuf_type setup1a_trigger = {
	.data3 = 0x34 + write_bit, //  CONFIG Register, set trigger bit
	.data2 = 0x00,
	.data1 = 0x10,
	.data0 = 0x00,
};
const ticbuf_type setup1a_read = {
	.data3 = 0x34, // // Read global CONFIG Register
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x00,
};
const ticbuf_type setup22 = {// interrupt generation on falling and rising edge
	.data3 = 0x45 + write_bit,
	.data2 = 0x7f,
	.data1 = 0xff,
	.data0 = 0xff,
};
const ticbuf_type setup23 = {// interrupt generation on falling and rising edge
	.data3 = 0x47 + write_bit,
	.data2 = 0x7f,
	.data1 = 0xff,
	.data0 = 0xff,
};
const ticbuf_type setup24 = {// INT_EN_CFG0 Register, global interrupt bits
	.data3 = 0x48 + write_bit,
	.data2 = 0x00,
	.data1 = 0x80,
	.data0 = 0x00,
};
const ticbuf_type setup1d = {// WC_CFG0 Register, set contact wetting current
	.data3 = 0x3a + write_bit,
	.data2 = 0x7f,
	.data1 = 0xff,
	.data0 = 0xfe,
};
const ticbuf_type setup1e = {//  WC_CFG0 Register, set contact wetting current
	.data3 = 0x3c + write_bit,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x00,
};
const ticbuf_type ticread05 = {// IN_STAT_COMP Register, read switch status bits
	.data3 = 0x0a,
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x01,
};
const ticbuf_type ticdevid01 = {
	.data3 = 0x02, // DEVICE_ID register
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x01,
};
const ticbuf_type ticstat02 = {
	.data3 = 0x04, // INT_STAT Register, error and run bits
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x00,
};
const ticbuf_type ticreset1a = {
	.data3 = 0x34 + write_bit, // CONFIG Register, set reset bit
	.data2 = 0x00,
	.data1 = 0x00,
	.data0 = 0x02,
};

/*
 * global status and value registers
 */
volatile uint32_t tic12400_status = 0, tic12400_counts = 0, tic12400_value_counts = 0, tic12400_id = 0, tic12400_read_status;
volatile uint32_t tic12400_value = 1957, tic12400_switch, ticvalue = 0, tic12400_fail_count, tic12400_parity_count;
volatile bool tic12400_init_ok = false, tic12400_event = false; // chip error detection flag
volatile bool tic12400_parity_status = false, tic12400_read_error = false;
volatile int32_t tic12400_fail_value = 0;
volatile uint8_t b_read = 0;

ticrw_type tic_rw;

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
 * software resets to set power up defaults
 */
void tic12400_reset(void)
{
	send_spi1_tic12400_dma((void*) &ticreset1a, 4);
	WaitMs(10);
	send_spi1_tic12400_dma((void*) &ticreset1a, 4);
	WaitMs(10);
	tic12400_fail_value = 0;
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
	send_spi1_tic12400_dma((void*) &ticstat02, 4); // tic12400 status register read
	send_spi1_tic12400_dma((void*) &ticstat02, 4);
	if (((spi_link.rxbuf[0] & parity_fail_v))) { // check for any high bits beyond POR bits set
		send_spi1_tic12400_dma((void*) &ticreset1a, 4); // software reset
		WaitMs(10);
		tic12400_fail_value = -1;
		tic12400_fail_count++;
		tic12400_read_status = spi_link.rxbuf[0];
	}
	send_spi1_tic12400_dma((void*) &setup32, 4);
	if (((spi_link.rxbuf[0] & parity_fail_v))) {
		tic12400_fail_count++;
		tic12400_fail_value = -2;
		tic12400_read_status = spi_link.rxbuf[0];
	}
	send_spi1_tic12400_dma((void*) &setup21, 4);
	if (((spi_link.rxbuf[0] & parity_fail_v))) {
		tic12400_fail_count++;
		tic12400_fail_value = -3;
		tic12400_read_status = spi_link.rxbuf[0];
	}
	send_spi1_tic12400_dma((void*) &setup1c, 4);
	if (((spi_link.rxbuf[0] & parity_fail_v))) {
		tic12400_fail_count++;
		tic12400_fail_value = -4;
		tic12400_read_status = spi_link.rxbuf[0];
	}
	send_spi1_tic12400_dma((void*) &setup1b, 4);
	if (((spi_link.rxbuf[0] & parity_fail_v))) {
		tic12400_fail_count++;
		tic12400_fail_value = -5;
		tic12400_read_status = spi_link.rxbuf[0];
	}
	send_spi1_tic12400_dma((void*) &setup22, 4);
	if (((spi_link.rxbuf[0] & parity_fail_v))) {
		tic12400_fail_count++;
		tic12400_fail_value = -6;
		tic12400_read_status = spi_link.rxbuf[0];
	}
	send_spi1_tic12400_dma((void*) &setup23, 4);
	if (((spi_link.rxbuf[0] & parity_fail_v))) {
		tic12400_fail_count++;
		tic12400_fail_value = -7;
		tic12400_read_status = spi_link.rxbuf[0];
	}
	/*
	 * just poll for input status
	 */
	//	send_spi1_tic12400_dma((void*) &setup24, 4);
	//	if (((spi_link.rxbuf[0] & parity_fail_v))) {
	//		tic12400_fail_count++;
	//		tic12400_fail_value = -8;
	//		tic12400_read_status = spi_link.rxbuf[0];
	//	}
	//	send_spi1_tic12400_dma((void*) &setup1d, 4);
	//	if (((spi_link.rxbuf[0] & parity_fail_v))) {
	//		tic12400_fail_count++;
	//		tic12400_fail_value = -10;
	//		tic12400_read_status = spi_link.rxbuf[0];
	//	}

	send_spi1_tic12400_dma((void*) &setup1a_trigger, 4); // start monitoring and reporting switch inputs
	WaitMs(2);
	if (spi_link.rxbuf[0] & spi_fail_bit_v) {
		tic12400_fail_value = -12;
		tic12400_fail_count++;
		tic12400_read_status = spi_link.rxbuf[0];
	}
	tic12400_id = tic12400_wr(&ticdevid01, 0) >> 1;
	if (spi_link.rxbuf[0] & spi_fail_bit_v) {
		tic12400_fail_value = -13;
		tic12400_fail_count++;
		tic12400_read_status = spi_link.rxbuf[0];
	}

	IOCBF6_SetInterruptHandler(tic_int_handler);
	if (tic12400_fail_count > 1) {
		tic12400_init_ok = false;
		MLED_SetHigh();
	} else {
		tic12400_init_ok = true;
	}

	return tic12400_init_ok;
}

/*
 * send tic12400 commands to SPI port 1 with possible delay after transfer
 * returns 32-bit spi response from the tic12400
 * spi_link.rxbuf[0] is MSB (byte) in the 32-bit value
 */
uint32_t tic12400_wr(const ticbuf_type * buffer, uint16_t del)
{
	send_spi1_tic12400_dma((void*) buffer, 4);
	ticvalue = (uint32_t) spi_link.rxbuf[3]&0x000000ff;
	in_buf1 = spi_link.rxbuf[3];
	in_buf2 = spi_link.rxbuf[2];
	in_buf3 = spi_link.rxbuf[1];
	ticvalue += (uint32_t) (spi_link.rxbuf[2] << 8) & 0x0000ff00;
	ticvalue += (uint32_t) ((uint32_t) spi_link.rxbuf[1] << (uint32_t) 16)&0x00ff0000;
	ticvalue += (uint32_t) ((uint32_t) spi_link.rxbuf[0] << (uint32_t) 24)&0xff000000;
	tic12400_read_error = false;

	if (spi_link.rxbuf[0] & parity_fail_v) { // check for command parity errors
		tic12400_parity_status = true;
		tic12400_read_error = true;
		tic12400_parity_count++;
		send_spi1_tic12400_dma((void*) &ticstat02, 4);
	};
	if (spi_link.rxbuf[0] & spi_fail_bit_v) {
		tic12400_read_error = true;
		tic12400_fail_count++;
		send_spi1_tic12400_dma((void*) &ticstat02, 4);
	}
	if (del) {
		WaitMs(del);
	}
	return(uint32_t) ticvalue; // raw 32-bit switch register value from the device SO pins
}

/*
 * switch data reading testing routine
 * tic12400 value is updated in external interrupt #2 ISR
 */
uint32_t tic12400_get_sw(void)
{
	if (tic12400_init_ok == false) { // Trouble in River City
		return 0;
	}
	if (tic12400_switch & (raw_mask_0)) {
	} else {
	}
	if (tic12400_switch & (raw_mask_11)) {
	} else {
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

	if ((spi_link.rxbuf[0] & ssc_bit_s)) { // only trigger on switch state change
		tic12400_event = true;
		tic12400_value_counts++;
	}
	tic12400_switch = (tic12400_value >> 1) & switch_mask_d;
	tic12400_counts++;
}

void tic_int_handler(void)
{
	if (tic12400_init_ok) {
		tic12400_read_sw(0, (uintptr_t) NULL);
		MLED_SetHigh();
	}

	b_read = PORTB;
}