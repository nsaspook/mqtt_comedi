/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F57Q84
        Driver Version    :  2.11
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

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set channel_ANA0 aliases
#define channel_ANA0_TRIS                 TRISAbits.TRISA0
#define channel_ANA0_LAT                  LATAbits.LATA0
#define channel_ANA0_PORT                 PORTAbits.RA0
#define channel_ANA0_WPU                  WPUAbits.WPUA0
#define channel_ANA0_OD                   ODCONAbits.ODCA0
#define channel_ANA0_ANS                  ANSELAbits.ANSELA0
#define channel_ANA0_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define channel_ANA0_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define channel_ANA0_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define channel_ANA0_GetValue()           PORTAbits.RA0
#define channel_ANA0_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define channel_ANA0_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define channel_ANA0_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define channel_ANA0_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define channel_ANA0_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define channel_ANA0_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define channel_ANA0_SetAnalogMode()      do { ANSELAbits.ANSELA0 = 1; } while(0)
#define channel_ANA0_SetDigitalMode()     do { ANSELAbits.ANSELA0 = 0; } while(0)

// get/set channel_ANA1 aliases
#define channel_ANA1_TRIS                 TRISAbits.TRISA1
#define channel_ANA1_LAT                  LATAbits.LATA1
#define channel_ANA1_PORT                 PORTAbits.RA1
#define channel_ANA1_WPU                  WPUAbits.WPUA1
#define channel_ANA1_OD                   ODCONAbits.ODCA1
#define channel_ANA1_ANS                  ANSELAbits.ANSELA1
#define channel_ANA1_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define channel_ANA1_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define channel_ANA1_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define channel_ANA1_GetValue()           PORTAbits.RA1
#define channel_ANA1_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define channel_ANA1_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define channel_ANA1_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define channel_ANA1_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define channel_ANA1_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define channel_ANA1_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define channel_ANA1_SetAnalogMode()      do { ANSELAbits.ANSELA1 = 1; } while(0)
#define channel_ANA1_SetDigitalMode()     do { ANSELAbits.ANSELA1 = 0; } while(0)

// get/set channel_ANA2 aliases
#define channel_ANA2_TRIS                 TRISAbits.TRISA2
#define channel_ANA2_LAT                  LATAbits.LATA2
#define channel_ANA2_PORT                 PORTAbits.RA2
#define channel_ANA2_WPU                  WPUAbits.WPUA2
#define channel_ANA2_OD                   ODCONAbits.ODCA2
#define channel_ANA2_ANS                  ANSELAbits.ANSELA2
#define channel_ANA2_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define channel_ANA2_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define channel_ANA2_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define channel_ANA2_GetValue()           PORTAbits.RA2
#define channel_ANA2_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define channel_ANA2_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define channel_ANA2_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define channel_ANA2_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define channel_ANA2_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define channel_ANA2_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define channel_ANA2_SetAnalogMode()      do { ANSELAbits.ANSELA2 = 1; } while(0)
#define channel_ANA2_SetDigitalMode()     do { ANSELAbits.ANSELA2 = 0; } while(0)

// get/set RA3 procedures
#define RA3_SetHigh()            do { LATAbits.LATA3 = 1; } while(0)
#define RA3_SetLow()             do { LATAbits.LATA3 = 0; } while(0)
#define RA3_Toggle()             do { LATAbits.LATA3 = ~LATAbits.LATA3; } while(0)
#define RA3_GetValue()              PORTAbits.RA3
#define RA3_SetDigitalInput()    do { TRISAbits.TRISA3 = 1; } while(0)
#define RA3_SetDigitalOutput()   do { TRISAbits.TRISA3 = 0; } while(0)
#define RA3_SetPullup()             do { WPUAbits.WPUA3 = 1; } while(0)
#define RA3_ResetPullup()           do { WPUAbits.WPUA3 = 0; } while(0)
#define RA3_SetAnalogMode()         do { ANSELAbits.ANSELA3 = 1; } while(0)
#define RA3_SetDigitalMode()        do { ANSELAbits.ANSELA3 = 0; } while(0)

// get/set channel_ANA4 aliases
#define channel_ANA4_TRIS                 TRISAbits.TRISA4
#define channel_ANA4_LAT                  LATAbits.LATA4
#define channel_ANA4_PORT                 PORTAbits.RA4
#define channel_ANA4_WPU                  WPUAbits.WPUA4
#define channel_ANA4_OD                   ODCONAbits.ODCA4
#define channel_ANA4_ANS                  ANSELAbits.ANSELA4
#define channel_ANA4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define channel_ANA4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define channel_ANA4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define channel_ANA4_GetValue()           PORTAbits.RA4
#define channel_ANA4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define channel_ANA4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define channel_ANA4_SetPullup()          do { WPUAbits.WPUA4 = 1; } while(0)
#define channel_ANA4_ResetPullup()        do { WPUAbits.WPUA4 = 0; } while(0)
#define channel_ANA4_SetPushPull()        do { ODCONAbits.ODCA4 = 0; } while(0)
#define channel_ANA4_SetOpenDrain()       do { ODCONAbits.ODCA4 = 1; } while(0)
#define channel_ANA4_SetAnalogMode()      do { ANSELAbits.ANSELA4 = 1; } while(0)
#define channel_ANA4_SetDigitalMode()     do { ANSELAbits.ANSELA4 = 0; } while(0)

// get/set channel_ANA5 aliases
#define channel_ANA5_TRIS                 TRISAbits.TRISA5
#define channel_ANA5_LAT                  LATAbits.LATA5
#define channel_ANA5_PORT                 PORTAbits.RA5
#define channel_ANA5_WPU                  WPUAbits.WPUA5
#define channel_ANA5_OD                   ODCONAbits.ODCA5
#define channel_ANA5_ANS                  ANSELAbits.ANSELA5
#define channel_ANA5_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define channel_ANA5_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define channel_ANA5_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define channel_ANA5_GetValue()           PORTAbits.RA5
#define channel_ANA5_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define channel_ANA5_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define channel_ANA5_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define channel_ANA5_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define channel_ANA5_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define channel_ANA5_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define channel_ANA5_SetAnalogMode()      do { ANSELAbits.ANSELA5 = 1; } while(0)
#define channel_ANA5_SetDigitalMode()     do { ANSELAbits.ANSELA5 = 0; } while(0)

