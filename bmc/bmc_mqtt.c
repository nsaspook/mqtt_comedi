#include "bmc_mqtt.h"
#include <math.h>

#define COEF            12.0f

static const char *const FW_Date = __DATE__;
static const char *const FW_Time = __TIME__;

struct itimerval new_timer = {
	.it_value.tv_sec = CMD_SEC,
	.it_value.tv_usec = CMD_USEC,
	.it_interval.tv_sec = CMD_SEC,
	.it_interval.tv_usec = CMD_USEC,
};
struct itimerval old_timer;
time_t rawtime;
MQTTClient_connectOptions conn_opts_p = MQTTClient_connectOptions_initializer,
	conn_opts_sd = MQTTClient_connectOptions_initializer,
	conn_opts_ha = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;
char hname[256], *hname_ptr = hname;
size_t hname_len = 12;
int32_t validate_failure;

struct ha_csv_type {
	double acvolts, acamps, acwatts, acwatts_gti, acva, acvar, acpf, achz, acwin, acwout, bvolts, pvolts, bamps, pamps, panel_watts, fm_online, fm_mode, em540_online, bsensor0, dcwin, dcwout, bmc_id;
};

char tmp_test_ptr[512];

struct ha_flag_type ha_flag_vars_ss = {
	.runner = false,
	.receivedtoken = false,
	.deliveredtoken = false,
	.rec_ok = false,
	.ha_id = COMEDI_ID,
	.var_update = 0,
};

struct ha_daq_hosts_type ha_daq_host = {
	.hosts[0] = "10.1.1.30",
	.hosts[1] = "10.1.1.39",
	.hosts[2] = "10.1.1.46",
	.hosts[3] = "10.1.1.45",
	.topics[0] = "comedi/bmc/data/bmc/1",
	.topics[1] = "comedi/bmc/data/bmc/2",
	.topics[2] = "comedi/bmc/data/bmc/3",
	.topics[3] = "comedi/bmc/data/bmc/4",
	.listen[0] = "comedi/bmc/listen/bmc/1",
	.listen[1] = "comedi/bmc/listen/bmc/2",
	.listen[2] = "comedi/bmc/listen/bmc/3",
	.listen[3] = "comedi/bmc/listen/bmc/4",
	.hname[0] = "RDAQ1",
	.hname[1] = "RDAQ2",
	.hname[2] = "RDAQ3",
	.hname[3] = "RDAQ4",
	.clients[0] = "Energy_Mqtt_BMC1",
	.clients[1] = "Energy_Mqtt_BMC2",
	.clients[2] = "Energy_Mqtt_BMC3",
	.clients[3] = "Energy_Mqtt_BMC4",
	.scaler[0] = HV_SCALE0,
	.scaler[1] = HV_SCALE1,
	.scaler[2] = HV_SCALE2,
	.scaler[3] = HV_SCALE3,
	.scaler4[0] = HV_SCALE4_0,
	.scaler4[1] = HV_SCALE4_1,
	.scaler4[2] = HV_SCALE4_2,
	.scaler4[3] = HV_SCALE4_3,
	.scaler5[0] = HV_SCALE5_0,
	.scaler5[1] = HV_SCALE5_1,
	.scaler5[2] = HV_SCALE5_2,
	.scaler5[3] = HV_SCALE5_3,
	.pacer[0] = UPDATE_PACER, // opiha
	.pacer[1] = UPDATE_PACER, // daq1
	.pacer[2] = UPDATE_PACER, // daq2
	.pacer[3] = UPDATE_PACER,
	.hindex = 0,
	.bindex = 0,
	.calib.checkmark = CHECKMARK,
	.calib.bmc_id[0] = 362934, // bmc Q84 MUI testing board
	.calib.offset4[0] = HV_SCALE_OFFSET,
	.calib.scaler4[0] = HV_SCALE4_0,
	.calib.offset5[0] = HV_SCALE_OFFSET,
	.calib.scaler5[0] = HV_SCALE5_0,
	.calib.A200_Z[0] = A200_0_ZERO,
	.calib.A200_S[0] = A200_0_SCALAR,
	.calib.bmc_id[1] = 1, // bmc Q84 MUI
	.calib.offset4[1] = HV_SCALE_OFFSET,
	.calib.scaler4[1] = HV_SCALE4_1,
	.calib.offset5[1] = HV_SCALE_OFFSET,
	.calib.scaler5[1] = HV_SCALE5_1,
	.calib.A200_Z[1] = A200_0_ZERO,
	.calib.A200_S[1] = A200_0_SCALAR,
	.calib.bmc_id[2] = 2, // bmc Q84 MUI
	.calib.offset4[2] = HV_SCALE_OFFSET,
	.calib.scaler4[2] = HV_SCALE4_2,
	.calib.offset5[2] = HV_SCALE_OFFSET,
	.calib.scaler5[2] = HV_SCALE5_2,
	.calib.A200_Z[2] = A200_0_ZERO,
	.calib.A200_S[2] = A200_0_SCALAR,
	.calib.bmc_id[3] = 371638, // bmc Q84 MUI enclosure board
	.calib.offset4[3] = HV_SCALE_OFFSET,
	.calib.scaler4[3] = HV_SCALE4_3,
	.calib.offset5[3] = HV_SCALE_OFFSET,
	.calib.scaler5[3] = HV_SCALE5_3,
	.calib.A200_Z[3] = A200_0_ZERO,
	.calib.A200_S[3] = A200_0_SCALAR,
};

