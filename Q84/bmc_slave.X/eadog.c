/** \file eadog.c
 */
#include <string.h>
#include "eadog.h"
#include "vconfig.h"
#include "mcc_generated_files/mcc.h"


#ifdef TRACE
#define E_TRACE	DEBUG1_Toggle()
#else
#define E_TRACE	""
#endif

#ifdef CAN_REMOTE // SPI DMA wait timeout counts
#define DONE_DELAY	99999
#else
#define DONE_DELAY	9999
#endif

volatile struct spi_link_type spi_link = {
	.LCD_DATA = false,
};

static const spi1_configuration_t spi1_configuration[] = {
	{0x83, 0x20, 0x3, 0x4, 0}
};

static volatile uint8_t inx = 0;
static char Sstr[NSB][LSB];
static volatile bool scroll_lock = false, powerup = true;
static volatile uint8_t scroll_line_pos = 4;
volatile uint8_t c0, c1, c2;

static void send_lcd_cmd_long(const uint8_t); // for display init only
static void send_lcd_data(const uint8_t);
static void send_lcd_cmd(const uint8_t);
static void spi_lcd_byte(void);

static void spi_src_byte(void);
static void spi_des_byte(void);
static void spi_or_byte(void);
static void wdtdelay(const uint32_t);

/*
 * Init the NHD-0420D3Z-NSW-BBW-V3 in 8-bit serial mode
 * channel 1 DMA if configured
 */
bool init_display(void)
{
	spi_link.txbuf = lcd_dma_buf; // use MCC DMA buffer variable
	spi_link.rxbuf = (void *) spi1_rec_buf; // setup receive buffer for other devices on the buss
	memset(Sstr, ' ', NSB * LSB); // clear scroll buffer of junk

#ifdef USE_LCD_DMA
	DMA1_SetSCNTIInterruptHandler(clear_lcd_done);
	DMA1_SetORIInterruptHandler(spi_lcd_byte);
	DMA1_SetDMAPriority(2);

	DMA2_SetSCNTIInterruptHandler(spi_src_byte);
	DMA2_SetORIInterruptHandler(spi_or_byte);
	DMA2_SetDCNTIInterruptHandler(spi_des_byte);
	DMA2_SetDMAPriority(3);

	DMA3_SetSCNTIInterruptHandler(spi_src_byte);
	DMA3_SetORIInterruptHandler(spi_or_byte);
	DMA3_SetDCNTIInterruptHandler(spi_des_byte);
	DMA3_SetDMAPriority(3);
#endif
#ifdef NHD  // uses MODE 3 on the Q84, https://newhavendisplay.com/content/specs/NHD-0420D3Z-NSW-BBW-V3.pdf
#ifdef USEMCC_SPI
#else
	SPI1CON0bits.EN = 0;
	// mode 3
	SPI1CON1 = 0x20;
	// SSET disabled; RXR suspended if the RxFIFO is full; TXR required for a transfer; 
	SPI1CON2 = 0x03;
	// BAUD 0; 
	SPI1BAUD = 0x04; // 50kHz SCK
	// CLKSEL MFINTOSC; 
	SPI1CLK = 0x02;
	// BMODE every byte; LSBF MSb first; EN enabled; MST bus master; 
	SPI1CON0 = 0x83;
	SPI1CON0bits.EN = 1;
#endif
	if (powerup) {
		wdtdelay(LCD_PWR_DELAY); // > 400ms power up delay
	}

#ifdef USE_LCD_DMA
	SPI1INTFbits.SPI1TXUIF = 0;

	DMASELECT = 0; // use DMA1 TX
	DMAnCON0bits.EN = 0;
	SPI1CON0bits.EN = 0;
	SPI1CON2 = 0x02; //  Received data is not stored in the FIFO
	SPI1CON0bits.EN = 1;
	DMAnCON1bits.DMODE = 0;
	DMAnCON1bits.DSTP = 0;
	DMAnCON1bits.SMODE = 1;
	DMAnCON1bits.SMR = 0;
	DMAnCON1bits.SSTP = 1;
	DMAnSSA = (uint24_t) spi_link.txbuf;
	DMAnCON0bits.DGO = 0;
	DMAnCON0bits.EN = 1; /* enable DMA */
	SPI1INTFbits.SPI1TXUIF = 1;
	send_lcd_cmd_dma(LCD_CMD_BRI); // set back-light level
	send_lcd_data_dma(NHD_BL_MED);
	send_lcd_cmd_dma(LCD_CMD_CONT); // set display contrast
	send_lcd_data_dma(NHD_CONT);
	send_lcd_cmd_dma(LCD_CMD_ON); // display on
	send_lcd_cmd_dma(LCD_CMD_CLR); // clear screen
	wdtdelay(NHD_L_DELAY);
	DMA1_StopTransfer();
	DMA2_StopTransfer();
#else
	send_lcd_cmd(LCD_CMD_BRI); // set back-light level
	send_lcd_data(NHD_BL_LOW);
	send_lcd_cmd(LCD_CMD_CONT); // set display contrast
	send_lcd_data(NHD_CONT);
	send_lcd_cmd(LCD_CMD_OFF); // display on
	send_lcd_cmd(LCD_CMD_CLR); // clear screen
	wdtdelay(NHD_L_DELAY);
	DMA1_StopTransfer();
	DMA2_StopTransfer();
#endif
#endif
	powerup = false; // only of the first display init call
	return true;
}