// get/set SPI2_REQ aliases
#define SPI2_REQ_TRIS                 TRISBbits.TRISB0
#define SPI2_REQ_LAT                  LATBbits.LATB0
#define SPI2_REQ_PORT                 PORTBbits.RB0
#define SPI2_REQ_WPU                  WPUBbits.WPUB0
#define SPI2_REQ_OD                   ODCONBbits.ODCB0
#define SPI2_REQ_ANS                  ANSELBbits.ANSELB0
#define SPI2_REQ_SetHigh()            do { LATBbits.LATB0 = 1; } while(0)
#define SPI2_REQ_SetLow()             do { LATBbits.LATB0 = 0; } while(0)
#define SPI2_REQ_Toggle()             do { LATBbits.LATB0 = ~LATBbits.LATB0; } while(0)
#define SPI2_REQ_GetValue()           PORTBbits.RB0
#define SPI2_REQ_SetDigitalInput()    do { TRISBbits.TRISB0 = 1; } while(0)
#define SPI2_REQ_SetDigitalOutput()   do { TRISBbits.TRISB0 = 0; } while(0)
#define SPI2_REQ_SetPullup()          do { WPUBbits.WPUB0 = 1; } while(0)
#define SPI2_REQ_ResetPullup()        do { WPUBbits.WPUB0 = 0; } while(0)
#define SPI2_REQ_SetPushPull()        do { ODCONBbits.ODCB0 = 0; } while(0)
#define SPI2_REQ_SetOpenDrain()       do { ODCONBbits.ODCB0 = 1; } while(0)
#define SPI2_REQ_SetAnalogMode()      do { ANSELBbits.ANSELB0 = 1; } while(0)
#define SPI2_REQ_SetDigitalMode()     do { ANSELBbits.ANSELB0 = 0; } while(0)

// get/set MLED aliases
#define MLED_TRIS                 TRISBbits.TRISB1
#define MLED_LAT                  LATBbits.LATB1
#define MLED_PORT                 PORTBbits.RB1
#define MLED_WPU                  WPUBbits.WPUB1
#define MLED_OD                   ODCONBbits.ODCB1
#define MLED_ANS                  ANSELBbits.ANSELB1
#define MLED_SetHigh()            do { LATBbits.LATB1 = 1; } while(0)
#define MLED_SetLow()             do { LATBbits.LATB1 = 0; } while(0)
#define MLED_Toggle()             do { LATBbits.LATB1 = ~LATBbits.LATB1; } while(0)
#define MLED_GetValue()           PORTBbits.RB1
#define MLED_SetDigitalInput()    do { TRISBbits.TRISB1 = 1; } while(0)
#define MLED_SetDigitalOutput()   do { TRISBbits.TRISB1 = 0; } while(0)
#define MLED_SetPullup()          do { WPUBbits.WPUB1 = 1; } while(0)
#define MLED_ResetPullup()        do { WPUBbits.WPUB1 = 0; } while(0)
#define MLED_SetPushPull()        do { ODCONBbits.ODCB1 = 0; } while(0)
#define MLED_SetOpenDrain()       do { ODCONBbits.ODCB1 = 1; } while(0)
#define MLED_SetAnalogMode()      do { ANSELBbits.ANSELB1 = 1; } while(0)
#define MLED_SetDigitalMode()     do { ANSELBbits.ANSELB1 = 0; } while(0)

// get/set RLED aliases
#define RLED_TRIS                 TRISBbits.TRISB2
#define RLED_LAT                  LATBbits.LATB2
#define RLED_PORT                 PORTBbits.RB2
#define RLED_WPU                  WPUBbits.WPUB2
#define RLED_OD                   ODCONBbits.ODCB2
#define RLED_ANS                  ANSELBbits.ANSELB2
#define RLED_SetHigh()            do { LATBbits.LATB2 = 1; } while(0)
#define RLED_SetLow()             do { LATBbits.LATB2 = 0; } while(0)
#define RLED_Toggle()             do { LATBbits.LATB2 = ~LATBbits.LATB2; } while(0)
#define RLED_GetValue()           PORTBbits.RB2
#define RLED_SetDigitalInput()    do { TRISBbits.TRISB2 = 1; } while(0)
#define RLED_SetDigitalOutput()   do { TRISBbits.TRISB2 = 0; } while(0)
#define RLED_SetPullup()          do { WPUBbits.WPUB2 = 1; } while(0)
#define RLED_ResetPullup()        do { WPUBbits.WPUB2 = 0; } while(0)
#define RLED_SetPushPull()        do { ODCONBbits.ODCB2 = 0; } while(0)
#define RLED_SetOpenDrain()       do { ODCONBbits.ODCB2 = 1; } while(0)
#define RLED_SetAnalogMode()      do { ANSELBbits.ANSELB2 = 1; } while(0)
#define RLED_SetDigitalMode()     do { ANSELBbits.ANSELB2 = 0; } while(0)

// get/set DLED aliases
#define DLED_TRIS                 TRISBbits.TRISB3
#define DLED_LAT                  LATBbits.LATB3
#define DLED_PORT                 PORTBbits.RB3
#define DLED_WPU                  WPUBbits.WPUB3
#define DLED_OD                   ODCONBbits.ODCB3
#define DLED_ANS                  ANSELBbits.ANSELB3
#define DLED_SetHigh()            do { LATBbits.LATB3 = 1; } while(0)
#define DLED_SetLow()             do { LATBbits.LATB3 = 0; } while(0)
#define DLED_Toggle()             do { LATBbits.LATB3 = ~LATBbits.LATB3; } while(0)
#define DLED_GetValue()           PORTBbits.RB3
#define DLED_SetDigitalInput()    do { TRISBbits.TRISB3 = 1; } while(0)
#define DLED_SetDigitalOutput()   do { TRISBbits.TRISB3 = 0; } while(0)
#define DLED_SetPullup()          do { WPUBbits.WPUB3 = 1; } while(0)
#define DLED_ResetPullup()        do { WPUBbits.WPUB3 = 0; } while(0)
#define DLED_SetPushPull()        do { ODCONBbits.ODCB3 = 0; } while(0)
#define DLED_SetOpenDrain()       do { ODCONBbits.ODCB3 = 1; } while(0)
#define DLED_SetAnalogMode()      do { ANSELBbits.ANSELB3 = 1; } while(0)
#define DLED_SetDigitalMode()     do { ANSELBbits.ANSELB3 = 0; } while(0)

