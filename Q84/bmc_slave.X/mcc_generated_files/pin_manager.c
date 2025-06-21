/**
  Generated Pin Manager File

  Company:
    Microchip Technology Inc.

  File Name:
    pin_manager.c

  Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  Description:
    This header file provides implementations for pin APIs for all pins selected in the GUI.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F47Q84
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above
        MPLAB             :  MPLAB X 6.00

    Copyright (c) 2013 - 2015 released Microchip Technology Inc.  All rights reserved.
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

#include "pin_manager.h"
#include "interrupt_manager.h"




void (*IOCBF6_InterruptHandler)(void);


void PIN_MANAGER_Initialize(void)
{
    /**
    LATx registers
    */
    LATE = 0x07;
    LATD = 0xDA;
    LATA = 0x00;
    LATB = 0x01;
    LATC = 0x02;

    /**
    TRISx registers
    */
    TRISE = 0x00;
    TRISA = 0xFF;
    TRISB = 0xE0;
    TRISC = 0xD1;
    TRISD = 0x27;

    /**
    ANSELx registers
    */
    ANSELD = 0x20;
    ANSELC = 0xC0;
    ANSELB = 0x80;
    ANSELE = 0x00;
    ANSELA = 0xFF;

    /**
    WPUx registers
    */
    WPUD = 0x8F;
    WPUE = 0x07;
    WPUB = 0x71;
    WPUA = 0x00;
    WPUC = 0x31;

    /**
    ODx registers
    */
    ODCONE = 0x00;
    ODCONA = 0x00;
    ODCONB = 0x01;
    ODCONC = 0x00;
    ODCOND = 0x80;

    /**
    SLRCONx registers
    */
    SLRCONA = 0xFF;
    SLRCONB = 0xFF;
    SLRCONC = 0xFF;
    SLRCOND = 0xFF;
    SLRCONE = 0x07;

    /**
    INLVLx registers
    */
    INLVLA = 0xFF;
    INLVLB = 0xFF;
    INLVLC = 0xFF;
    INLVLD = 0xFF;
    INLVLE = 0x0F;


    /**
    IOCx registers 
    */
    //interrupt on change for group IOCBF - flag
    IOCBFbits.IOCBF6 = 0;
    //interrupt on change for group IOCBN - negative
    IOCBNbits.IOCBN6 = 1;
    //interrupt on change for group IOCBP - positive
    IOCBPbits.IOCBP6 = 0;



    // register default IOC callback functions at runtime; use these methods to register a custom function
    IOCBF6_SetInterruptHandler(IOCBF6_DefaultInterruptHandler);
   
    // Enable IOCI interrupt 
    PIE0bits.IOCIE = 1; 
    
	
    U2RXPPS = 0x18;   //RD0->UART2:RX2;    
    SPI1SCKPPS = 0x13;   //RC3->SPI1:SCK1;    
    SPI2SDIPPS = 0x0D;   //RB5->SPI2:SDI2;    
    RC3PPS = 0x31;   //RC3->SPI1:SCK1;    
    RC1PPS = 0x20;   //RC1->UART1:TX1;    
    RB4PPS = 0x35;   //RB4->SPI2:SDO2;    
    RC5PPS = 0x32;   //RC5->SPI1:SDO1;    
    U1RXPPS = 0x10;   //RC0->UART1:RX1;    
    RD6PPS = 0x23;   //RD6->UART2:TX2;    
    SPI2SCKPPS = 0x1A;   //RD2->SPI2:SCK2;    
    SPI1SDIPPS = 0x14;   //RC4->SPI1:SDI1;    
}
  
void __interrupt(irq(IOC),base(8)) PIN_MANAGER_IOC()
{   
	// interrupt on change for pin IOCBF6
    if(IOCBFbits.IOCBF6 == 1)
    {
        IOCBF6_ISR();  
    }	
}

/**
   IOCBF6 Interrupt Service Routine
*/
void IOCBF6_ISR(void) {

    // Add custom IOCBF6 code

    // Call the interrupt handler for the callback registered at runtime
    if(IOCBF6_InterruptHandler)
    {
        IOCBF6_InterruptHandler();
    }
    IOCBFbits.IOCBF6 = 0;
}

/**
  Allows selecting an interrupt handler for IOCBF6 at application runtime
*/
void IOCBF6_SetInterruptHandler(void (* InterruptHandler)(void)){
    IOCBF6_InterruptHandler = InterruptHandler;
}

/**
  Default interrupt handler for IOCBF6
*/
void IOCBF6_DefaultInterruptHandler(void){
    // add your IOCBF6 interrupt custom code
    // or set custom function using IOCBF6_SetInterruptHandler()
}

/**
 End of File
*/