#ifdef NHD

/*
 * R2 short on LCD NHD-0420D3Z-NSW-BBW-V3 board
 */

static void send_lcd_data(const uint8_t data)
{
	CS_SetLow();
	SPI1_ExchangeByte(data);
	wdtdelay(NHD_T_DELAY);
}

static void send_lcd_cmd(const uint8_t cmd)
{
	CS_SetLow();
	SPI1_ExchangeByte(NHD_CMD);
	wdtdelay(NHD_T_DELAY);
	SPI1_ExchangeByte(cmd);
	wdtdelay(NHD_T_DELAY);
}

static void send_lcd_cmd_long(const uint8_t cmd)
{
	CS_SetLow();
	SPI1_ExchangeByte(NHD_CMD);
	wdtdelay(NHD_T_DELAY);
	SPI1_ExchangeByte(cmd);
	wdtdelay(NHD_L_DELAY);
}

/*
 * CAN use DMA channel 1 for transfers
 */
void eaDogM_WriteString(char *strPtr)
{
	uint8_t len = (uint8_t) strlen(strPtr);

	wait_lcd_done();
	wait_lcd_set();
	CS_SetLow(); /* SPI select display */
	if (len > (uint8_t) MAX_STRLEN) {
		len = MAX_STRLEN;
	}
	memcpy(spi_link.txbuf, strPtr, len);
#ifdef USE_LCD_DMA
	DMASELECT = 0x00;
	DMAnCON0bits.EN = 0; /* disable TX DMA to change source count */
	DMA1_SetSourceSize(len);
	DMA1_SetDestinationSize(1);
	DMAnCON0bits.EN = 1; /* enable DMA */
#else
	SPI1_ExchangeBlock(spi_link.txbuf, len);
#endif
	start_lcd(); // start DMA transfer
}

/*
 * uses DMA channel 1 for transfers
 */
void send_lcd_cmd_dma(const uint8_t strPtr)
{
	send_lcd_data_dma(NHD_CMD); //prefix
	send_lcd_data_dma(strPtr); // cmd code
}

/*
 * uses DMA channel 1 for transfers
 */
void send_lcd_data_dma(const uint8_t strPtr)
{
	wait_lcd_done();
	wait_lcd_set();
	CS_SetLow(); /* SPI select display */
	spi_link.txbuf[0] = strPtr;
	DMASELECT = 0x00;
	DMAnCON0bits.EN = 0; /* disable TX DMA to change source count */
	DMA1_SetSourceSize(1);
	DMA1_SetDestinationSize(1);
	DMAnCON0bits.EN = 1; /* enable DMA */
	start_lcd(); // start DMA transfer
}

void send_spi1_tic12400_dma(uint8_t *strPtr, const uint8_t len)
{
	wait_lcd_done();
	TIC_CS_SetLow(); /* SPI select display */
	spi_link.des_bytes++;
//	spi_link.READ_DATA = true;
	memcpy(spi_link.txbuf, strPtr, len);
	spi_link.rxbuf[0] = SPI1_ExchangeByte(spi_link.txbuf[0]);
	spi_link.rxbuf[1] = SPI1_ExchangeByte(spi_link.txbuf[1]);
	spi_link.rxbuf[2] = SPI1_ExchangeByte(spi_link.txbuf[2]);
	spi_link.rxbuf[3] = SPI1_ExchangeByte(spi_link.txbuf[3]);
	spi_link.READ_DATA = false; // extra time before CS goes high
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	TIC_CS_SetHigh();
}

void send_spi1_mc33996_dma(uint8_t *strPtr, const uint8_t len)
{
	wait_lcd_done();
	spi_link.READ_DATA = true;
	MCZ_CS_SetLow(); /* SPI select display */
	spi_link.des_bytes++;
	spi_link.READ_DATA = true;
	memcpy(spi_link.txbuf, strPtr, len);
	spi_link.rxbuf[0] = SPI1_ExchangeByte(spi_link.txbuf[0]);
	spi_link.rxbuf[1] = SPI1_ExchangeByte(spi_link.txbuf[1]);
	spi_link.rxbuf[2] = SPI1_ExchangeByte(spi_link.txbuf[2]);
	spi_link.READ_DATA = false;
	spi_link.READ_DATA = false;
	MCZ_CS_SetHigh();
}