// get/set RB4 procedures
#define RB4_SetHigh()            do { LATBbits.LATB4 = 1; } while(0)
#define RB4_SetLow()             do { LATBbits.LATB4 = 0; } while(0)
#define RB4_Toggle()             do { LATBbits.LATB4 = ~LATBbits.LATB4; } while(0)
#define RB4_GetValue()              PORTBbits.RB4
#define RB4_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define RB4_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)
#define RB4_SetPullup()             do { WPUBbits.WPUB4 = 1; } while(0)
#define RB4_ResetPullup()           do { WPUBbits.WPUB4 = 0; } while(0)
#define RB4_SetAnalogMode()         do { ANSELBbits.ANSELB4 = 1; } while(0)
#define RB4_SetDigitalMode()        do { ANSELBbits.ANSELB4 = 0; } while(0)

// get/set RB5 procedures
#define RB5_SetHigh()            do { LATBbits.LATB5 = 1; } while(0)
#define RB5_SetLow()             do { LATBbits.LATB5 = 0; } while(0)
#define RB5_Toggle()             do { LATBbits.LATB5 = ~LATBbits.LATB5; } while(0)
#define RB5_GetValue()              PORTBbits.RB5
#define RB5_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define RB5_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
#define RB5_SetPullup()             do { WPUBbits.WPUB5 = 1; } while(0)
#define RB5_ResetPullup()           do { WPUBbits.WPUB5 = 0; } while(0)
#define RB5_SetAnalogMode()         do { ANSELBbits.ANSELB5 = 1; } while(0)
#define RB5_SetDigitalMode()        do { ANSELBbits.ANSELB5 = 0; } while(0)

// get/set TIC_INT aliases
#define TIC_INT_TRIS                 TRISBbits.TRISB6
#define TIC_INT_LAT                  LATBbits.LATB6
#define TIC_INT_PORT                 PORTBbits.RB6
#define TIC_INT_WPU                  WPUBbits.WPUB6
#define TIC_INT_OD                   ODCONBbits.ODCB6
#define TIC_INT_ANS                  ANSELBbits.ANSELB6
#define TIC_INT_SetHigh()            do { LATBbits.LATB6 = 1; } while(0)
#define TIC_INT_SetLow()             do { LATBbits.LATB6 = 0; } while(0)
#define TIC_INT_Toggle()             do { LATBbits.LATB6 = ~LATBbits.LATB6; } while(0)
#define TIC_INT_GetValue()           PORTBbits.RB6
#define TIC_INT_SetDigitalInput()    do { TRISBbits.TRISB6 = 1; } while(0)
#define TIC_INT_SetDigitalOutput()   do { TRISBbits.TRISB6 = 0; } while(0)
#define TIC_INT_SetPullup()          do { WPUBbits.WPUB6 = 1; } while(0)
#define TIC_INT_ResetPullup()        do { WPUBbits.WPUB6 = 0; } while(0)
#define TIC_INT_SetPushPull()        do { ODCONBbits.ODCB6 = 0; } while(0)
#define TIC_INT_SetOpenDrain()       do { ODCONBbits.ODCB6 = 1; } while(0)
#define TIC_INT_SetAnalogMode()      do { ANSELBbits.ANSELB6 = 1; } while(0)
#define TIC_INT_SetDigitalMode()     do { ANSELBbits.ANSELB6 = 0; } while(0)

// get/set RB7 procedures
#define RB7_SetHigh()            do { LATBbits.LATB7 = 1; } while(0)
#define RB7_SetLow()             do { LATBbits.LATB7 = 0; } while(0)
#define RB7_Toggle()             do { LATBbits.LATB7 = ~LATBbits.LATB7; } while(0)
#define RB7_GetValue()              PORTBbits.RB7
#define RB7_SetDigitalInput()    do { TRISBbits.TRISB7 = 1; } while(0)
#define RB7_SetDigitalOutput()   do { TRISBbits.TRISB7 = 0; } while(0)
#define RB7_SetPullup()             do { WPUBbits.WPUB7 = 1; } while(0)
#define RB7_ResetPullup()           do { WPUBbits.WPUB7 = 0; } while(0)
#define RB7_SetAnalogMode()         do { ANSELBbits.ANSELB7 = 1; } while(0)
#define RB7_SetDigitalMode()        do { ANSELBbits.ANSELB7 = 0; } while(0)

// get/set RC0 procedures
#define RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define RC0_GetValue()              PORTCbits.RC0
#define RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define RC0_SetPullup()             do { WPUCbits.WPUC0 = 1; } while(0)
#define RC0_ResetPullup()           do { WPUCbits.WPUC0 = 0; } while(0)
#define RC0_SetAnalogMode()         do { ANSELCbits.ANSELC0 = 1; } while(0)
#define RC0_SetDigitalMode()        do { ANSELCbits.ANSELC0 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()              PORTCbits.RC1
#define RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define RC1_SetPullup()             do { WPUCbits.WPUC1 = 1; } while(0)
#define RC1_ResetPullup()           do { WPUCbits.WPUC1 = 0; } while(0)
#define RC1_SetAnalogMode()         do { ANSELCbits.ANSELC1 = 1; } while(0)
#define RC1_SetDigitalMode()        do { ANSELCbits.ANSELC1 = 0; } while(0)

// get/set TP1 aliases
#define TP1_TRIS                 TRISCbits.TRISC2
#define TP1_LAT                  LATCbits.LATC2
#define TP1_PORT                 PORTCbits.RC2
#define TP1_WPU                  WPUCbits.WPUC2
#define TP1_OD                   ODCONCbits.ODCC2
#define TP1_ANS                  ANSELCbits.ANSELC2
#define TP1_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define TP1_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define TP1_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define TP1_GetValue()           PORTCbits.RC2
#define TP1_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define TP1_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define TP1_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define TP1_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define TP1_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define TP1_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define TP1_SetAnalogMode()      do { ANSELCbits.ANSELC2 = 1; } while(0)
#define TP1_SetDigitalMode()     do { ANSELCbits.ANSELC2 = 0; } while(0)

// get/set RC3 procedures
#define RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define RC3_GetValue()              PORTCbits.RC3
#define RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define RC3_SetPullup()             do { WPUCbits.WPUC3 = 1; } while(0)
#define RC3_ResetPullup()           do { WPUCbits.WPUC3 = 0; } while(0)
#define RC3_SetAnalogMode()         do { ANSELCbits.ANSELC3 = 1; } while(0)
#define RC3_SetDigitalMode()        do { ANSELCbits.ANSELC3 = 0; } while(0)

