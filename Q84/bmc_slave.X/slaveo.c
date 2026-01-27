
/* Parts of this code were modified from
 *  http://www.d.umn.edu/~cprince/PubRes/Hardware/SPI/
 * examples
 *
 * Fully interrupt driven SPI slave ADC for OPI
 *
 */
/** \file slaveo.c
 * BMCboard SPI DAQ slave for the Orange PI
 */

#include <xc.h>
#include "slaveo.h"

volatile bool failure = false;
volatile uint8_t in_buf1 = 0x19, in_buf2 = 0x57, in_buf3 = 0x07;
static uint8_t *tmp_buf4 = (void *) &ha_daq_calib.scaler4;
static uint8_t *tmp_buf5 = (void *) &ha_daq_calib.scaler5;
volatile bool r_string_ready = false, bmc_string_ready = false, update_bmc_string = false;

static void clear_slaveo_flags(void);

void check_slaveo(void) /* SPI Slave error check */
{
	if (SPI2STATUSbits.TXWE) { // check for overruns/collisions
	}
}

static void clear_slaveo_flags(void)
{
	serial_buffer_ss.adc_value = false;
	serial_buffer_ss.make_value = false;
	serial_buffer_ss.dac_value = false;
	serial_buffer_ss.get_value = false;
	serial_buffer_ss.cfg_value = false;
	serial_buffer_ss.cmake_value = false;
	serial_buffer_ss.cget_value = false;
}

/*
 * setup the interrupt call backs and data structures
 * For SPI2 using MODE 3, DON'T USE MODE 0 with Orange PI SPI links
 */
void init_slaveo(void)
{
	uint8_t val = CHECKBYTE;

	SPI2CON0bits.EN = 0;
	SPI2_SetRxInterruptHandler(slaveo_rx_isr);
	SPI2_SetInterruptHandler(slaveo_spi_isr);
	while (!SPI2STATUSbits.RXRE) { // clear the FIFO of data
		val = SPI2RXB;
	}
	serial_buffer_ss.data[BMC_D1] = val;
	SPI2STATUSbits.RXRE = 0;
	SPI2CON2bits.TXR = 1; // FIFO required for transmit
	SPI2CON0bits.EN = 1;
	ADC_SelectContext(CONTEXT_1);
}

/*
 * DAQ_BMC command processor task
 */
