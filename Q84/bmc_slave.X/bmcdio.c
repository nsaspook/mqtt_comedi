
#include <pic18f47q84.h>

#include "bmcdio.h"
#include "eadog.h"

static wchar_t wstr[MAX_STR]; // buffer for id settings strings from MPC2210

static mcp2210_spi_type SPI_buffer; //  MCP2210 I/O structure
mcp2210_spi_type *S = &SPI_buffer; // working I/O structure pointer
static const char *build_date = __DATE__, *build_time = __TIME__;

/*
 * zero rx/tx buffers
 */
void cbufs(void)
{
	memset(S->buf, 0, sizeof(S->buf)); // initialize bufs to zeros
	memset(S->rbuf, 0, sizeof(S->rbuf));
}

int32_t get_usb_res(void)
{
	return S->res;
}

int nanosleep(const struct timespec *, struct timespec *);

void sleep_us(const uint32_t microseconds)
{
	struct timespec ts;
	if (!microseconds) {
		return;
	}
	ts.tv_sec = microseconds / 1000000; // whole seconds
	ts.tv_nsec = (microseconds % 1000000) * 1000; // remainder, in nanoseconds
	nanosleep(&ts, NULL);
}

/*
 * when connected to the TIC12400 interrupt pin it shows a switch has changed state
 */
bool get_MCP2210_ext_interrupt(void)
{
#ifdef EXT_INT_DPRINT
	static uint32_t counts = 0;
#endif

	cbufs();
	S->buf[0] = 0x12; // Get (VM) the Current Number of Events From the Interrupt Pin, GPIO 6 FUNC2
	S->buf[1] = 0x00; // reads, then resets the event counter
	//	S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
	if (S->rbuf[4] || S->rbuf[5]) {
#ifdef EXT_INT_DPRINT
		printf("\r\nrbuf4 %x: rbuf5 %x: counts %i\n", S->rbuf[4], S->rbuf[5], ++counts);
#endif
		return true;
	}
	return false;
}

int32_t cancel_spi_transfer(void)
{
	cbufs();
	S->buf[0] = 0x11; // 0x11 cancel SPI transfer
	//	S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
	return S->res;
}

bool SPI_WriteRead(uint8_t *buf, uint8_t *rbuf)
{
	//	S->res = SendUSBCmd(handle, buf, rbuf);
	while (rbuf[3] == SPI_STATUS_STARTED_NO_DATA_TO_RECEIVE || rbuf[3] == SPI_STATUS_SUCCESSFUL) {
		//		S->res = SendUSBCmd(handle, buf, rbuf);
	}
	return true;
}

bool SPI_MCP2210_WriteRead(uint8_t* pTransmitData, const size_t txSize, uint8_t* pReceiveData, const size_t rxSize)
{
#ifdef DPRINT
	static uint32_t tx_count = 0;
#endif

	cbufs();
	S->buf[0] = 0x42; // transfer SPI data command
	S->buf[1] = (uint8_t) rxSize; // no. of SPI bytes to transfer
	S->buf[4] = pTransmitData[3];
	S->buf[5] = pTransmitData[2];
	S->buf[6] = pTransmitData[1];
	S->buf[7] = pTransmitData[0];
	//	S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
#ifdef DPRINT
	printf("TX SPI res %x - tx count %i\n", S->res, ++tx_count);
#endif

#ifdef DPRINT
	uint32_t rcount = 0;
#endif
	while (S->rbuf[3] == SPI_STATUS_STARTED_NO_DATA_TO_RECEIVE || S->rbuf[3] == SPI_STATUS_SUCCESSFUL) {
#ifdef DPRINT
		printf("SPI RX wait %i: code %x\n", ++rcount, S->rbuf[3]);
#endif
		//		S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
	}
	pReceiveData[3] = S->rbuf[4];
	pReceiveData[2] = S->rbuf[5];
	pReceiveData[1] = S->rbuf[6];
	pReceiveData[0] = S->rbuf[7];
	return true;
}