// get/set RC4 procedures
#define RC4_SetHigh()            do { LATCbits.LATC4 = 1; } while(0)
#define RC4_SetLow()             do { LATCbits.LATC4 = 0; } while(0)
#define RC4_Toggle()             do { LATCbits.LATC4 = ~LATCbits.LATC4; } while(0)
#define RC4_GetValue()              PORTCbits.RC4
#define RC4_SetDigitalInput()    do { TRISCbits.TRISC4 = 1; } while(0)
#define RC4_SetDigitalOutput()   do { TRISCbits.TRISC4 = 0; } while(0)
#define RC4_SetPullup()             do { WPUCbits.WPUC4 = 1; } while(0)
#define RC4_ResetPullup()           do { WPUCbits.WPUC4 = 0; } while(0)
#define RC4_SetAnalogMode()         do { ANSELCbits.ANSELC4 = 1; } while(0)
#define RC4_SetDigitalMode()        do { ANSELCbits.ANSELC4 = 0; } while(0)

// get/set RC5 procedures
#define RC5_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define RC5_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define RC5_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RC5_GetValue()              PORTCbits.RC5
#define RC5_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define RC5_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define RC5_SetPullup()             do { WPUCbits.WPUC5 = 1; } while(0)
#define RC5_ResetPullup()           do { WPUCbits.WPUC5 = 0; } while(0)
#define RC5_SetAnalogMode()         do { ANSELCbits.ANSELC5 = 1; } while(0)
#define RC5_SetDigitalMode()        do { ANSELCbits.ANSELC5 = 0; } while(0)

// get/set channel_ANC6 aliases
#define channel_ANC6_TRIS                 TRISCbits.TRISC6
#define channel_ANC6_LAT                  LATCbits.LATC6
#define channel_ANC6_PORT                 PORTCbits.RC6
#define channel_ANC6_WPU                  WPUCbits.WPUC6
#define channel_ANC6_OD                   ODCONCbits.ODCC6
#define channel_ANC6_ANS                  ANSELCbits.ANSELC6
#define channel_ANC6_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define channel_ANC6_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define channel_ANC6_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define channel_ANC6_GetValue()           PORTCbits.RC6
#define channel_ANC6_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define channel_ANC6_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define channel_ANC6_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define channel_ANC6_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define channel_ANC6_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define channel_ANC6_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define channel_ANC6_SetAnalogMode()      do { ANSELCbits.ANSELC6 = 1; } while(0)
#define channel_ANC6_SetDigitalMode()     do { ANSELCbits.ANSELC6 = 0; } while(0)

// get/set channel_ANC7 aliases
#define channel_ANC7_TRIS                 TRISCbits.TRISC7
#define channel_ANC7_LAT                  LATCbits.LATC7
#define channel_ANC7_PORT                 PORTCbits.RC7
#define channel_ANC7_WPU                  WPUCbits.WPUC7
#define channel_ANC7_OD                   ODCONCbits.ODCC7
#define channel_ANC7_ANS                  ANSELCbits.ANSELC7
#define channel_ANC7_SetHigh()            do { LATCbits.LATC7 = 1; } while(0)
#define channel_ANC7_SetLow()             do { LATCbits.LATC7 = 0; } while(0)
#define channel_ANC7_Toggle()             do { LATCbits.LATC7 = ~LATCbits.LATC7; } while(0)
#define channel_ANC7_GetValue()           PORTCbits.RC7
#define channel_ANC7_SetDigitalInput()    do { TRISCbits.TRISC7 = 1; } while(0)
#define channel_ANC7_SetDigitalOutput()   do { TRISCbits.TRISC7 = 0; } while(0)
#define channel_ANC7_SetPullup()          do { WPUCbits.WPUC7 = 1; } while(0)
#define channel_ANC7_ResetPullup()        do { WPUCbits.WPUC7 = 0; } while(0)
#define channel_ANC7_SetPushPull()        do { ODCONCbits.ODCC7 = 0; } while(0)
#define channel_ANC7_SetOpenDrain()       do { ODCONCbits.ODCC7 = 1; } while(0)
#define channel_ANC7_SetAnalogMode()      do { ANSELCbits.ANSELC7 = 1; } while(0)
#define channel_ANC7_SetDigitalMode()     do { ANSELCbits.ANSELC7 = 0; } while(0)

// get/set RD0 procedures
#define RD0_SetHigh()            do { LATDbits.LATD0 = 1; } while(0)
#define RD0_SetLow()             do { LATDbits.LATD0 = 0; } while(0)
#define RD0_Toggle()             do { LATDbits.LATD0 = ~LATDbits.LATD0; } while(0)
#define RD0_GetValue()              PORTDbits.RD0
#define RD0_SetDigitalInput()    do { TRISDbits.TRISD0 = 1; } while(0)
#define RD0_SetDigitalOutput()   do { TRISDbits.TRISD0 = 0; } while(0)
#define RD0_SetPullup()             do { WPUDbits.WPUD0 = 1; } while(0)
#define RD0_ResetPullup()           do { WPUDbits.WPUD0 = 0; } while(0)
#define RD0_SetAnalogMode()         do { ANSELDbits.ANSELD0 = 1; } while(0)
#define RD0_SetDigitalMode()        do { ANSELDbits.ANSELD0 = 0; } while(0)

// get/set RD2 procedures
#define RD2_SetHigh()            do { LATDbits.LATD2 = 1; } while(0)
#define RD2_SetLow()             do { LATDbits.LATD2 = 0; } while(0)
#define RD2_Toggle()             do { LATDbits.LATD2 = ~LATDbits.LATD2; } while(0)
#define RD2_GetValue()              PORTDbits.RD2
#define RD2_SetDigitalInput()    do { TRISDbits.TRISD2 = 1; } while(0)
#define RD2_SetDigitalOutput()   do { TRISDbits.TRISD2 = 0; } while(0)
#define RD2_SetPullup()             do { WPUDbits.WPUD2 = 1; } while(0)
#define RD2_ResetPullup()           do { WPUDbits.WPUD2 = 0; } while(0)
#define RD2_SetAnalogMode()         do { ANSELDbits.ANSELD2 = 1; } while(0)
#define RD2_SetDigitalMode()        do { ANSELDbits.ANSELD2 = 0; } while(0)