double ac0_filter(const double);
double ac1_filter(const double);

/** \file bmc_mqtt.c
 * show all assigned networking addresses and types
 * on the current machine
 */
void showIP(void)
{
	struct ifaddrs *ifaddr, *ifa;
	int s;
	char host[NI_MAXHOST];

	if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
	}

	for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL)
			continue;

		s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

		if (ifa->ifa_addr->sa_family == AF_INET) {
			if (s != 0) {
				exit(EXIT_FAILURE);
			}
			printf("\tInterface : <%s>\n", ifa->ifa_name);
			printf("\t  Address : <%s>\n", host);

			if (strcmp(host, &ha_daq_host.hosts[0][0]) == 0) {
				ha_daq_host.hindex = 0;
			}
			if (strcmp(host, &ha_daq_host.hosts[1][0]) == 0) {
				ha_daq_host.hindex = 1;
			}
			if (strcmp(host, &ha_daq_host.hosts[2][0]) == 0) {
				ha_daq_host.hindex = 2;
			}
			if (strcmp(host, &ha_daq_host.hosts[3][0]) == 0) {
				ha_daq_host.hindex = 3;
			}
		}
	}
	freeifaddrs(ifaddr);
}

/*
 * setup ha_energy program to run as a background deamon
 * disconnect and exit foreground startup process
 */
void skeleton_daemon(void)
{
	pid_t pid;

	/* Fork off the parent process */
	pid = fork();

	/* An error occurred */
	if (pid < 0) {
		printf("\r\n%s DAEMON failure  LOG Version %s : MQTT Version %s\r\n", log_time(false), LOG_VERSION, MQTT_VERSION);
		exit(EXIT_FAILURE);
	}

	/* Success: Let the parent terminate */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* On success: The child process becomes session leader */
	if (setsid() < 0) {
		exit(EXIT_FAILURE);
	}

	/* Catch, ignore and handle signals */
	/*TODO: Implement a working signal handler */
	//    signal(SIGCHLD, SIG_IGN);
	//    signal(SIGHUP, SIG_IGN);

	/* Fork off for the second time*/
	pid = fork();

	/* An error occurred */
	if (pid < 0) {
		exit(EXIT_FAILURE);
	}

	/* Success: Let the parent terminate */
	if (pid > 0) {
		exit(EXIT_SUCCESS);
	}

	/* Set new file permissions */
	umask(0);

	/* Change the working directory to the root directory */
	/* or another appropriated directory */
	chdir("/");

	/* Close all open file descriptors */
	int x;
	for (x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
		close(x);
	}
}

