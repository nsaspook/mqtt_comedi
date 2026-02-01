/** \file bmcdio.c
 */
#include "bmcdio.h"
#include "eadog.h"

static const char *build_date = __DATE__, *build_time = __TIME__;
static volatile uint8_t dummy_b = 0;

/*
 * SPI1
 */
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

/*
 * SPI1
 */
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
	SPI1CON1bits.CKP = 0; // SCK starts low
	SPI1CON1bits.CKE = 0;
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

/*
 * SPI1
 */
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
	SPI1CON1bits.CKE = 0;
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

/*
 * SPI2
 */
void SPI2_PI(void)
{
}