// get/set CS aliases
#define CS_TRIS                 TRISDbits.TRISD3
#define CS_LAT                  LATDbits.LATD3
#define CS_PORT                 PORTDbits.RD3
#define CS_WPU                  WPUDbits.WPUD3
#define CS_OD                   ODCONDbits.ODCD3
#define CS_ANS                  ANSELDbits.ANSELD3
#define CS_SetHigh()            do { LATDbits.LATD3 = 1; } while(0)
#define CS_SetLow()             do { LATDbits.LATD3 = 0; } while(0)
#define CS_Toggle()             do { LATDbits.LATD3 = ~LATDbits.LATD3; } while(0)
#define CS_GetValue()           PORTDbits.RD3
#define CS_SetDigitalInput()    do { TRISDbits.TRISD3 = 1; } while(0)
#define CS_SetDigitalOutput()   do { TRISDbits.TRISD3 = 0; } while(0)
#define CS_SetPullup()          do { WPUDbits.WPUD3 = 1; } while(0)
#define CS_ResetPullup()        do { WPUDbits.WPUD3 = 0; } while(0)
#define CS_SetPushPull()        do { ODCONDbits.ODCD3 = 0; } while(0)
#define CS_SetOpenDrain()       do { ODCONDbits.ODCD3 = 1; } while(0)
#define CS_SetAnalogMode()      do { ANSELDbits.ANSELD3 = 1; } while(0)
#define CS_SetDigitalMode()     do { ANSELDbits.ANSELD3 = 0; } while(0)

// get/set HLED aliases
#define HLED_TRIS                 TRISDbits.TRISD4
#define HLED_LAT                  LATDbits.LATD4
#define HLED_PORT                 PORTDbits.RD4
#define HLED_WPU                  WPUDbits.WPUD4
#define HLED_OD                   ODCONDbits.ODCD4
#define HLED_ANS                  ANSELDbits.ANSELD4
#define HLED_SetHigh()            do { LATDbits.LATD4 = 1; } while(0)
#define HLED_SetLow()             do { LATDbits.LATD4 = 0; } while(0)
#define HLED_Toggle()             do { LATDbits.LATD4 = ~LATDbits.LATD4; } while(0)
#define HLED_GetValue()           PORTDbits.RD4
#define HLED_SetDigitalInput()    do { TRISDbits.TRISD4 = 1; } while(0)
#define HLED_SetDigitalOutput()   do { TRISDbits.TRISD4 = 0; } while(0)
#define HLED_SetPullup()          do { WPUDbits.WPUD4 = 1; } while(0)
#define HLED_ResetPullup()        do { WPUDbits.WPUD4 = 0; } while(0)
#define HLED_SetPushPull()        do { ODCONDbits.ODCD4 = 0; } while(0)
#define HLED_SetOpenDrain()       do { ODCONDbits.ODCD4 = 1; } while(0)
#define HLED_SetAnalogMode()      do { ANSELDbits.ANSELD4 = 1; } while(0)
#define HLED_SetDigitalMode()     do { ANSELDbits.ANSELD4 = 0; } while(0)

// get/set channel_AND5 aliases
#define channel_AND5_TRIS                 TRISDbits.TRISD5
#define channel_AND5_LAT                  LATDbits.LATD5
#define channel_AND5_PORT                 PORTDbits.RD5
#define channel_AND5_WPU                  WPUDbits.WPUD5
#define channel_AND5_OD                   ODCONDbits.ODCD5
#define channel_AND5_ANS                  ANSELDbits.ANSELD5
#define channel_AND5_SetHigh()            do { LATDbits.LATD5 = 1; } while(0)
#define channel_AND5_SetLow()             do { LATDbits.LATD5 = 0; } while(0)
#define channel_AND5_Toggle()             do { LATDbits.LATD5 = ~LATDbits.LATD5; } while(0)
#define channel_AND5_GetValue()           PORTDbits.RD5
#define channel_AND5_SetDigitalInput()    do { TRISDbits.TRISD5 = 1; } while(0)
#define channel_AND5_SetDigitalOutput()   do { TRISDbits.TRISD5 = 0; } while(0)
#define channel_AND5_SetPullup()          do { WPUDbits.WPUD5 = 1; } while(0)
#define channel_AND5_ResetPullup()        do { WPUDbits.WPUD5 = 0; } while(0)
#define channel_AND5_SetPushPull()        do { ODCONDbits.ODCD5 = 0; } while(0)
#define channel_AND5_SetOpenDrain()       do { ODCONDbits.ODCD5 = 1; } while(0)
#define channel_AND5_SetAnalogMode()      do { ANSELDbits.ANSELD5 = 1; } while(0)
#define channel_AND5_SetDigitalMode()     do { ANSELDbits.ANSELD5 = 0; } while(0)

// get/set RD6 procedures
#define RD6_SetHigh()            do { LATDbits.LATD6 = 1; } while(0)
#define RD6_SetLow()             do { LATDbits.LATD6 = 0; } while(0)
#define RD6_Toggle()             do { LATDbits.LATD6 = ~LATDbits.LATD6; } while(0)
#define RD6_GetValue()              PORTDbits.RD6
#define RD6_SetDigitalInput()    do { TRISDbits.TRISD6 = 1; } while(0)
#define RD6_SetDigitalOutput()   do { TRISDbits.TRISD6 = 0; } while(0)
#define RD6_SetPullup()             do { WPUDbits.WPUD6 = 1; } while(0)
#define RD6_ResetPullup()           do { WPUDbits.WPUD6 = 0; } while(0)
#define RD6_SetAnalogMode()         do { ANSELDbits.ANSELD6 = 1; } while(0)
#define RD6_SetDigitalMode()        do { ANSELDbits.ANSELD6 = 0; } while(0)