/*
 * sent the current UTC to the Dump Load controller
 */
char * log_time(bool log)
{
	static char time_log[RBUF_SIZ] = {0};
	time_t rawtime_log;
	int32_t len = 0;

	tzset();
	timezone = 0;
	daylight = 0;
	time(&rawtime_log);
	sprintf(time_log, "%s", ctime(&rawtime_log));
	len = strlen(time_log);
	time_log[len - 1] = 0; // munge out the return character
	if (log) {
		fprintf(fout, "%s ", time_log);
		fflush(fout);
	}
	return time_log;
}

/*
 * data update timer flag
 * and CMD_SEC seconds software time clock
 */
void timer_callback(int32_t signum)
{
	signal(signum, timer_callback);
	ha_flag_vars_ss.runner = true;
	E.thirty_sec_clock++;
	E.log_time_reset++;
}

/*
 * MQTT Broker connection errors can be fatal
 */
void connlost(void *context, char *cause)
{
	struct ha_flag_type *ha_flag = context;
	int32_t id_num = ha_flag->ha_id;
	static uint32_t times = 0;
	char * where = "Missing Topic";
	char * what = "Reconnection Error";

	// bug-out if no context variables passed to callback
	if (context == NULL) {
		id_num = -1;
		goto bugout; /// trouble in River-city
	}

	if (times++ > MQTT_RECONN) {
		goto bugout;
	} else {
		if (times > 1) {
			fprintf(fout, "%s Connection lost, retrying %d \n", log_time(false), times);
			fprintf(fout, "%s     cause: %s, h_id %d, c_id %d, %s \n", log_time(false), cause, id_num, 0, what);
			fprintf(fout, "%s MQTT DAEMON reconnect failure,  LOG Version %s : MQTT Version %s\n", log_time(false), LOG_VERSION, MQTT_VERSION);
		}
		fflush(fout);
		times = 0;
		return;
	}

bugout:
	fprintf(fout, "%s Connection lost, exit ha_energy program\n", log_time(false));
	fprintf(fout, "%s     cause: %s, h_id %d, c_id %d, %s \n", log_time(false), cause, id_num, 0, where);
	fprintf(fout, "%s MQTT DAEMON context is NULL failure,  LOG Version %s : MQTT Version %s\n", log_time(false), LOG_VERSION, MQTT_VERSION);
	fflush(fout);
	exit(EXIT_FAILURE);
}

/*
 * set the broker has message token
 */
void delivered(void *context, MQTTClient_deliveryToken dt)
{
	struct ha_flag_type *ha_flag = context;

	// bug-out if no context variables passed to callback
	if (context == NULL) {
		return;
	}
	ha_flag->deliveredtoken = dt;
}

bool use_cal_file(FILE * file, bool write)
{
	bool ret = false;

	return ret;
}

/** \file bmc_mqtt.c
 *
 */