void slaveo_rx_isr(void)
{
	uint8_t command;
	uint8_t tmp_buf;

	/* we only get this when the master wants data, the slave never generates one */
	// SPI port #2 SLAVE receiver

	DLED_SetHigh();
	Nop();
#ifdef SLAVE_DEBUG
	if (SPI2INTFbits.RXOIF) {
		spi_stat_ss.rxof_bit++;
	}

	if (SPI2STATUSbits.SPI2RXRE) {
	} else {
		if (spi_comm_ss.PORT_DATA) {
		}
	}
#endif

	data_in2 = SPI2RXB;
	serial_buffer_ss.data[serial_buffer_ss.raw_index] = data_in2;

	// CHAR_GO_BYTES
	// use r_string array to buffer ascii data
	if (serial_buffer_ss.cmake_value) {
		if (serial_buffer_ss.raw_index == CHAR_GO_BYTES) {
			if ((serial_buffer_ss.data[BMC_D1] & 0x07) < BMC_EM540_DATA) { // [0..3]
				/*
				 * uart1 only
				 */
				UART1_Write(serial_buffer_ss.data[BMC_D0]);

				if (!r_string_ready) {
					if (serial_buffer_ss.data[BMC_D0] == STX) { // character sync
						serial_buffer_ss.r_string_index = 0;
						serial_buffer_ss.r_string[serial_buffer_ss.r_string_index] = 0;
					} else {
						serial_buffer_ss.r_string[serial_buffer_ss.r_string_index++] = serial_buffer_ss.data[BMC_D0];
						serial_buffer_ss.r_string_chan = serial_buffer_ss.data[BMC_D1] & 0x03;
						if (serial_buffer_ss.r_string_index >= MAX_STRLEN) {
							serial_buffer_ss.r_string[serial_buffer_ss.r_string_index] = 0;
							serial_buffer_ss.r_string_index = 0;
							r_string_ready = true;
						}
					}
				}
			} else { // [4..7] sync character for type of data requested
				switch (serial_buffer_ss.data[BMC_D0]) {
				case STX:
				case DC1_CMD:
				case DC2_CMD:
				case DC3_CMD:
				case DC4_CMD:
					serial_buffer_ss.r_string_index = 0;
					serial_buffer_ss.r_string[serial_buffer_ss.r_string_index] = 0;
					update_bmc_string = true; // print to log_buffer
					bmc_string_ready = true; // display bmc data on first line
					r_string_ready = true;
					break;
				default: // does nothing
					break;
				}
			}
			serial_buffer_ss.cmake_value = false;
			serial_buffer_ss.raw_index = BMC_CMD;
			spi_stat_ss.txdone_bit++; // number of completed packets
			data_in2 = 0;
		} else {
			data_in2 = 0;
		}
	}

	// PORT_GO_BYTES
	if (serial_buffer_ss.make_value) {
		if (serial_buffer_ss.raw_index == PORT_GO_BYTES) {
			V.bmc_do = serial_buffer_ss.data[BMC_D0];
			V.bmc_do += (uint32_t) (serial_buffer_ss.data[BMC_D1] << 8u)&0x0000ff00;
			data_in2 = 0;
			serial_buffer_ss.make_value = false;
			serial_buffer_ss.raw_index = BMC_CMD;
			spi_stat_ss.txdone_bit++; // number of completed packets
			data_in2 = 0;
		} else {
			data_in2 = 0;
		}
	}

	// DAC_GO_BYTES
	if (serial_buffer_ss.dac_value) {
		if (serial_buffer_ss.raw_index == DAC_GO_BYTES) {
			V.bmc_ao = serial_buffer_ss.data[BMC_D0];
			serial_buffer_ss.dac_value = false;
			serial_buffer_ss.raw_index = BMC_CMD;
			spi_stat_ss.txdone_bit++; // number of completed packets
			data_in2 = 0;
		} else {
			data_in2 = 0;
		}
	}

	// CHAR_GET_BYTES
	// use r_string array to buffer ascii data
	if (serial_buffer_ss.cget_value) {
		if (serial_buffer_ss.raw_index == CHAR_GET_BYTES) {
			if ((serial_buffer_ss.data[BMC_D1] & 0x07) < BMC_EM540_DATA) { // [0..3]
				/*
				 * uart1 only
				 */
				if (UART1_is_rx_ready()) {
					tmp_buf = UART1_Read();
				} else {
					tmp_buf = 0;
				}
				SPI2TXB = tmp_buf;
			} else { // [4..7]
				tmp_buf = 0x57;
				if (update_bmc_string == true) { // log_buffer has been updated
					tmp_buf = log_buffer[BMC4.pos++];
				}
				SPI2TXB = tmp_buf;
				spi_stat_ss.bmc_counts++;
			}
			serial_buffer_ss.cget_value = false;
			serial_buffer_ss.raw_index = BMC_CMD;
			spi_stat_ss.txdone_bit++; // number of completed packets

			data_in2 = 0;
		} else {
			if ((serial_buffer_ss.data[BMC_D1] & 0x07) < BMC_EM540_DATA) { // [0..3]
				if (serial_buffer_ss.raw_index == BMC_D0) {
					tmp_buf = 0x00; //
				} else {
					tmp_buf = UART1_is_rx_ready(); // new data is ready
				}
			} else {
				tmp_buf = log_buffer[BMC4.pos];
			}
			SPI2TXB = tmp_buf;
			data_in2 = 0;
		}
	}

	// PORT_GET_BYTES
	if (serial_buffer_ss.get_value) {
		if (serial_buffer_ss.raw_index == PORT_GET_BYTES) {
			tmp_buf = (uint8_t) in_buf3;
			SPI2TXB = tmp_buf;
			serial_buffer_ss.get_value = false;
			serial_buffer_ss.raw_index = BMC_CMD;
			spi_stat_ss.txdone_bit++; // number of completed packets
			data_in2 = 0;
		} else {
			if (serial_buffer_ss.raw_index == BMC_D0) {
				tmp_buf = (uint8_t) in_buf1 | 0b00000001;
			} else {
				tmp_buf = (uint8_t) in_buf2;
			}
			SPI2TXB = tmp_buf;
			data_in2 = 0;
		}
	}

	// ADC_GET_BYTES
	if (serial_buffer_ss.adc_value) {
		if (serial_buffer_ss.raw_index == ADC_GET_BYTES) {
			SPI2TXB = spi_stat_ss.daq_conf; // send PCB configuration code
			serial_buffer_ss.adc_value = false;
			serial_buffer_ss.raw_index = BMC_CMD;
			spi_stat_ss.txdone_bit++; // number of completed packets
			data_in2 = 0;
		} else {
			if (serial_buffer_ss.raw_index == BMC_D0) {
				SPI2TXB = (adc_buffer[channel] &0x00ff);
			} else {
				SPI2TXB = ((adc_buffer[channel] >> 8)&0x00ff);
			}
			data_in2 = 0;
		}
	}

	// GET_CFG_BYTES
	if (serial_buffer_ss.cfg_value) {
		if (serial_buffer_ss.raw_index == CFG_GET_BYTES) {
			SPI2TXB = CHECKBYTE;
			serial_buffer_ss.cfg_value = false;
			serial_buffer_ss.raw_index = BMC_CMD;
			spi_stat_ss.txdone_bit++; // number of completed packets
			data_in2 = 0;
		} else {
			switch (channel) {
			case 0x04:
				switch (serial_buffer_ss.raw_index) {
				case BMC_D0:
					SPI2TXB = (char) tmp_buf4[0];
					break;
				case BMC_D1:
					SPI2TXB = (char) tmp_buf4[1];
					break;
				case BMC_D2:
					SPI2TXB = (char) tmp_buf4[2];
					break;
				case BMC_D3:
					SPI2TXB = (char) tmp_buf4[3];
					break;
				default:
					SPI2TXB = CHECKBYTE;
					break;
				}
				break;
			case 0x05:
				switch (serial_buffer_ss.raw_index) {
				case BMC_D0:
					SPI2TXB = (char) tmp_buf5[0];
					break;
				case BMC_D1:
					SPI2TXB = (char) tmp_buf5[1];
					break;
				case BMC_D2:
					SPI2TXB = (char) tmp_buf5[2];
					break;
				case BMC_D3:
					SPI2TXB = (char) tmp_buf5[3];
					break;
				default:
					SPI2TXB = CHECKBYTE;
					break;
				}
				break;
			case 0x0:
			default:
				switch (serial_buffer_ss.raw_index) {
				case BMC_D0:
					SPI2TXB = CHECKBYTE;
					break;
				default:
					SPI2TXB = spi_stat_ss.daq_conf; // respond with DAQ configuration bits
					break;
				}
				break;
			}
			data_in2 = 0;
		}
	}

	if (++serial_buffer_ss.raw_index > SPI_BUFFER_LEN - 1) {
		serial_buffer_ss.raw_index = BMC_CMD;
		spi_stat_ss.txuf_bit++; // buffer high watermark cleared
	}

	command = data_in2 & HI_NIBBLE;

	if (serial_buffer_ss.dac_value || serial_buffer_ss.get_value || serial_buffer_ss.make_value || serial_buffer_ss.dac_value || serial_buffer_ss.cfg_value || serial_buffer_ss.cget_value || serial_buffer_ss.cmake_value) {
		goto isr_end;
	}

	if (command == CMD_DAC_GO) { // Found a GO for a DAC conversion command
		spi_comm_ss.ADC_RUN = false;
		spi_comm_ss.PORT_DATA = false;
		spi_comm_ss.CHAR_DATA = false;
		serial_buffer_ss.raw_index = BMC_D0;
		serial_buffer_ss.dac_value = true;
		spi_comm_ss.REMOTE_LINK = true;
		TMR0_Reload();
	}

	if (command == CMD_ADC_GO) { // Found a GO for a conversion command
		spi_comm_ss.ADC_RUN = false;
		spi_comm_ss.PORT_DATA = false;
		spi_comm_ss.CHAR_DATA = false;
		channel = data_in2 & LO_NIBBLE; // only 16 possible channels so higher numbers needs to be munged
		if (channel > AI_CHAN_FIX) {
			switch (channel) {
			case 0x6:
				channel = channel_ANC6;
				break;
			case 0x7:
				channel = channel_ANC7;
				break;
			case 0x8:
				channel = channel_AND5;
				break;
			case 0x9:
				channel = channel_VSS;
				break;
			case 0xa:
				channel = channel_Temp;
				break;
			case 0xb:
				channel = channel_DAC1;
				break;
			case 0xc:
				channel = channel_FVR_Buffer1;
				break;
			case 0xd:
				channel = channel_FVR_Buffer2;
				break;
			case 0xf:
				SPI1CON0bits.EN = 0;
				MLED_SetHigh();
				serial_buffer_ss.raw_index = BMC_CMD;
				clear_slaveo_flags();
				MLED_SetLow();
				SPI1CON0bits.EN = 1;
				channel = channel_ANA0;
				break;
			default:
				channel = channel_ANA0;
				break;
			}
		}
		serial_buffer_ss.raw_index = BMC_D0;
		serial_buffer_ss.adc_value = true;
		spi_comm_ss.REMOTE_LINK = true;
		TMR0_Reload();
	}

	if (!V.di_fail && command == CMD_PORT_GET) { // send the V.bmc_di buffer
		spi_comm_ss.ADC_RUN = false;
		spi_comm_ss.PORT_DATA = true;
		spi_comm_ss.CHAR_DATA = false;
		serial_buffer_ss.raw_index = BMC_D0;
		serial_buffer_ss.get_value = true;
		spi_comm_ss.REMOTE_LINK = true;
		TMR0_Reload();
	}

	if (!V.do_fail && command == CMD_PORT_GO) { // Found a GO for a DO command
		spi_comm_ss.ADC_RUN = false;
		spi_comm_ss.PORT_DATA = true;
		spi_comm_ss.CHAR_DATA = false;
		serial_buffer_ss.raw_index = BMC_D0;
		serial_buffer_ss.make_value = true;
		spi_comm_ss.REMOTE_LINK = true;
		TMR0_Reload();
	}

	if (!V.do_fail && command == CMD_CHAR_GET) { // send the serial buffer
		spi_comm_ss.ADC_RUN = false;
		spi_comm_ss.PORT_DATA = false;
		spi_comm_ss.CHAR_DATA = true;
		channel = data_in2 & LO_NIBBLE; // only 16 possible channels
		serial_buffer_ss.raw_index = BMC_D0;
		serial_buffer_ss.cget_value = true;
		spi_comm_ss.REMOTE_LINK = true;
		TMR0_Reload();
	}

	if (!V.do_fail && command == CMD_CHAR_GO) { // get data for the serial buffer
		spi_comm_ss.ADC_RUN = false;
		spi_comm_ss.PORT_DATA = false;
		spi_comm_ss.CHAR_DATA = true;
		channel = data_in2 & LO_NIBBLE; // only 16 possible channels
		serial_buffer_ss.raw_index = BMC_D0;
		serial_buffer_ss.cmake_value = true;
		spi_comm_ss.REMOTE_LINK = true;
		TMR0_Reload();
	}

	if (command == CMD_DUMMY_CFG) {
		serial_buffer_ss.raw_index = BMC_D0;
		serial_buffer_ss.cfg_value = true;
		channel = data_in2 & LO_NIBBLE; // only 16 possible channels so higher numbers needs to be munged
		tmp_buf4 = (void *) &ha_daq_calib.scaler4;
		tmp_buf5 = (void *) &ha_daq_calib.scaler5;
		if (channel == 0x7) {
			if (!serial_buffer_ss.adc_value && !serial_buffer_ss.dac_value && !serial_buffer_ss.make_value && !serial_buffer_ss.get_value&& !serial_buffer_ss.cmake_value && !serial_buffer_ss.cget_value) {
				MLED_SetHigh();
				clear_slaveo_flags();
				MCZ_PWM_SetLow();
				MLED_SetLow();
			}
		}
		spi_comm_ss.REMOTE_LINK = true;
		TMR0_Reload();
		StartTimer(TMR_ADC, ADCDELAY);
	}

isr_end:
	DLED_SetLow();
}

void slaveo_spi_isr(void)
{
	spi_stat_ss.spi_error_count++;
	SPI2INTF = 0;
}

void slaveo_time_isr(void)
{
	SPI2CON0bits.EN = 0; // reset OPi SPI link module
	SPI2CON0bits.EN = 1;
	if (SPI2STATUSbits.TXWE || SPI2STATUSbits.RXRE) { // check for overruns/collisions
	}
	DLED_SetLow();
}