// get/set MCZ_PWM aliases
#define MCZ_PWM_TRIS                 TRISDbits.TRISD7
#define MCZ_PWM_LAT                  LATDbits.LATD7
#define MCZ_PWM_PORT                 PORTDbits.RD7
#define MCZ_PWM_WPU                  WPUDbits.WPUD7
#define MCZ_PWM_OD                   ODCONDbits.ODCD7
#define MCZ_PWM_ANS                  ANSELDbits.ANSELD7
#define MCZ_PWM_SetHigh()            do { LATDbits.LATD7 = 1; } while(0)
#define MCZ_PWM_SetLow()             do { LATDbits.LATD7 = 0; } while(0)
#define MCZ_PWM_Toggle()             do { LATDbits.LATD7 = ~LATDbits.LATD7; } while(0)
#define MCZ_PWM_GetValue()           PORTDbits.RD7
#define MCZ_PWM_SetDigitalInput()    do { TRISDbits.TRISD7 = 1; } while(0)
#define MCZ_PWM_SetDigitalOutput()   do { TRISDbits.TRISD7 = 0; } while(0)
#define MCZ_PWM_SetPullup()          do { WPUDbits.WPUD7 = 1; } while(0)
#define MCZ_PWM_ResetPullup()        do { WPUDbits.WPUD7 = 0; } while(0)
#define MCZ_PWM_SetPushPull()        do { ODCONDbits.ODCD7 = 0; } while(0)
#define MCZ_PWM_SetOpenDrain()       do { ODCONDbits.ODCD7 = 1; } while(0)
#define MCZ_PWM_SetAnalogMode()      do { ANSELDbits.ANSELD7 = 1; } while(0)
#define MCZ_PWM_SetDigitalMode()     do { ANSELDbits.ANSELD7 = 0; } while(0)

// get/set MCZ_CS aliases
#define MCZ_CS_TRIS                 TRISEbits.TRISE0
#define MCZ_CS_LAT                  LATEbits.LATE0
#define MCZ_CS_PORT                 PORTEbits.RE0
#define MCZ_CS_WPU                  WPUEbits.WPUE0
#define MCZ_CS_OD                   ODCONEbits.ODCE0
#define MCZ_CS_ANS                  ANSELEbits.ANSELE0
#define MCZ_CS_SetHigh()            do { LATEbits.LATE0 = 1; } while(0)
#define MCZ_CS_SetLow()             do { LATEbits.LATE0 = 0; } while(0)
#define MCZ_CS_Toggle()             do { LATEbits.LATE0 = ~LATEbits.LATE0; } while(0)
#define MCZ_CS_GetValue()           PORTEbits.RE0
#define MCZ_CS_SetDigitalInput()    do { TRISEbits.TRISE0 = 1; } while(0)
#define MCZ_CS_SetDigitalOutput()   do { TRISEbits.TRISE0 = 0; } while(0)
#define MCZ_CS_SetPullup()          do { WPUEbits.WPUE0 = 1; } while(0)
#define MCZ_CS_ResetPullup()        do { WPUEbits.WPUE0 = 0; } while(0)
#define MCZ_CS_SetPushPull()        do { ODCONEbits.ODCE0 = 0; } while(0)
#define MCZ_CS_SetOpenDrain()       do { ODCONEbits.ODCE0 = 1; } while(0)
#define MCZ_CS_SetAnalogMode()      do { ANSELEbits.ANSELE0 = 1; } while(0)
#define MCZ_CS_SetDigitalMode()     do { ANSELEbits.ANSELE0 = 0; } while(0)

// get/set TIC_CS aliases
#define TIC_CS_TRIS                 TRISEbits.TRISE1
#define TIC_CS_LAT                  LATEbits.LATE1
#define TIC_CS_PORT                 PORTEbits.RE1
#define TIC_CS_WPU                  WPUEbits.WPUE1
#define TIC_CS_OD                   ODCONEbits.ODCE1
#define TIC_CS_ANS                  ANSELEbits.ANSELE1
#define TIC_CS_SetHigh()            do { LATEbits.LATE1 = 1; } while(0)
#define TIC_CS_SetLow()             do { LATEbits.LATE1 = 0; } while(0)
#define TIC_CS_Toggle()             do { LATEbits.LATE1 = ~LATEbits.LATE1; } while(0)
#define TIC_CS_GetValue()           PORTEbits.RE1
#define TIC_CS_SetDigitalInput()    do { TRISEbits.TRISE1 = 1; } while(0)
#define TIC_CS_SetDigitalOutput()   do { TRISEbits.TRISE1 = 0; } while(0)
#define TIC_CS_SetPullup()          do { WPUEbits.WPUE1 = 1; } while(0)
#define TIC_CS_ResetPullup()        do { WPUEbits.WPUE1 = 0; } while(0)
#define TIC_CS_SetPushPull()        do { ODCONEbits.ODCE1 = 0; } while(0)
#define TIC_CS_SetOpenDrain()       do { ODCONEbits.ODCE1 = 1; } while(0)
#define TIC_CS_SetAnalogMode()      do { ANSELEbits.ANSELE1 = 1; } while(0)
#define TIC_CS_SetDigitalMode()     do { ANSELEbits.ANSELE1 = 0; } while(0)

// get/set SS_CS aliases
#define SS_CS_TRIS                 TRISEbits.TRISE2
#define SS_CS_LAT                  LATEbits.LATE2
#define SS_CS_PORT                 PORTEbits.RE2
#define SS_CS_WPU                  WPUEbits.WPUE2
#define SS_CS_OD                   ODCONEbits.ODCE2
#define SS_CS_ANS                  ANSELEbits.ANSELE2
#define SS_CS_SetHigh()            do { LATEbits.LATE2 = 1; } while(0)
#define SS_CS_SetLow()             do { LATEbits.LATE2 = 0; } while(0)
#define SS_CS_Toggle()             do { LATEbits.LATE2 = ~LATEbits.LATE2; } while(0)
#define SS_CS_GetValue()           PORTEbits.RE2
#define SS_CS_SetDigitalInput()    do { TRISEbits.TRISE2 = 1; } while(0)
#define SS_CS_SetDigitalOutput()   do { TRISEbits.TRISE2 = 0; } while(0)
#define SS_CS_SetPullup()          do { WPUEbits.WPUE2 = 1; } while(0)
#define SS_CS_ResetPullup()        do { WPUEbits.WPUE2 = 0; } while(0)
#define SS_CS_SetPushPull()        do { ODCONEbits.ODCE2 = 0; } while(0)
#define SS_CS_SetOpenDrain()       do { ODCONEbits.ODCE2 = 1; } while(0)
#define SS_CS_SetAnalogMode()      do { ANSELEbits.ANSELE2 = 1; } while(0)
#define SS_CS_SetDigitalMode()     do { ANSELEbits.ANSELE2 = 0; } while(0)