void bmc_mqtt_init(void)
{
	E.mqtt_count = 0;
	gethostname(hname, hname_len);
	hname[12] = 0;
	printf("\r\n  LOG Version %s : MQTT Version %s : Host Name %s\r\n", LOG_VERSION, MQTT_VERSION, hname);
	showIP();
	skeleton_daemon();
#ifdef LOG_TO_FILE
	fout = fopen(LOG_TO_FILE, "a");
	if (fout == NULL) {
		fout = fopen(LOG_TO_FILE_ALT, "a");
		if (fout == NULL) {
			fout = fopen(LOG_TO_FILE_ALT, "a+"); // create a new file
		}
	}
#else
	fout = stdout;
#endif

#ifdef CAL_FILE
	calfile = fopen(CAL_FILE, "r");
	if (calfile == NULL) {
		calfile = fopen(CAL_FILE_ALT, "r");
		if (calfile == NULL) {
			calfile = fopen(CAL_FILE, "a+"); // create a new file
			if (calfile != NULL) {
				ha_daq_host.calib.newfile = true;
			}
		} else {
			ha_daq_host.calib.oldfile = true;
		}
	} else {
		ha_daq_host.calib.oldfile = true;
	}
#endif
	fprintf(fout, "\r\n%s LOG Version %s : MQTT Version %s : Host Name %s", log_time(false), LOG_VERSION, MQTT_VERSION, hname);
	fflush(fout);

	/*
	 * set the timer for MQTT publishing sample speed
	 * CMD_SEC sets the time in seconds
	 */
	setitimer(ITIMER_REAL, &new_timer, &old_timer);
	signal(SIGALRM, timer_callback);

	if (strncmp(hname, TNAME, 6) == 0) {
		MQTTClient_create(&E.client_p, LADDRESS, (const char *) &ha_daq_host.topics[ha_daq_host.hindex],
			MQTTCLIENT_PERSISTENCE_NONE, NULL);
		conn_opts_p.keepAliveInterval = KAI;
		conn_opts_p.cleansession = 1;
		hname_ptr = LADDRESS;
	} else {
		MQTTClient_create(&E.client_p, ha_daq_host.hosts[ha_daq_host.hindex], (const char *) &ha_daq_host.clients[ha_daq_host.hindex],
			MQTTCLIENT_PERSISTENCE_NONE, NULL);
		conn_opts_p.keepAliveInterval = KAI;
		conn_opts_p.cleansession = 1;
		hname_ptr = (char *) ha_daq_host.hosts[ha_daq_host.hindex];
	}

	fprintf(fout, "\r\n%s Connect to MQTT server %s with client %s\n", log_time(false), hname_ptr, (const char *) &ha_daq_host.clients[ha_daq_host.hindex]);
	fflush(fout);
	MQTTClient_setCallbacks(E.client_p, &ha_flag_vars_ss, connlost, msgarrvd, delivered);
	if ((E.rc = MQTTClient_connect(E.client_p, &conn_opts_p)) != MQTTCLIENT_SUCCESS) {
		fprintf(fout, "%s Failed to connect MQTT server, return code %d %s, %s\n", log_time(false), E.rc, hname_ptr, (const char *) &ha_daq_host.clients[ha_daq_host.hindex]);
		fflush(fout);
		pthread_mutex_destroy(&E.ha_lock);
		exit(EXIT_FAILURE);
	}

	MQTTClient_subscribe(E.client_p, ha_daq_host.topics[ha_daq_host.hindex], QOS); // remote DAQ data for the HA_Energy system

	pubmsg.payload = "online";
	pubmsg.payloadlen = strlen("online");
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	ha_flag_vars_ss.deliveredtoken = 0;
}

int32_t msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	int32_t i, ret = 1;
	const char* payloadptr;
	char buffer[MBMQTT];
	struct ha_flag_type *ha_flag = context;

	E.mqtt_count++;
	// bug-out if no context variables passed to callback
	if (context == NULL) {
		ret = -1;
		goto null_exit;
	}

#ifdef DEBUG_REC
	fprintf(fout, "Message arrived\n");
#endif
	/*
	 * move the received message into a processing holding buffer
	 */
	payloadptr = message->payload;
	for (i = 0; i < message->payloadlen; i++) {
		buffer[i] = *payloadptr++;
	}
	buffer[i] = 0; // make a null terminated C string

	// parse the JSON data in the holding buffer
	cJSON *json = cJSON_ParseWithLength(buffer, message->payloadlen);
	if (json == NULL) {
		const char *error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			fprintf(fout, "%s Error: %s NULL cJSON pointer\n", log_time(false), error_ptr);
		}
		ret = -1;
		ha_flag->rec_ok = false;
		E.comedi = false;
		goto error_exit;
	}

	/*
	 * MQTT messages for COMEDI
	 */
