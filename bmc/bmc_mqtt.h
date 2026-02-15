
#ifndef BMC_MQTT_H
#define BMC_MQTT_H

#ifdef __cplusplus
extern "C" {
#endif
#define _DEFAULT_SOURCE

#include "bmc.h"
#include "daq.h"
#include <sys/socket.h>
#include <netdb.h>
#include <libconfig.h>

	/*
	 * configuration data for Home Assistant
	 */
#define BMC_MAXHOST      1024 // hosts buffer size	

#define DBENERGY  3100.0f
#define DBVOLTAGE 12.6f
#define DPVENERGY 300.0f
#define DPVVOLTAGE 12.6f
#define DSOC_MODE 2.0f

	struct bmc_settings {
		double BENERGYV, BVOLTAGEV, PVENERGYV, PVVOLTAGEV, SOC_MODEV;
		char MQTT_HOSTIP[BMC_MAXHOST];
		char MY_IP[BMC_MAXHOST];
	};

#define HOST_SLOTS 10 //BMC host data slots
#define OPEN_HOST 9 // BMC host array index for those without a known IP address or MUI
#define BENERGY_INTEGRAL 1440.0f
#define MQTT_RETRY 10

#define HA_SW_DELAY     400000  // usecs
#define TOKEN_DELAY     600
#define GTI_TOKEN_DELAY 300

#define MAIN_DELAY      1250 // 1.0 msec comedi sample rate max

#define QOS             2

#define RDEV_SIZE 10

#define SLEEP_CODE      0
#define FLOAT_CODE      1

#define MBMQTT  1024
#define CHECKMARK 1957
#define CHECKBYTE 0x57
	/*
	 * number of data items from daq_bmc board for serial data streams
	 */
#define CSV_COUNT 22

#define MAINS_HZ 60.0f
#define MAINS_HZ_LOW 45.0f
#define MAINS_HZ_HIGH 65.0f

#define CALIB_HV_LOW 50.0f
#define CALIB_HV_HIGH 75.0f

#define BSENSOR_MAX_NEG  -125.0f
#define BSENSOR_MAX_POS  125.0f

#define UPDATE_PACER  250 // MQTT and logging frequency to 0.01 seconds.
#define UPDATE_PACER_RPI2B 400
#define BAT_RUN_MAX 140.0f  // max displayed run time at current load
#define DRAIN_HOUR 1.0f
#define IDLE_DRAIN      1.0f // system battery losses in W

#define VALIDATE_LEN 55

#define BVSOC_SLOTS     12      // 24V LiFePO4 Battery to SOC data table slots

	/*
	 * maps the EM540 modbus registers to int32_t and uint16_t values
	 */
	typedef struct EM_data1 {
		volatile int32_t
		vl1n, vl2n, vl3n,
		vl1l2, vl2l3, vl3l1,
		al1, al2, al3,
		wl1, wl2, wl3,
		val1, val2, val3,
		varl1, varl2, varl3, // extra stuff
		vlnsys, vllsys, wsys, vasys, varsys;
		volatile int16_t
		pfl1, pfl2, pfl3, pfsys,
		phaseseq, hz;
	} EM_data1;

	typedef struct EM_data2 {
		volatile int64_t
		kwhpt, kvarhpt, kwhpp, kvarhpp,
		kwhpl1, kwhpl2, kwhpl3,
		kwhnt, kvarhnt, kwhnp, kvarhnp,
		kvaht, kvahp;
		volatile int32_t
		rhm, rhmk, rhmp, rhmkp,
		hz, rhlc;
	} EM_data2;

	enum mqtt_id {
		P8055_ID,
		FM80_ID,
		DUMPLOAD_ID,
		HA_ID,
		COMEDI_ID,
		LAST_MQTT_ID,
	};

	struct ha_flag_type {
		volatile MQTTClient_deliveryToken deliveredtoken, receivedtoken;
		volatile bool runner, rec_ok;
		int32_t ha_id;
		volatile int32_t var_update, energy_mode;
	};

	struct ha_daq_calib_type {
		uint16_t checkmark;
		bool newfile;
		bool oldfile, fileok;
		uint64_t bmc_id[HOST_SLOTS];
		double offset4[HOST_SLOTS];
		double scalar4[HOST_SLOTS];
		double offset5[HOST_SLOTS];
		double scalar5[HOST_SLOTS];
		double A200_Z[HOST_SLOTS];
		double A200_S[HOST_SLOTS];
	};

	struct ha_daq_hosts_type {
		const char hosts[HOST_SLOTS][BMC_MAXHOST];
		char mqtt[HOST_SLOTS][BMC_MAXHOST];
		char clients[HOST_SLOTS][BMC_MAXHOST];
		char topics[HOST_SLOTS][BMC_MAXHOST];
		char listen[HOST_SLOTS][BMC_MAXHOST];
		char hname[HOST_SLOTS][BMC_MAXHOST];
		double scalar[HOST_SLOTS], scalar4[HOST_SLOTS], scalar5[HOST_SLOTS];
		uint8_t hindex, bindex;  // host array, calibration array
		uint32_t pacer[HOST_SLOTS];
		struct ha_daq_calib_type calib;
	};

	extern struct ha_flag_type ha_flag_vars_ss, ha_daq_hosts_type;
	extern struct ha_daq_hosts_type ha_daq_host;
	char * validate_bmc_text(const char *, bool *);
	extern struct bmc_settings S;

	void mqtt_bmc_data(MQTTClient, const char *);
	void delivered(void *, MQTTClient_deliveryToken);
	int32_t msgarrvd(void *, char *, int, MQTTClient_message *);
	void connlost(void *, char *);
	void showIP(void);
	void skeleton_daemon(void);
	void bmc_mqtt_init(void);
	char * log_time(bool);
	bool use_cal_file(FILE *, bool);

	void timer_callback(int32_t);
	void comedi_push_mqtt(void);
	bool get_bmc_serial(void);
	bool validate_ok(void);
	void set_bmc_timer(void);

#ifdef __cplusplus
}
#endif

#endif /* BMC_MQTT_H */

