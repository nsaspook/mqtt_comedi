/**
  DMA3 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    dma3.c

  @Summary
    This is the generated driver implementation file for the DMA3 driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This source file provides APIs for DMA3.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  1.0.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB 	          :  MPLAB X 6.00
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "dma3.h"

void (*DMA3_SCNTI_InterruptHandler)(void);
void (*DMA3_DCNTI_InterruptHandler)(void);
void (*DMA3_ORI_InterruptHandler)(void);

/**
  Section: DMA3 APIs
*/

void DMA3_Initialize(void)
{
    //DMA Instance Selection : 0x02
    DMASELECT = 0x02;
    //Source Address : &SPI1RXB
    DMAnSSA = (volatile unsigned short)&SPI1RXB;
    //Destination Address : &spi1_rec_buf1
    DMAnDSA= (volatile unsigned short)&spi1_rec_buf1;
    //DMODE unchanged; DSTP not cleared; SMR SFR; SMODE unchanged; SSTP not cleared; 
    DMAnCON1 = 0x00;
    //Source Message Size : 1
    DMAnSSZ = 1;
    //Destination Message Size : 3
    DMAnDSZ = 3;
    //Start Trigger : SIRQ None; 
    DMAnSIRQ = 0x00;
    //Abort Trigger : AIRQ None; 
    DMAnAIRQ = 0x00;
	
    // Clear Destination Count Interrupt Flag bit
    PIR10bits.DMA3DCNTIF = 0; 
    // Clear Source Count Interrupt Flag bit
    PIR10bits.DMA3SCNTIF = 0; 
    // Clear Abort Interrupt Flag bit
    PIR10bits.DMA3AIF = 0; 
    // Clear Overrun Interrupt Flag bit
    PIR10bits.DMA3ORIF =0; 
    
    PIE10bits.DMA3DCNTIE = 1;
	DMA3_SetDCNTIInterruptHandler(DMA3_DefaultInterruptHandler);
    PIE10bits.DMA3SCNTIE = 1; 
	DMA3_SetSCNTIInterruptHandler(DMA3_DefaultInterruptHandler);
    PIE10bits.DMA3AIE = 0;
    PIE10bits.DMA3ORIE =1; 
	DMA3_SetORIInterruptHandler(DMA3_DefaultInterruptHandler);
	
    //EN enabled; SIRQEN disabled; DGO not in progress; AIRQEN disabled; 
    DMAnCON0 = 0x80;
	
}

void DMA3_SelectSourceRegion(uint8_t region)
{
    DMASELECT = 0x02;
	DMAnCON1bits.SMR  = region;
}

void DMA3_SetSourceAddress(uint24_t address)
{
    DMASELECT = 0x02;
	DMAnSSA = address;
}

void DMA3_SetDestinationAddress(uint16_t address)
{
    DMASELECT = 0x02;
	DMAnDSA = address;
}

void DMA3_SetSourceSize(uint16_t size)
{
    DMASELECT = 0x02;
	DMAnSSZ= size;
}

void DMA3_SetDestinationSize(uint16_t size)
{                     
    DMASELECT = 0x02;
	DMAnDSZ= size;
}

uint24_t DMA3_GetSourcePointer(void)
{
    DMASELECT = 0x02;
	return DMAnSPTR;
}

uint16_t DMA3_GetDestinationPointer(void)
{
    DMASELECT = 0x02;
	return DMAnDPTR;
}

void DMA3_SetStartTrigger(uint8_t sirq)
{
    DMASELECT = 0x02;
	DMAnSIRQ = sirq;
}

void DMA3_SetAbortTrigger(uint8_t airq)
{
    DMASELECT = 0x02;
	DMAnAIRQ = airq;
}

void DMA3_StartTransfer(void)
{
    DMASELECT = 0x02;
	DMAnCON0bits.DGO = 1;
}

void DMA3_StartTransferWithTrigger(void)
{
    DMASELECT = 0x02;
	DMAnCON0bits.SIRQEN = 1;
}

void DMA3_StopTransfer(void)
{
    DMASELECT = 0x02;
	DMAnCON0bits.SIRQEN = 0; 
	DMAnCON0bits.DGO = 0;
}

void DMA3_SetDMAPriority(uint8_t priority)
{
    // This function is dependant on the PR1WAY CONFIG bit
	PRLOCK = 0x55;
	PRLOCK = 0xAA;
	PRLOCKbits.PRLOCKED = 0;
	DMA3PR = priority;
	PRLOCK = 0x55;
	PRLOCK = 0xAA;
	PRLOCKbits.PRLOCKED = 1;
}

void __interrupt(irq(IRQ_DMA3SCNT),base(8)) DMA3_DMASCNTI_ISR()
{
    // Clear the source count interrupt flag
    PIR10bits.DMA3SCNTIF = 0;

    if (DMA3_SCNTI_InterruptHandler)
            DMA3_SCNTI_InterruptHandler();
}

void DMA3_SetSCNTIInterruptHandler(void (* InterruptHandler)(void))
{
	 DMA3_SCNTI_InterruptHandler = InterruptHandler;
}

void __interrupt(irq(IRQ_DMA3DCNT),base(8)) DMA3_DMADCNTI_ISR()
{
    // Clear the source count interrupt flag
    PIR10bits.DMA3DCNTIF = 0;

    if (DMA3_DCNTI_InterruptHandler)
            DMA3_DCNTI_InterruptHandler();
}

void DMA3_SetDCNTIInterruptHandler(void (* InterruptHandler)(void))
{
	 DMA3_DCNTI_InterruptHandler = InterruptHandler;
}

void __interrupt(irq(IRQ_DMA3OR),base(8)) DMA3_DMAORI_ISR()
{
    // Clear the source count interrupt flag
    PIR10bits.DMA3ORIF = 0;

    if (DMA3_ORI_InterruptHandler)
            DMA3_ORI_InterruptHandler();
}

void DMA3_SetORIInterruptHandler(void (* InterruptHandler)(void))
{
	 DMA3_ORI_InterruptHandler = InterruptHandler;
}

void DMA3_DefaultInterruptHandler(void){
    // add your DMA3 interrupt custom code
    // or set custom function using DMA3_SetSCNTIInterruptHandler() /DMA3_SetDCNTIInterruptHandler() /DMA3_SetAIInterruptHandler() /DMA3_SetORIInterruptHandler()
}
/**
 End of File
*/
