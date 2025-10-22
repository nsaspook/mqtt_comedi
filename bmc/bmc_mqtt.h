
#ifndef BMC_MQTT_H
#define BMC_MQTT_H

#ifdef __cplusplus
extern "C" {
#endif
#define _DEFAULT_SOURCE

#include "bmc.h"
#include "daq.h"

#define NI_MAXHOST      1025
#define NI_MAXSERV      32 

#include <sys/socket.h>
#include <netdb.h>

	
#define BENERGY	480.0f
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
#define CSV_COUNT 22
	
#define MAINS_HZ 60.0f
#define MAINS_HZ_LOW 45.0f
#define MAINS_HZ_HIGH 65.0f

#define CALIB_HV_LOW 50.0f
#define CALIB_HV_HIGH 75.0f

#define BSENSOR_MAX_NEG  -125.0f 
#define BSENSOR_MAX_POS  125.0f 

#define UPDATE_PACER  250 // MQTT and logging frequency to 0.01 seconds.

#define VALIDATE_LEN 55

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
		uint64_t bmc_id[4];
		double offset4[4];
		double scaler4[4];
		double offset5[4];
		double scaler5[4];
		double A200_Z[4];
		double A200_S[4];
	};

	struct ha_daq_hosts_type {
		const char hosts[4][NI_MAXHOST];
		const char mqtt[4][NI_MAXHOST];
		const char clients[4][NI_MAXHOST];
		const char topics[4][NI_MAXHOST];
		const char listen[4][NI_MAXHOST];
		char hname[4][NI_MAXHOST];
		double scaler[4], scaler4[4], scaler5[4];
		uint8_t hindex, bindex;
		uint32_t pacer[4];
		struct ha_daq_calib_type calib;
	};

	extern struct ha_flag_type ha_flag_vars_ss, ha_daq_hosts_type;
	extern struct ha_daq_hosts_type ha_daq_host;
	char * validate_bmc_text(const char *, bool *);

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

#ifdef __cplusplus
}
#endif

#endif /* BMC_MQTT_H */

