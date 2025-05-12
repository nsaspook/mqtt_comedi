/* 
 * File:   bmcdio.h
 * Author: root
 *
 * Created on May 12, 2025, 11:20 AM
 */

#ifndef BMCDIO_H
#define BMCDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <fcntl.h>           /* Definition of AT_* constants */
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>        
#include <hidapi/hidapi.h>
#include "tic12400.h"
#include "mc33996.h"

#define MCP2210_DRIVER "V0.8"

#define MAX_STR                                 255
#define OPERATION_SUCCESSFUL                    0
#define ERROR_UNABLE_TO_OPEN_DEVICE             -1
#define ERROR_UNABLE_TO_WRITE_TO_DEVICE         -2
#define ERROR_UNABLE_TO_READ_FROM_DEVICE        -3
#define ERROR_INVALID_DEVICE_HANDLE             -99

#define COMMAND_BUFFER_LENGTH                   64
#define RESPONSE_BUFFER_LENGTH                  64

#define SPI_STATUS_FINISHED_NO_DATA_TO_SEND     0x10
#define SPI_STATUS_STARTED_NO_DATA_TO_RECEIVE   0x20
#define SPI_STATUS_SUCCESSFUL                   0x30
#define SPI_STATUS_DELAY_US                     0x01        

#define MC33996_DATA                           6
#define MC33996_DATA_LEN                       8

        /*
         * HIDAPI I/O structure
         */
        typedef struct {
                hid_device *handle;
                struct hid_device_info *devs, *cur_dev;
                uint8_t buf[COMMAND_BUFFER_LENGTH]; // command buffer written to MCP2210
                uint8_t rbuf[RESPONSE_BUFFER_LENGTH]; // response buffer
                int32_t res; // # of bytes sent from hid_read(), hid_write() functions
        } mcp2210_spi_type;

        void cbufs();
        int32_t SendUSBCmd(hid_device *, uint8_t *, uint8_t *);
        int32_t get_usb_res(void);
        void sleep_us(const uint32_t);
        bool get_MCP2210_ext_interrupt(void);
        int32_t cancel_spi_transfer(void);
        bool SPI_WriteRead(hid_device *, uint8_t *, uint8_t *);
        bool SPI_MCP2210_WriteRead(uint8_t* pTransmitData, const size_t txSize, uint8_t* pReceiveData, const size_t rxSize);
        void setup_tic12400_transfer(void);
        void get_tic12400_transfer(void);
        void setup_bmx160_transfer(uint8_t);
        void get_bmx160_transfer(void);
        void show_bmx160_transfer(void);
        void move_bmx160_transfer_data(uint8_t *);
        void move_bmx160_transfer_status(uint8_t *);
        uint8_t bmx160_get(uint8_t, uint8_t);
        uint8_t bmx160_set(uint8_t, uint8_t);
        uint8_t bmx160_set3(uint8_t *, uint8_t);
        void bmx160_update(void);
        void mc33996_init(void);
        bool mc33996_check(void);
        void mc33996_set(uint8_t, uint8_t, uint8_t);
        void setup_mc33996_transfer(uint8_t);
        void get_mc33996_transfer(void);
        void mc33996_update(void);
        mcp2210_spi_type* hidrawapi_mcp2210_init(const wchar_t *serial_number);


#ifdef __cplusplus
}
#endif

#endif /* BMCDIO_H */