/*
 * send three byte command string via DMA
 */
void send_lcd_pos_dma(const uint8_t strPtr)
{
	wait_lcd_done();
	wait_lcd_set();
	CS_SetLow(); /* SPI select display */
	spi_link.txbuf[0] = NHD_CMD;
	spi_link.txbuf[1] = NHD_POS;
	spi_link.txbuf[2] = strPtr;
	DMASELECT = 0x00;
	DMAnCON0bits.EN = 0; /* disable TX DMA to change source count */
	DMA1_SetSourceSize(3);
	DMA1_SetDestinationSize(1);
	DMAnCON0bits.EN = 1; /* enable DMA */
	start_lcd(); // start DMA transfer
}

void eaDogM_WriteStringAtPos(const uint8_t r, const uint8_t c, char *strPtr)
{
	uint8_t row;

#ifndef CAN_REMOTE
	if (scroll_lock) { // don't update LCD text when in scroll mode
		return;
	}
#endif
	switch (r) {
	case LCD1:
		row = 0x40;
		break;
	case LCD2:
		row = 0x14;
		break;
	case LCD3:
		row = 0x54;
		break;
	case LCD0:
		row = 0x00;
		break;
	default:
		row = 0x00;
		break;
	}
#ifdef USE_LCD_DMA
	send_lcd_pos_dma(row + c);
	wdtdelay(NHD_S_DELAY); // display command processing delay
#else
	send_lcd_cmd(0x45);
	send_lcd_data(row + c);
#endif
#ifdef USE_CAN
	can_fd_lcd_mirror(r, strPtr);
#endif
	eaDogM_WriteString(strPtr);
}

void eaDogM_WriteIntAtPos(const uint8_t r, const uint8_t c, const uint8_t i)
{

}

void eaDogM_SetPos(const uint8_t r, const uint8_t c)
{

}

void eaDogM_ClearRow(const uint8_t r)
{

}

void eaDogM_WriteByteToCGRAM(const uint8_t ndx, const uint8_t data)
{

}

#else
#endif

void eaDogM_WriteCommand(const uint8_t cmd)
{
#ifdef USE_LCD_DMA
	send_lcd_cmd_dma(cmd);
#else
	send_lcd_cmd(cmd);
#endif
}

void eaDogM_WriteChr(const int8_t value)
{
#ifdef USE_LCD_DMA
	send_lcd_data_dma((uint8_t) value);
#else
	send_lcd_data((uint8_t) value);
#endif
}

/*
 * Trigger the SPI DMA transfer to the LCD display
 */
void start_lcd(void)
{
#ifdef USE_LCD_DMA
	DMA1_StartTransferWithTrigger();
#endif
}

/*
 * Trigger the SPI DMA transfer from SPI devices
 */
void start_spi1_read(void)
{
#ifdef USE_LCD_DMA
	DMA2_StartTransfer();
	DMA2_StartTransferWithTrigger();
#endif
}

/*
 * Trigger the SPI DMA transfer from SPI devices
 */
void start_spi1_read1(void)
{
#ifdef USE_LCD_DMA
	DMA3_StartTransfer();
	DMA3_StartTransferWithTrigger();
#endif
}

void wait_lcd_set(void)
{
	spi_link.LCD_DATA = true;
}

void wait_read_set(void)
{
	spi_link.READ_DATA = true;
}

bool wait_lcd_check(void)
{
	return spi_link.LCD_DATA;
}

void wait_lcd_done(void)
{
#ifdef USE_LCD_DMA
	uint32_t delay = 0;
	while (spi_link.LCD_DATA) {
		if (delay++ > DONE_DELAY) {
			return;
		}
	};
	delay = 0;
	while (!SPI1STATUSbits.TXBE) {
		if (delay++ > DONE_DELAY) {
			return;
		}
	};
	DLED_SetLow();
#endif
}

void wait_read_done(void)
{
#ifdef USE_LCD_DMA
	uint32_t delay = 0;
	while (spi_link.READ_DATA) {
		if (delay++ > DONE_DELAY) {
			return;
		}
	};
	delay = 0;
	while (!SPI1STATUSbits.TXBE) {
		if (delay++ > DONE_DELAY) {
			return;
		}
	};
	DLED_SetLow();
#endif
}

/*
 * in DMA mode this is a ISR that runs when the TX source count is complete
 */
void clear_lcd_done(void)
{
	spi_link.LCD_DATA = false;
}

/*
 * SPI buffer LCD ISR
 */
static void spi_lcd_byte(void)
{
//	MLED_Toggle();
}

/*
 * in DMA mode this is a ISR that runs when the RX source count is complete
 */