#ifdef DEBUG_REC
	fprintf(fout, "COMEDI MQTT data\r\n");
#endif
	cJSON *data_result = json;

	data_result = cJSON_GetObjectItemCaseSensitive(json, "Comedi_Request");

	if (cJSON_IsString(data_result) && (data_result->valuestring != NULL)) {
		fprintf(fout, "%s Comedi Trigger from MQTT server, Topic %s %s\n", log_time(false), topicName, data_result->valuestring);
		fflush(fout);
		ret = true;
	}
	E.comedi = true;

	// done with processing MQTT async message, set state flags
	ha_flag->receivedtoken = true;
	ha_flag->rec_ok = true;
	ha_flag_vars_ss.runner = true; // send data in response to received message of any type
	/*
	 * exit and delete/free resources. In steps depending of possible error conditions
	 */
error_exit:
	// delete the JSON object
	cJSON_Delete(json);
null_exit:
	// free the MQTT objects
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);
	fflush(fout);
	return ret;
}

/*
 * send Comedi variables MQTT host
 */
void mqtt_bmc_data(MQTTClient client_p, const char * topic_p)
{
	cJSON *json;
	time_t rawtime;
	static uint32_t spam = 0;
	static uint32_t pacer = 1001;

	MQTTClient_message pubmsg = MQTTClient_message_initializer;
	MQTTClient_deliveryToken token;
	ha_flag_vars_ss.deliveredtoken = 0;
	static struct ha_csv_type R; // results from Q84 board

	//#define DIGITAL_ONLY

#ifndef DIGITAL_ONLY
	double over_sample;

	over_sample = 0.0f; // over-sample avg
	for (int i = 0; i < OVER_SAMP; i++) {
		if (bmc.BOARD == bmcboard) {
			over_sample += ac0_filter(get_adc_volts(channel_ANA4));
		} else {
			over_sample += ac0_filter(get_adc_volts(0));
		}
	}
	if (bmc.BOARD == bmcboard) {
		E.adc[channel_ANA4] = over_sample / (double) OVER_SAMP;
	} else {
		E.adc[channel_ANA0] = over_sample / (double) OVER_SAMP;
	}

	over_sample = 0.0f; // over-sample avg
	for (int i = 0; i < OVER_SAMP; i++) {
		if (bmc.BOARD == bmcboard) {
			over_sample += ac1_filter(get_adc_volts(channel_ANA5));
		} else {
			over_sample += ac1_filter(get_adc_volts(1));
		}
	}
	if (bmc.BOARD == bmcboard) {
		E.adc[channel_ANA5] = over_sample / (double) OVER_SAMP;
	} else {
		E.adc[channel_ANA1] = over_sample / (double) OVER_SAMP;
	}

	if (bmc.BOARD == bmcboard) {
		E.adc[channel_ANA0] = get_adc_volts(channel_ANA0);
		R.bsensor0 = lp_filter((E.adc[channel_ANA0] - ha_daq_host.calib.A200_Z[ha_daq_host.bindex]) * ha_daq_host.calib.A200_S[ha_daq_host.bindex], BSENSOR0, true);
		E.adc[channel_ANA1] = get_adc_volts(channel_ANA1);
		E.adc[channel_ANA2] = get_adc_volts(channel_ANA2);
		E.adc[channel_ANC6] = get_adc_volts(channel_ANC6);
		E.adc[channel_ANC7] = get_adc_volts(channel_ANC7);
		E.adc[channel_AND5] = get_adc_volts(channel_AND5);
	}
#endif

	E.do_16b = bmc.dataout.dio_buf;
	E.di_16b = datain;

	if (pacer++ > ha_daq_host.pacer[ha_daq_host.hindex]) {
		bool ok_data = false;
		static uint32_t goods = 0, bads = 0;
		char *tmp_ptr, *jtoken;

		pacer = 0;
		strncpy(tmp_test_ptr, daq_bmc_data_buf, 512);
		tmp_ptr = validate_bmc_text(daq_bmc_data_buf, &ok_data);
		strncpy(daq_bmc_data_buf, tmp_ptr, 511);
		if (ok_data) {
			goods++;
			jtoken = strtok(daq_bmc_data_buf, ",");
			if (jtoken != NULL) {
				/*
				 * parse the string for variable values
				 * double acvolts, acamps, acwatts, acva, acvar, acpf, achz, bvolts, pvolts, bamps, pamps, fm_online, fm_mode;
				 */
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.acvolts = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.acamps = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.acwatts = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.acwatts_gti = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.acva = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.acvar = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.acpf = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.achz = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.bvolts = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.pvolts = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.bamps = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.pamps = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.panel_watts = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.fm_online = atoi(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.fm_mode = atoi(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					R.em540_online = atoi(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL) {
					R.bmc_id = atoll(jtoken);
					if (R.bmc_id == ha_daq_host.calib.bmc_id[0]) {
						ha_daq_host.bindex = 0;
					}
					if (R.bmc_id == ha_daq_host.calib.bmc_id[1]) {
						ha_daq_host.bindex = 1;
					}
					if (R.bmc_id == ha_daq_host.calib.bmc_id[2]) {
						ha_daq_host.bindex = 2;
					}
					if (R.bmc_id == ha_daq_host.calib.bmc_id[3]) {
						ha_daq_host.bindex = 3;
					}
				}
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					ha_daq_host.calib.scaler4[ha_daq_host.bindex] = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					ha_daq_host.calib.scaler5[ha_daq_host.bindex] = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					ha_daq_host.calib.A200_Z[ha_daq_host.bindex] = atof(jtoken);
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL)
					ha_daq_host.calib.A200_S[ha_daq_host.bindex] = atof(jtoken);
			}
		}

		/*
		 * various data fix-ups and sanity checks
		 */
		R.acwin = R.acwatts_gti;
		if (R.acwatts > 0.0f) {
			R.acwout = calc_fixups(R.acwatts, NO_NEG); // utility power used
		} else {
			R.acwout = 0.0f;
		}

		if (R.bsensor0 < BSENSOR_MAX_NEG || R.bsensor0 > BSENSOR_MAX_POS) {
			R.bsensor0 = 0.0f;
		}

		if (R.bsensor0 * R.bvolts > 0.0f) {
			R.dcwin = calc_fixups(R.bsensor0 * R.bvolts, NO_NEG); // charge
			R.dcwout = 0.0f;
		} else {
			R.dcwout = fabs(R.bsensor0 * R.bvolts); // discharge
			R.dcwin = 0.0f;
		}

		if (R.achz < MAINS_HZ_LOW || R.achz > MAINS_HZ_HIGH) {
			R.achz = MAINS_HZ;
		}

		if (ha_daq_host.calib.scaler4[ha_daq_host.bindex] > CALIB_HV_HIGH || ha_daq_host.calib.scaler4[ha_daq_host.bindex] < CALIB_HV_LOW) {
			ha_daq_host.calib.scaler4[ha_daq_host.bindex] = HV_SCALE4_0;
		}
		if (ha_daq_host.calib.scaler5[ha_daq_host.bindex] > CALIB_HV_HIGH || ha_daq_host.calib.scaler5[ha_daq_host.bindex] < CALIB_HV_LOW) {
			ha_daq_host.calib.scaler5[ha_daq_host.bindex] = HV_SCALE5_0;
		}


		if (ok_data) {
			fprintf(fout, "%s Sending Comedi data to MQTT server, Topic %s DO 0x%.4x DI 0x%.6x, DAQ, OK Data %d, goods %d, bads %d, validate failure code %d\n", log_time(false), topic_p, bmc.dataout.dio_buf, datain, ok_data, goods, bads, validate_failure);
		} else {
			bads++;
			fprintf(fout, "%s Sending Comedi data to MQTT server, Topic %s DO 0x%.4x DI 0x%.6x, DAQ %s, OK Data %d, bads %d, validate failure code %d\n", log_time(false), topic_p, bmc.dataout.dio_buf, datain, tmp_test_ptr, ok_data, bads, validate_failure);
		}
		memset(daq_bmc_data_text, 0, MAX_STRLEN);
		if (bmc.BOARD == bmcboard) {
			fprintf(fout, "ANA0 %lfV, ANA1 %fV, ANA2 %f, ANA4 %fV, ANA5 %fV, AND5 %fV, Battery Sensor %6.3fA, : Host Index %d, Scaler Index %d, Scaler ANA4 %f, Scaler ANA5 %f Serial 0X%X\n",
				get_adc_volts(channel_ANA0), get_adc_volts(channel_ANA1), get_adc_volts(channel_ANA2),
				E.adc[channel_ANA4], E.adc[channel_ANA5], E.adc[channel_AND5], R.bsensor0, ha_daq_host.hindex, ha_daq_host.bindex, ha_daq_host.calib.scaler4[ha_daq_host.bindex], ha_daq_host.calib.scaler5[ha_daq_host.bindex],
				daq_bmc_data[0]);
		} else {
			fprintf(fout, "ANA0 %lfV, ANA1 %fV : Scaler Index %d, Scaler ANA4 %f, Scaler ANA5 %f\n",
				get_adc_volts(channel_ANA0), get_adc_volts(channel_ANA1),
				ha_daq_host.hindex, ha_daq_host.scaler4[ha_daq_host.hindex], ha_daq_host.scaler5[ha_daq_host.hindex]);
		}
		fflush(fout);
		E.mqtt_count++;
		E.sequence++;
		json = cJSON_CreateObject();
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "name", 64);
		cJSON_AddStringToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], (const char *) &ha_daq_host.clients[ha_daq_host.hindex]);
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "board", 64);
		cJSON_AddStringToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], (const char *) bmc.BNAME);
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "sequence", 64);
		cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], E.sequence);
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "mqtt_do_16b", 64);
		cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], (double) E.do_16b);
		if (bmc.BOARD == bmcboard) {
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "mqtt_di_24b", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], (double) datain);
		} else {
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "mqtt_di_16b", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], (double) E.di_16b);
		}
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_adc0", 64);
		if (bmc.BOARD == bmcboard) {
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], E.adc[channel_ANA4]);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_adc1", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], E.adc[channel_ANA5]);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_bsamps0", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.bsensor0);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_bswatts0", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.bsensor0 * R.bvolts);
		} else {
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], E.adc[channel_ANA0]);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_adc1", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], E.adc[channel_ANA1]);
		}
		/*
		 * parse the string for variable values
		 * double acvolts, acamps, acwatts, acva, acvar, acpf, achz, bvolts, pvolts, bamps, pamps, fm_online, fm_mode;
		 */
		if (bmc.BOARD == bmcboard) { // EM540 and FM80 data
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_acvolts", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.acvolts);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_acamps", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.acamps);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_acwatts", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.acwatts);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_acwatts_gti", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.acwatts_gti);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_acva", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.acva);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_acvar", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.acvar);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_acpf", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.acpf);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_achz", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.achz);

			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_acwin", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.acwin);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_acwout", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.acwout);

			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_dcwin", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.dcwin);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_dcwout", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.dcwout);

			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_bvolts", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.bvolts);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_pvolts", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.pvolts);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_bamps", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.bamps);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_pamps", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.pamps);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_panel_watts", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.panel_watts);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_fm_online", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.fm_online);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_fm_mode", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.fm_mode);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_em540_online", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.em540_online);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_bmc_id", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], R.bmc_id);
		}
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "build_date", 64);
		cJSON_AddStringToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], FW_Date);
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "build_time", 64);
		cJSON_AddStringToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], FW_Time);
		time(&rawtime);
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "sequence_time", 64);
		cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], (double) rawtime);
		// convert the cJSON object to a JSON string
		char *json_str = cJSON_Print(json);

		pubmsg.payload = json_str;
		pubmsg.payloadlen = strlen(json_str);
		pubmsg.qos = QOS;
		pubmsg.retained = 0;

		MQTTClient_publishMessage(client_p, topic_p, &pubmsg, &token);
		// a busy, wait loop for the async delivery thread to complete
		{
			uint32_t waiting = 0;
			while (ha_flag_vars_ss.deliveredtoken != token) {
				usleep(TOKEN_DELAY);
				if (waiting++ > MQTT_RETRY) {
					if (spam++ > 1) {
						fprintf(fout, "%s SW mqtt_bmc_data, Still Waiting, timeout\r\n", log_time(false));
						fflush(fout);
						spam = 0;
					}
					break;
				} else {
					spam = 0;
				}
			};
		}

		cJSON_free(json_str);
		cJSON_Delete(json);
		BMC4.bmc_flag = true;
	}
}

