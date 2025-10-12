#include "mxcmd.h"

/** \file mxcmd.c
 * FM80 MATE serial protocol communications
 */
static volatile uint8_t data = 0x00, dcount = 0, dstart = 0, rdstart = 0;
static volatile uint16_t tbuf[FM_BUFFER + 1], rbuf[FM_BUFFER + 1];
static uint16_t *p_tbuf = (uint16_t*) tbuf, *p_rbuf = (uint16_t*) rbuf;
static volatile uint8_t pace = 0; // the charge controller doesn't like back to back bytes

void FM_restart(void)
{
	data = U1RXB;
	dcount = 0;
	dstart = 0;
	rdstart = 0;
	BM.ten_sec_flag = false;
}

/*
 * Check for TX transmission
 */
bool FM_tx_empty(void)
{
	if (dcount == 0) {
		return true;
	} else {
		return false;
	}
}

/*
 * after the tbuf has been loaded start the TX transfer
 */
uint8_t FM_tx(const uint16_t * data, const uint8_t count)
{
	if (dcount == 0) {
		memcpy((void *) tbuf, (const void *) data, (size_t) (count << 1)); // copy 16-bit values
		dstart = 0;
		dcount = count;
		BM.FM80_io = true;
	}
	return dstart;
}

/*
 * serial I/O ISR, TMR6 500us I/O sample rate
 * polls the required UART registers for 9-bit send and receive into 16-bit arrays
 */
void FM_io(void)
{
	if (pace++ > BUFFER_SPACING) {
		if (dcount-- > 0) {
			if (tbuf[dstart] > 0xff) { // Check for bit-9
				U2P1L = (uint8_t) tbuf[dstart]; // send with bit-9 high, start of packet
			} else {
				UART2_Write((uint8_t) tbuf[dstart]); // send with bit-9 low
			}
			dstart++;
		} else {
			dstart = 0;
			dcount = 0;
		}
		pace = 0;
	}

	/*
	 * handle framing errors
	 */
	if (U2ERRIRbits.RXFOIF) {
		rbuf[0] = U2RXB; // read bad data to clear error
		U2ERRIRbits.RXFOIF = 0;
		rdstart = 0; // reset buffer to start
	}

	/*
	 * read serial data if polled interrupt flag is set
	 */
	if (PIR8bits.U2RXIF) {
		if (U2ERRIRbits.FERIF) {
			// do nothing, will clear auto
		}

		if (rdstart > FM_BUFFER - 1) { // overload buffer index
			rdstart = 0; // reset buffer to start
		}
		if (U2ERRIRbits.PERIF) {
			rdstart = 0; // restart receive buffer when we see a 9-th bit high
			rbuf[rdstart] = 0x0100; // start of packet, bit 9 set
		} else {
			rbuf[rdstart] = 0x00;
		}
		rbuf[rdstart] += U2RXB;
		rdstart++;
	}

	timer_ms_tick(0, 0); // software timers update
}

/*
 * buffer received data
 * disabled using critical section interrupts here and it was too long 500us
 * and causing data errors
 */
uint8_t FM_rx(uint16_t * data)
{
	uint8_t count;

	count = rdstart;
	if (count > 0) {
		memcpy(data, (const void *) rbuf, (size_t) (count << 1)); // copy 16-bit values
	}
	rdstart = 0;
	return count;
}

bool FM_rx_ready(void)
{
	if (rdstart == 0) {
		return false;
	} else {
		return true;
	}
}

uint8_t FM_rx_count(void)
{
	uint8_t count;

	count = rdstart;
	return count;
}

void FM_onesec_io(void)
{
	BM.one_sec_flag = true;
}

void FM_tensec_io(void)
{
	BM.ten_sec_flag = true;
}

/*
 * floating point low pass filter,
 * slow/fast select, use (-1) to zero buffer channel and return new
 */
float lp_filter(const float new, const uint8_t bn, const int8_t slow)
{
	static float smooth[LP_BUFFER_SIZE];
	float lp_speed;

	if (bn >= LP_BUFFER_SIZE) // buffer index check
		return new;

	if (slow == (-1)) { // reset smooth buffer and return original value
		smooth[bn] = 0.0f;
		return new;
	}

	if (slow) { // some random filter cutoffs beta values
		lp_speed = 0.0333f;
	} else {
		lp_speed = 0.1f;
	}
	// exponentially weighted moving average
	return smooth[bn] = smooth[bn] + ((new - smooth[bn]) * lp_speed);
}

uint16_t calc_checksum(const uint8_t* data, const uint8_t len)
{
	uint16_t sum = 0;
	for (int i = 0; i < len; i++) {
		sum += data[i];
	}
	return sum;
}