// get/set RF0 procedures
#define RF0_SetHigh()            do { LATFbits.LATF0 = 1; } while(0)
#define RF0_SetLow()             do { LATFbits.LATF0 = 0; } while(0)
#define RF0_Toggle()             do { LATFbits.LATF0 = ~LATFbits.LATF0; } while(0)
#define RF0_GetValue()              PORTFbits.RF0
#define RF0_SetDigitalInput()    do { TRISFbits.TRISF0 = 1; } while(0)
#define RF0_SetDigitalOutput()   do { TRISFbits.TRISF0 = 0; } while(0)
#define RF0_SetPullup()             do { WPUFbits.WPUF0 = 1; } while(0)
#define RF0_ResetPullup()           do { WPUFbits.WPUF0 = 0; } while(0)
#define RF0_SetAnalogMode()         do { ANSELFbits.ANSELF0 = 1; } while(0)
#define RF0_SetDigitalMode()        do { ANSELFbits.ANSELF0 = 0; } while(0)

// get/set RF1 procedures
#define RF1_SetHigh()            do { LATFbits.LATF1 = 1; } while(0)
#define RF1_SetLow()             do { LATFbits.LATF1 = 0; } while(0)
#define RF1_Toggle()             do { LATFbits.LATF1 = ~LATFbits.LATF1; } while(0)
#define RF1_GetValue()              PORTFbits.RF1
#define RF1_SetDigitalInput()    do { TRISFbits.TRISF1 = 1; } while(0)
#define RF1_SetDigitalOutput()   do { TRISFbits.TRISF1 = 0; } while(0)
#define RF1_SetPullup()             do { WPUFbits.WPUF1 = 1; } while(0)
#define RF1_ResetPullup()           do { WPUFbits.WPUF1 = 0; } while(0)
#define RF1_SetAnalogMode()         do { ANSELFbits.ANSELF1 = 1; } while(0)
#define RF1_SetDigitalMode()        do { ANSELFbits.ANSELF1 = 0; } while(0)

// get/set RF2 procedures
#define RF2_SetHigh()            do { LATFbits.LATF2 = 1; } while(0)
#define RF2_SetLow()             do { LATFbits.LATF2 = 0; } while(0)
#define RF2_Toggle()             do { LATFbits.LATF2 = ~LATFbits.LATF2; } while(0)
#define RF2_GetValue()              PORTFbits.RF2
#define RF2_SetDigitalInput()    do { TRISFbits.TRISF2 = 1; } while(0)
#define RF2_SetDigitalOutput()   do { TRISFbits.TRISF2 = 0; } while(0)
#define RF2_SetPullup()             do { WPUFbits.WPUF2 = 1; } while(0)
#define RF2_ResetPullup()           do { WPUFbits.WPUF2 = 0; } while(0)
#define RF2_SetAnalogMode()         do { ANSELFbits.ANSELF2 = 1; } while(0)
#define RF2_SetDigitalMode()        do { ANSELFbits.ANSELF2 = 0; } while(0)

// get/set DERE aliases
#define DERE_TRIS                 TRISFbits.TRISF3
#define DERE_LAT                  LATFbits.LATF3
#define DERE_PORT                 PORTFbits.RF3
#define DERE_WPU                  WPUFbits.WPUF3
#define DERE_OD                   ODCONFbits.ODCF3
#define DERE_ANS                  ANSELFbits.ANSELF3
#define DERE_SetHigh()            do { LATFbits.LATF3 = 1; } while(0)
#define DERE_SetLow()             do { LATFbits.LATF3 = 0; } while(0)
#define DERE_Toggle()             do { LATFbits.LATF3 = ~LATFbits.LATF3; } while(0)
#define DERE_GetValue()           PORTFbits.RF3
#define DERE_SetDigitalInput()    do { TRISFbits.TRISF3 = 1; } while(0)
#define DERE_SetDigitalOutput()   do { TRISFbits.TRISF3 = 0; } while(0)
#define DERE_SetPullup()          do { WPUFbits.WPUF3 = 1; } while(0)
#define DERE_ResetPullup()        do { WPUFbits.WPUF3 = 0; } while(0)
#define DERE_SetPushPull()        do { ODCONFbits.ODCF3 = 0; } while(0)
#define DERE_SetOpenDrain()       do { ODCONFbits.ODCF3 = 1; } while(0)
#define DERE_SetAnalogMode()      do { ANSELFbits.ANSELF3 = 1; } while(0)
#define DERE_SetDigitalMode()     do { ANSELFbits.ANSELF3 = 0; } while(0)

// get/set IO_RF4 aliases
#define IO_RF4_TRIS                 TRISFbits.TRISF4
#define IO_RF4_LAT                  LATFbits.LATF4
#define IO_RF4_PORT                 PORTFbits.RF4
#define IO_RF4_WPU                  WPUFbits.WPUF4
#define IO_RF4_OD                   ODCONFbits.ODCF4
#define IO_RF4_ANS                  ANSELFbits.ANSELF4
#define IO_RF4_SetHigh()            do { LATFbits.LATF4 = 1; } while(0)
#define IO_RF4_SetLow()             do { LATFbits.LATF4 = 0; } while(0)
#define IO_RF4_Toggle()             do { LATFbits.LATF4 = ~LATFbits.LATF4; } while(0)
#define IO_RF4_GetValue()           PORTFbits.RF4
#define IO_RF4_SetDigitalInput()    do { TRISFbits.TRISF4 = 1; } while(0)
#define IO_RF4_SetDigitalOutput()   do { TRISFbits.TRISF4 = 0; } while(0)
#define IO_RF4_SetPullup()          do { WPUFbits.WPUF4 = 1; } while(0)
#define IO_RF4_ResetPullup()        do { WPUFbits.WPUF4 = 0; } while(0)
#define IO_RF4_SetPushPull()        do { ODCONFbits.ODCF4 = 0; } while(0)
#define IO_RF4_SetOpenDrain()       do { ODCONFbits.ODCF4 = 1; } while(0)
#define IO_RF4_SetAnalogMode()      do { ANSELFbits.ANSELF4 = 1; } while(0)
#define IO_RF4_SetDigitalMode()     do { ANSELFbits.ANSELF4 = 0; } while(0)

