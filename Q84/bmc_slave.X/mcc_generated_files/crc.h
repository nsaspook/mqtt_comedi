/**
  CRC Generated Driver API Header File

  @Company
    Microchip Technology Inc.

  @File Name
    crc.h

  @Summary
    This is the generated header file for the CRC driver using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for CRC.
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.81.8
        Device            :  PIC18F57Q84
        Driver Version    :  1.0.0
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.36 and above or later
        MPLAB             :  MPLAB X 6.00
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

#ifndef CRC_H
#define CRC_H

#include <stdint.h>
#include <stdbool.h>

/**
  @Summary
    Initializes the CRC.

  @Description
    This function initializes the CRC Registers.
    This function must be called before any other CRC function is called.

  @Preconditions
    None

  @Param
    None

  @Returns
    None

  @Comment
   
   
  @Example
    <code>
    uint32_t buffer [] = {0x55,0x66,0x77,0x88};
    uint32_t crcVal;
    uint32_t length = sizeof(buffer);
    uint32_t value = 0xff00;

    // Initialize the CRC module
    CRC_Initialize();

    // Start CRC
    CRC_StartCrc();

    CRC_WriteData(value);

    while (CRC_IsCrcBusy());

    // Read CRC check value
    crcVal = CRC_GetCalculatedResult(false,0x00);
    </code>
*/
void CRC_Initialize(void);

/**
  @Summary
    Starts the CRC module

  @Description
    This routine sets the CRCGO bit of the CRCCON0 register to begin the shifting
    process.

    This routine must be called after the initialization of the CRC module.

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
    uint32_t crcVal;

    // Initialize the CRC module
    CRC_Initialize();

    // Start CRC
    CRC_StartCrc();

    // Write data
    CRC_WriteData(0x55)

    // Check CRC busy?
    while(CRC_IsCrcBusy());

    // Read CRC check value
    crcVal = CRC_GetCalculatedResult(false,0x00);
    </code>
*/
inline void CRC_StartCrc(void);

/**
  @Summary
    Writes data into CRCDATL register pair.

  @Description
    This routine writes data into CRCDATHL register pair.

  @Preconditions
    None.

  @Returns
    1 - if CRC data registers are not full
    0 - if CRC data registers are full

  @Param
    data:  crc input data

  @Example
    <code>
    uint32_t crcVal;

    // Initialize the CRC module
    CRC_Initialize();

    // Start CRC
    CRC_StartCrc();

    // write 8-bit data
    bool retVal = CRC_WriteData(0x55);

    // Check CRC busy?
    while(CRC_IsCrcBusy());

    // Read CRC check value
    crcVal = CRC_GetCalculatedResult(false,0x00);
    </code>
*/
bool CRC_WriteData(uint32_t data);

/**
  @Summary
   Reads crc check value.

  @Description
    This routine reads and returns the Normal or reverse value.

  @Preconditions
    None.

  @Returns
    None.

  @Param
   reverse: false - Normal value, true - Reverse value
   xorValue: value which xored with CRC.

  @Example
     <code>
    uint32_t crcVal;
    // Initialize the CRC module
    CRC_Initialize();

    // Start CRC
    CRC_StartCrc();

    // write 8-bit data
    CRC_WriteData(0x55)

    // Check CRC busy?
    while(CRC_IsCrcBusy());

    // Read CRC check value
    crcVal = CRC_GetCalculatedResult(false,0x00);
    </code>
*/
uint32_t CRC_GetCalculatedResult(bool reverse, uint32_t xorValue);

/**
  @Summary
    Reads the status of BUSY bit of CRCCON0 register.

  @Description
    This routine returns the status of the BUSY bit of CRCCON0 register to check
    CRC calculation is over or not.

  @Preconditions
    None.

  @Returns
    1 - CRC busy.
    0 - CRC not busy.

  @Param
    None.

  @Example
    <code>
    uint32_t crcVal;
    // Initialize the CRC module
    CRC_Initialize();

    // Start CRC
    CRC_StartCrc();

    // write 8-bit data
    CRC_WriteData(0x55)

    // Check CRC busy?
    while(CRC_IsCrcBusy());

    // Read calculated CRC
    crcVal = CRC_GetCalculatedResult();
    </code>
*/
inline bool CRC_IsCrcBusy(void);

/**
  @Summary
    Starts the program memory scan

  @Description
    This routine starts the scanning process

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
     uint32_t crcVal;
    // Initialize the CRC SCAN module
    CRC_Initialize();

    // Sets SCAN address limit
    CRC_SetScannerAddressLimit(0x0000,0x07FF);

    // Start Scanner
    CRC_StartScanner();

    // Scan completed?
    while(CRC_IsCrcBusy() ||  CRC_IsScannerBusy());

    // Stop Scanner
    CRC_StopScanner();

    // Read CRC check value
    crcVal = CRC_CalculatedResultGet(false,0x00);
    </code>
*/
inline void CRC_StartScanner(void);

/**
  @Summary
    Stops the program memory scan

  @Description
    This routine Stops the scanning process

  @Preconditions
    None.

  @Returns
    None.

  @Param
    None.

  @Example
    <code>
     uint32_t crcVal;
    // Initialize the CRC SCAN module
    CRC_Initialize();

    // Sets SCAN address limit
    CRC_SetScannerAddressLimit(0x0000,0x07FF);

    // Start Scanner
    CRC_StartScanner();

    // Scan completed?
    while(CRC_IsCrcBusy() ||  CRC_IsScannerBusy());

    // Stop Scanner
    CRC_StopScanner();

    // Read CRC check value
    crcVal = CRC_CalculatedResultGet(false,0x00);
    </code>
*/
inline void CRC_StopScanner(void);

/**
  @Summary
    Sets the memory address limit for scanning

  @Description
    This routine loads  address limits into the SCANLADRH/L and SCANHADRH/L register pairs.

  @Preconditions
    None.

  @Returns
    None.

  @Param
   startAddr: Starting address of memory block.
   endAddr:   Ending address of memory block.

  @Example
    <code>
     uint32_t crcVal;
    // Initialize the CRC SCAN module
    CRC_Initialize();

    // Sets SCAN address limit
    CRC_SetScannerAddressLimit(0x0000,0x07FF);

    // Start Scanner
    CRC_StartScanner();

    // Scan completed?
    while(CRC_IsCrcBusy() ||  CRC_IsScannerBusy());

    // Stop Scanner
    CRC_StopScanner();

    // Read CRC check value
    crcVal = CRC_CalculatedResultGet(false,0x00);
    </code>
*/
void CRC_SetScannerAddressLimit(uint24_t startAddr, uint24_t endAddr);

/**
  @Summary
    Returns the status of BUSY bit of SCANCON0 register.

  @Description
    This routine returns the status of BUSY bit of SCANCON0 register.

  @Preconditions
    None.

  @Returns
    0 - SCAN not yet started.
    1- SCAN is in progress.

  @Param
    None.

  @Example
    <code>
     uint32_t crcVal;
    // Initialize the CRC SCAN module
    CRC_Initialize();

    // Wait if scanner is in progress.
    While(CRC_IsScannerBusy()!=1);

    // Sets SCAN address limit
    CRC_SetAddressLimit(0x0000,0x07FF);

    // Start Scanner
    CRC_StartScanner();

    // Scan completed?
    while(CRC_IsCrcBusy() ||  CRC_IsScannerBusy());

    // Stop Scanner
    CRC_StopScanner();

    // Read CRC check value
    crcVal = CRC_GetCalculatedResult(false,0x00);
    </code>
*/
inline bool CRC_IsScannerBusy(void);


#endif //CRC_H