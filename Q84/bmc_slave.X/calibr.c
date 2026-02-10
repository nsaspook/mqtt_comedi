#include "calibr.h"

static struct ha_daq_calib_type r_cal;

static const float HV_SCALE4_0 = 64.2695f;
static const float HV_SCALE5_0 = 64.2695f;
static const float HV_SCALE4_1 = 64.1890f;
static const float HV_SCALE5_1 = 64.1415f;
static const float HV_SCALE4_2 = 54.1890f;
static const float HV_SCALE5_2 = 54.1415f;
static const float HV_SCALE4_3 = 55.6000f;
static const float HV_SCALE5_3 = 55.6500f;
static const float HV_SCALE4_4 = 64.3590f;
static const float HV_SCALE5_4 = 64.3850f;
static const float HV_SCALE4_5 = 64.3520f;
static const float HV_SCALE5_5 = 64.3480f;
static const float HV_SCALE4_6 = 64.2500f;
static const float HV_SCALE5_6 = 64.2500f;

/*
 * use Q84 MUI to find calibration data for each board
 */
void set_calibration(unsigned long long mui)
{
	/*
	 * calibration scalar selection using MUI from controller
	 */
	switch (mui) {
	case 0x589B6: //
		ha_daq_calib.scaler4 = HV_SCALE4_0;
		ha_daq_calib.scaler5 = HV_SCALE5_0;
		break;
	case 0: // USBBoard
		ha_daq_calib.scaler4 = HV_SCALE4_1;
		ha_daq_calib.scaler5 = HV_SCALE5_1;
		break;
	case 0x598F3: //
		ha_daq_calib.scaler4 = HV_SCALE4_2;
		ha_daq_calib.scaler5 = HV_SCALE5_2;
		break;
	case 0x5ABB6: //
		ha_daq_calib.scaler4 = HV_SCALE4_3;
		ha_daq_calib.scaler5 = HV_SCALE5_3;
		break;
	case 0x4DE66:
		ha_daq_calib.scaler4 = HV_SCALE4_4;
		ha_daq_calib.scaler5 = HV_SCALE5_4;
		break;
	case 0x61DB5:
		ha_daq_calib.scaler4 = HV_SCALE4_5;
		ha_daq_calib.scaler5 = HV_SCALE5_5;
		break;
	case 0x5B728:
		ha_daq_calib.scaler4 = HV_SCALE4_6;
		ha_daq_calib.scaler5 = HV_SCALE5_6;
		break;
	default:
		ha_daq_calib.scaler4 = HV_SCALAR4;
		ha_daq_calib.scaler5 = HV_SCALAR5;
		break;
	}
}

/*
 * return scaled voltages
 */
float phy_chan4(uint16_t value)
{
	if (value) {
		return((((float) value)) / ADC_SCALE) * ha_daq_calib.scaler4;
	} else {
		return 0.00001f;
	}
}

float phy_chan5(uint16_t value)
{
	if (value) {
		return((((float) value)) / ADC_SCALE) * ha_daq_calib.scaler5;
	} else {
		return 0.00001f;
	}
}

/*
 * read EEPROM into temp program variable buffer
 */
bool read_cal_data(void)
{
	uint16_t x = 0, y;
	uint8_t *r_cal_ptr, crcVal_save;

	y = sizeof(r_cal);
	r_cal_ptr = (uint8_t*) & r_cal; // need to access the raw bytes of the calibration structure
	// Initialize the CRC module
	CRC_Initialize();

	// Start CRC
	CRC_StartCrc();

	do {
		r_cal_ptr[x] = DATAEE_ReadByte(x);
	} while (++x < y);
	crcVal_save = r_cal.crc;

	if (r_cal.checkmark == EE_CHECKMARK) {
		r_cal.crc = TATE; // standard CRC filler code
		x = 0;

		do {
			CRC_WriteData(r_cal_ptr[x]);
			while (CRC_IsCrcBusy());
		} while (++x < y);
		if (CRC_GetCalculatedResult(NORMAL, 0x00) != crcVal_save)
			return false;
	} else {
		return false;
	}
	r_cal.crc = crcVal_save; // reload actual EEPROM CRC
	return true;
}

/*
 * write program variables to eeprom
 */
void write_cal_data(void)
{
	uint16_t x = 0, y;
	uint8_t *r_cal_ptr, crcVal;

	y = sizeof(ha_daq_calib);
	ha_daq_calib.crc = TATE;
	r_cal_ptr = (uint8_t*) & ha_daq_calib;
	ha_daq_calib.checkmark = EE_CHECKMARK;
	// Initialize the CRC module
	CRC_Initialize();

	// Start CRC
	CRC_StartCrc();

	do {
		DATAEE_WriteByte(x, r_cal_ptr[x]);
		CRC_WriteData(r_cal_ptr[x]);
		while (CRC_IsCrcBusy());
	} while (++x < y);
	// Read CRC check value
	crcVal = (uint8_t) CRC_GetCalculatedResult(NORMAL, 0x00);
	// store crc in EEPROM
	DATAEE_WriteByte(sizeof(ha_daq_calib) - 1, crcVal);
}

/*
 * copy program variable buffer into active variable
 */
void update_cal_data(void)
{
	ha_daq_calib = r_cal;
	if (!ha_daq_calib.c_zero_cal) {
	}

	if (!ha_daq_calib.c_scale_cal) {
	}
}