// get/set IO_RF5 aliases
#define IO_RF5_TRIS                 TRISFbits.TRISF5
#define IO_RF5_LAT                  LATFbits.LATF5
#define IO_RF5_PORT                 PORTFbits.RF5
#define IO_RF5_WPU                  WPUFbits.WPUF5
#define IO_RF5_OD                   ODCONFbits.ODCF5
#define IO_RF5_ANS                  ANSELFbits.ANSELF5
#define IO_RF5_SetHigh()            do { LATFbits.LATF5 = 1; } while(0)
#define IO_RF5_SetLow()             do { LATFbits.LATF5 = 0; } while(0)
#define IO_RF5_Toggle()             do { LATFbits.LATF5 = ~LATFbits.LATF5; } while(0)
#define IO_RF5_GetValue()           PORTFbits.RF5
#define IO_RF5_SetDigitalInput()    do { TRISFbits.TRISF5 = 1; } while(0)
#define IO_RF5_SetDigitalOutput()   do { TRISFbits.TRISF5 = 0; } while(0)
#define IO_RF5_SetPullup()          do { WPUFbits.WPUF5 = 1; } while(0)
#define IO_RF5_ResetPullup()        do { WPUFbits.WPUF5 = 0; } while(0)
#define IO_RF5_SetPushPull()        do { ODCONFbits.ODCF5 = 0; } while(0)
#define IO_RF5_SetOpenDrain()       do { ODCONFbits.ODCF5 = 1; } while(0)
#define IO_RF5_SetAnalogMode()      do { ANSELFbits.ANSELF5 = 1; } while(0)
#define IO_RF5_SetDigitalMode()     do { ANSELFbits.ANSELF5 = 0; } while(0)

// get/set IO_RF6 aliases
#define IO_RF6_TRIS                 TRISFbits.TRISF6
#define IO_RF6_LAT                  LATFbits.LATF6
#define IO_RF6_PORT                 PORTFbits.RF6
#define IO_RF6_WPU                  WPUFbits.WPUF6
#define IO_RF6_OD                   ODCONFbits.ODCF6
#define IO_RF6_ANS                  ANSELFbits.ANSELF6
#define IO_RF6_SetHigh()            do { LATFbits.LATF6 = 1; } while(0)
#define IO_RF6_SetLow()             do { LATFbits.LATF6 = 0; } while(0)
#define IO_RF6_Toggle()             do { LATFbits.LATF6 = ~LATFbits.LATF6; } while(0)
#define IO_RF6_GetValue()           PORTFbits.RF6
#define IO_RF6_SetDigitalInput()    do { TRISFbits.TRISF6 = 1; } while(0)
#define IO_RF6_SetDigitalOutput()   do { TRISFbits.TRISF6 = 0; } while(0)
#define IO_RF6_SetPullup()          do { WPUFbits.WPUF6 = 1; } while(0)
#define IO_RF6_ResetPullup()        do { WPUFbits.WPUF6 = 0; } while(0)
#define IO_RF6_SetPushPull()        do { ODCONFbits.ODCF6 = 0; } while(0)
#define IO_RF6_SetOpenDrain()       do { ODCONFbits.ODCF6 = 1; } while(0)
#define IO_RF6_SetAnalogMode()      do { ANSELFbits.ANSELF6 = 1; } while(0)
#define IO_RF6_SetDigitalMode()     do { ANSELFbits.ANSELF6 = 0; } while(0)

// get/set IO_RF7 aliases
#define IO_RF7_TRIS                 TRISFbits.TRISF7
#define IO_RF7_LAT                  LATFbits.LATF7
#define IO_RF7_PORT                 PORTFbits.RF7
#define IO_RF7_WPU                  WPUFbits.WPUF7
#define IO_RF7_OD                   ODCONFbits.ODCF7
#define IO_RF7_ANS                  ANSELFbits.ANSELF7
#define IO_RF7_SetHigh()            do { LATFbits.LATF7 = 1; } while(0)
#define IO_RF7_SetLow()             do { LATFbits.LATF7 = 0; } while(0)
#define IO_RF7_Toggle()             do { LATFbits.LATF7 = ~LATFbits.LATF7; } while(0)
#define IO_RF7_GetValue()           PORTFbits.RF7
#define IO_RF7_SetDigitalInput()    do { TRISFbits.TRISF7 = 1; } while(0)
#define IO_RF7_SetDigitalOutput()   do { TRISFbits.TRISF7 = 0; } while(0)
#define IO_RF7_SetPullup()          do { WPUFbits.WPUF7 = 1; } while(0)
#define IO_RF7_ResetPullup()        do { WPUFbits.WPUF7 = 0; } while(0)
#define IO_RF7_SetPushPull()        do { ODCONFbits.ODCF7 = 0; } while(0)
#define IO_RF7_SetOpenDrain()       do { ODCONFbits.ODCF7 = 1; } while(0)
#define IO_RF7_SetAnalogMode()      do { ANSELFbits.ANSELF7 = 1; } while(0)
#define IO_RF7_SetDigitalMode()     do { ANSELFbits.ANSELF7 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);



/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handler for the IOCBF6 pin functionality
 * @Example
    IOCBF6_ISR();
 */
void IOCBF6_ISR(void);

/**
  @Summary
    Interrupt Handler Setter for IOCBF6 pin interrupt-on-change functionality

  @Description
    Allows selecting an interrupt handler for IOCBF6 at application runtime
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    InterruptHandler function pointer.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF6_SetInterruptHandler(MyInterruptHandler);

*/
void IOCBF6_SetInterruptHandler(void (* InterruptHandler)(void));

/**
  @Summary
    Dynamic Interrupt Handler for IOCBF6 pin

  @Description
    This is a dynamic interrupt handler to be used together with the IOCBF6_SetInterruptHandler() method.
    This handler is called every time the IOCBF6 ISR is executed and allows any function to be registered at runtime.
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF6_SetInterruptHandler(IOCBF6_InterruptHandler);

*/
extern void (*IOCBF6_InterruptHandler)(void);

/**
  @Summary
    Default Interrupt Handler for IOCBF6 pin

  @Description
    This is a predefined interrupt handler to be used together with the IOCBF6_SetInterruptHandler() method.
    This handler is called every time the IOCBF6 ISR is executed. 
    
  @Preconditions
    Pin Manager intializer called

  @Returns
    None.

  @Param
    None.

  @Example
    PIN_MANAGER_Initialize();
    IOCBF6_SetInterruptHandler(IOCBF6_DefaultInterruptHandler);

*/
void IOCBF6_DefaultInterruptHandler(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/