/*
 * main program function to send Comedi data to the MQTT server
 */
void comedi_push_mqtt(void)
{
	mqtt_bmc_data(E.client_p, ha_daq_host.topics[ha_daq_host.hindex]);
}

double ac0_filter(const double raw)
{
	static double accum = 0.0f;
	static double coef = COEF;
	accum = accum - accum / coef + raw;
	return accum / coef;
}

double ac1_filter(const double raw)
{
	static double accum = 0.0f;
	static double coef = COEF;
	accum = accum - accum / coef + raw;
	return accum / coef;
}

/*
 * check the BMC CSV data string for proper format, size and checkmark
 */
char * validate_bmc_text(const char * text, bool * valid)
{
	char tmp_test_ptr[512], *tmp_p = (char *) text;
	uint32_t len = 0, starts = 0, checkmark = 0;
	bool end_data = false;
	char *jtoken;

	validate_failure = 0;
	strncpy(tmp_test_ptr, text, 511);
	valid[0] = true;
	if (tmp_test_ptr[0] == '^') {
		starts++;
		tmp_p = (char *) &text[0]; // return pointer to start of possible data
		if ((len = strlen(tmp_test_ptr)) >= VALIDATE_LEN) {
			for (int i = 1; i <= len; i++) {
				if (tmp_test_ptr[i] == '^') {
					starts++;
					end_data = false;
					tmp_p = (char *) &text[i]; // return pointer to start of possible data
				}
				if (tmp_test_ptr[i] == '~') {
					if (starts) {
						end_data = true;
					}
				}
			}
			if (end_data == false) {
				valid[0] = false;
				validate_failure = 3;
			}
			strncpy(tmp_test_ptr, tmp_p, 511);
			jtoken = strtok(tmp_test_ptr, ",");
			if (jtoken != NULL) {
				for (int i = 0; i < CSV_COUNT; i++) {
					jtoken = strtok(NULL, ",");
					if (jtoken == NULL) {
						valid[0] = false;
						validate_failure = 4;
					}
				}
				jtoken = strtok(NULL, ",");
				if (jtoken != NULL) {
					checkmark = atoi(jtoken);
					if (checkmark != CHECKMARK) {
						valid[0] = false;
						validate_failure = 6;
					}
				} else {
					valid[0] = false;
					validate_failure = 5;
				}
			}
		} else {
			valid[0] = false;
			validate_failure = 2;
		}
	} else {
		valid[0] = false;
		validate_failure = 1;
	}

	return tmp_p;
}