void spi_src_byte(void)
{
	spi_link.rxbuf[inx++] = SPI1RXB;
	if (inx > 3) inx = 0;
	spi_link.src_bytes++;
}

/*
 * SPI buffer LCD ISR
 */
static void spi_or_byte(void)
{
//	MLED_Toggle();
	spi_link.or_bytes++;
}

/*
 * SPI buffer destination complete ISR
 */
static void spi_des_byte(void)
{
//	MLED_Toggle();
	spi_link.des_bytes++;
	spi_link.READ_DATA = false;
}

void spi_rec_done(void)
{
#ifdef USE_LCD_DMA
#endif
}

/*
 * auto scrolls up the string on the display
 */
char * eaDogM_Scroll_String(char *strPtr)
{
	scroll_lock = true;
	memcpy((void *) &Sstr[4][0], &Sstr[0][0], MAX_STRLEN); // move top line to old line buffer
	memcpy((void *) &Sstr[0][0], &Sstr[1][0], MAX_STRLEN);
	memcpy((void *) &Sstr[1][0], &Sstr[2][0], MAX_STRLEN);
	memcpy((void *) &Sstr[2][0], &Sstr[3][0], MAX_STRLEN);
	memcpy((void *) &Sstr[3][0], strPtr, MAX_STRLEN); // place new text on the bottom line
	scroll_line_pos = 4;
	return &Sstr[4][0];
}

/*
 * mainline info scroll updater, should run a ~1 second ticks
 */
void eaDogM_Scroll_Task(void)
{
	if (!scroll_lock) {
		return;
	}

	if (scroll_line_pos == 0) { // wait for last line display time period
		scroll_lock = false;
		scroll_line_pos = 4;
		return;
	}

	scroll_lock = false;
	--scroll_line_pos;
	eaDogM_WriteStringAtPos(scroll_line_pos, 0, &Sstr[scroll_line_pos][0]);
	scroll_lock = true;

}

void no_dma_set_lcd(void)
{
	send_lcd_cmd(LCD_CMD_BRI); // set back-light level
	send_lcd_data(NHD_BL_HIGH);
	send_lcd_cmd(LCD_CMD_CONT); // set display contrast
	send_lcd_data(NHD_CONT);
	send_lcd_cmd(LCD_CMD_ON); // display on
	send_lcd_cmd(LCD_CMD_CLR); // clear screen
	wdtdelay(NHD_L_DELAY);
}

void check_lcd_dim(const bool dim)
{
	if (B.display_update) {
		B.display_update = false;
		B.dim_delay = 0;
#ifdef USE_LCD_DMA
		if (dim) {
			send_lcd_cmd_dma(LCD_CMD_BRI); // set back-light level
			send_lcd_data_dma(NHD_BL_OFF);
		} else {
			send_lcd_cmd_dma(LCD_CMD_BRI); // set back-light level
			send_lcd_data_dma(NHD_BL_HIGH);
		}
#else
		if (dim) {
			send_lcd_cmd(LCD_CMD_BRI); // set back-light level
			send_lcd_data(NHD_BL_LOW);
		} else {
			send_lcd_cmd(LCD_CMD_BRI); // set back-light level
			send_lcd_data(NHD_BL_HIGH);
		}
#endif
	}
}

void set_lcd_dim(const bool dim)
{
	if (B.display_update) {
		B.display_update = false;
		B.dim_delay = 0;
#ifdef USE_LCD_DMA
		if (dim) {
			send_lcd_cmd_dma(LCD_CMD_BRI); // set back-light level
			send_lcd_data_dma(NHD_BL_OFF);
		} else {
			send_lcd_cmd_dma(LCD_CMD_BRI); // set back-light level
			send_lcd_data_dma(NHD_BL_HIGH);
		}
#else
		if (dim) {
			send_lcd_cmd(LCD_CMD_BRI); // set back-light level
			send_lcd_data(NHD_BL_LOW);
		} else {
			send_lcd_cmd(LCD_CMD_BRI); // set back-light level
			send_lcd_data(NHD_BL_HIGH);
		}
#endif
	}

	if (B.dim_delay++ >= DIM_DELAY) {
		B.dim_delay = 0;
#ifdef USE_LCD_DMA
		send_lcd_cmd_dma(LCD_CMD_BRI); // set back-light level
		send_lcd_data_dma(NHD_BL_OFF);
#else
		send_lcd_cmd(LCD_CMD_BRI); // set back-light level
		send_lcd_data(NHD_BL_LOW);
#endif
	}
}

/*
 * busy loop delay with WDT reset
 */
static void wdtdelay(const uint32_t delay)
{
	uint32_t dcount;

	for (dcount = 0; dcount <= delay; dcount++) { // delay a bit
		ClrWdt(); // reset the WDT timer
	};
}