/*
 * open the USB device with a optional serial number
 */
mcp2210_spi_type* hidrawapi_mcp2210_init(const wchar_t *serial_number)
{
	printf("\r\n--- BMCDIO Driver Version %s %s %s ---\r\n", BMCDIO_DRIVER, build_date, build_time);
	cbufs(); // clear command and response buffers
	printf("BMCDIO init complete\n"); // ctrl c to exit program
	return S;
}

void setup_tic12400_transfer(void)
{
	cbufs();
	S->buf[0] = 0x40; // SPI transfer settings command
	S->buf[4] = 0x00; // set SPI transfer bit rate;
	S->buf[5] = 0x09; // 32 bits, lsb = buf[4], msb buf[7]
	S->buf[6] = 0x3d; // 4MHz
	S->buf[7] = 0x00;
	S->buf[8] = 0xff; // set CS idle values to 1
	S->buf[9] = 0x01;
	S->buf[10] = 0b11011111; // set CS active values to 0, set the rest to 1
	S->buf[11] = 0b11111111;
	S->buf[18] = 0x4; // set no of bytes to transfer = 4 // 32-bit transfers
	S->buf[20] = 0x01; // spi mode 1
	//	S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
}

void get_tic12400_transfer(void)
{
	// ---------- Get SPI transfer settings (0x41)-------------
	cbufs();
	S->buf[0] = 0x41; // 0x41 Get SPI transfer settings
	//	S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
	printf("SPI TIC12400 transfer settings\n   "); // Print out the 0x41 returned buffer.
	for (int i = 0; i < S->rbuf[2]; i++) {
		printf("%02hhx ", S->rbuf[i]);
	}
	printf("\n");
}

/*
 * config the SPI device outputs to a default condition
 */
void mc33996_init(void)
{
	cbufs(); // clear the RX/TX buffer
	// MCP33996 config
	S->buf[0] = 0x42; // transfer SPI data command
	S->buf[1] = 3; // no. of SPI bytes to transfer
	S->buf[4] = 0x00; // on/off control
	S->buf[5] = 0x0f; // set all outputs to low
	S->buf[6] = 0xf0; // ""
	//	S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
};

/*
 * update the SPI device outputs
 */
void mc33996_set(uint8_t cmd, uint8_t data_h, uint8_t data_l)
{
	cbufs(); // clear the RX/TX buffer
	// MCP33996 config
	S->buf[0] = 0x42; // transfer SPI data command
	S->buf[1] = 6; // no. of SPI bytes to transfer
	S->buf[4] = cmd; // device command control
	S->buf[5] = data_h; // set all outputs
	S->buf[6] = data_l; // ""
	//	S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
};

/*
 * check for correct 48-bit data from mc33996 after command reset
 */
bool mc33996_check(void)
{
	if (S->rbuf[7] == mc33996_reset && S->rbuf[8] == mc33996_magic_h && S->rbuf[9] == mc33996_magic_l) {
		return true;
	} else {
		return false;
	}
};

/*
 * config the USB to SPI parameters for the slave device
 */
void setup_mc33996_transfer(uint8_t len)
{
	cbufs();
	S->buf[0] = 0x40; // SPI transfer settings command
	S->buf[4] = 0x00; // set SPI transfer bit rate;
	S->buf[5] = 0x09; // 32 bits, lsb = buf[4], msb buf[7]
	S->buf[6] = 0x3d; // 4MHz
	S->buf[7] = 0x00; // ""
	S->buf[8] = 0xff; // set CS idle values to 1
	S->buf[9] = 0x01; // ""
	S->buf[10] = 0b01111111; // set CS active values to 0, set the rest to 1
	S->buf[11] = 0b11111111; // ""
	S->buf[18] = len; // set no of bytes to transfer = 3
	S->buf[20] = 0x01; // spi mode 1
	//	S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
};

/*
 * display to stdio the USB to SPI transfer parameters
 */
void get_mc33996_transfer(void)
{
	cbufs();
	S->buf[0] = 0x41; // 0x41 Get SPI transfer settings
	//	S->res = SendUSBCmd(S->handle, S->buf, S->rbuf);
	printf("SPI MC33996 transfer settings\n   "); // Print out the 0x41 returned buffer.
	for (int i = 0; i < S->rbuf[2]; i++) {
		printf("%02hhx ", S->rbuf[i]);
	}
	printf("\n");
};

/*
 * setup SPI command sequence
 */
void mc33996_update(void)
{
	S->buf[4] = mc33996_control; // set MC33996 outputs command
};

void SPI_EADOG(void)
{
	wait_lcd_done();
	MCZ_CS_SetHigh();
	TIC_CS_SetHigh();
	SPI1CON0bits.EN = 0;
	//SMP Middle; CKE Idle to active; CKP Idle:High, Active:Low; FST disabled; SSP active low; SDIP active high; SDOP active high; 
	SPI1CON1 = 0x24;
	SPI1CON2 = 0x02; //  Received data is not stored in the FIFO
	//CLKSEL MFINTOSC; 
	SPI1CLK = 0x02;
	//BAUD 4;  50KHz
	SPI1BAUD = 0x04;
	SPI1CON0 = 0x83;
	SPI1CON0bits.EN = 1;
}

void SPI_TIC12400(void)
{
	wait_lcd_done();
	MCZ_CS_SetHigh();
	TIC_CS_SetHigh();
	CS_SetHigh();
	SPI1STATUSbits.CLRBF = 1;
	SPI1CON0bits.EN = 0;
	//EN enabled; LSBF MSb first; MST bus master; BMODE every byte; 
	SPI1CON0 = 0x03;
	//SMP Middle; CKE Idle to active; CKP Idle:Low, Active:High; FST disabled; SSP active high; SDIP active high; SDOP active high; 
	SPI1CON1bits.CKP = 0;
	SPI1CON1bits.CKE = 1;
	SPI1CON1bits.SMP = 1;
	SPI1CON1bits.FST = 0;
	SPI1CON1bits.SDIP = 0;
	SPI1CON1bits.SDOP = 0;
	SPI1CON2 = 0x03; //  Received data is not stored in the FIFO for DMA mode
	//CLKSEL FOSC; 
	SPI1CLK = 0x00;
	//BAUD 7;  4MHz
	SPI1BAUD = 0x07;
	SPI1CON0bits.EN = 1;
	SPI1STATUSbits.CLRBF = 0;
}

void SPI_MC33996(void)
{
	wait_lcd_done();
	MCZ_CS_SetHigh();
	TIC_CS_SetHigh();
	CS_SetHigh();
	SPI1STATUSbits.CLRBF = 1;
	SPI1CON0bits.EN = 0;
	//EN enabled; LSBF MSb first; MST bus master; BMODE every byte; 
	SPI1CON0 = 0x03;
	//SMP Middle; CKE Idle to active; CKP Idle:Low, Active:High; FST disabled; SSP active high; SDIP active high; SDOP active high; 
	SPI1CON1bits.CKP = 0;
	SPI1CON1bits.CKE = 1;
	SPI1CON1bits.SMP = 1;
	SPI1CON1bits.FST = 0;
	SPI1CON1bits.SDIP = 0;
	SPI1CON1bits.SDOP = 0;
	SPI1CON2 = 0x03; //  Received data is not stored in the FIFO for DMA mode
	//CLKSEL FOSC; 
	SPI1CLK = 0x00;
	//BAUD 7;  4MHz
	SPI1BAUD = 0x07;
	SPI1CON0bits.EN = 1;
	SPI1STATUSbits.CLRBF = 0;
}