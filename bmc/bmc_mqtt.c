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
	.hosts[3] = "10.1.2.39",
	.topics[0] = "comedi/bmc/data/bmc/1",
	.topics[1] = "comedi/bmc/data/bmc/2",
	.topics[2] = "comedi/bmc/data/bmc/3",
	.topics[3] = "comedi/bmc/data/bmc/4",
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
	.pacer[0] = 1500, // opiha
	.pacer[1] = 500, // daq1
	.pacer[2] = 1000, // daq2
	.pacer[3] = 500,
	.hindex = 0,
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
		MQTTClient_create(&E.client_p, ADDRESS, (const char *) &ha_daq_host.clients[ha_daq_host.hindex],
			MQTTCLIENT_PERSISTENCE_NONE, NULL);
		conn_opts_p.keepAliveInterval = KAI;
		conn_opts_p.cleansession = 1;
		hname_ptr = ADDRESS;
	}

	fprintf(fout, "\r\n%s Connect MQTT server %s, %s\n", log_time(false), hname_ptr, (const char *) &ha_daq_host.clients[ha_daq_host.hindex]);
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
		pacer = 0;
		fprintf(fout, "%s Sending Comedi data to MQTT server, Topic %s DO 0x%.4x DI 0x%.6x\n", log_time(false), topic_p, bmc.dataout.dio_buf, datain);
		if (bmc.BOARD == bmcboard) {
			fprintf(fout, "ANA0 %lfV, ANA1 %fV, ANA2 %f, ANA4 %fV, ANA5 %fV, AND5 %fV : Scaler Index %d, Scaler ANA4 %f, Scaler ANA5 %f\n",
				get_adc_volts(channel_ANA0), get_adc_volts(channel_ANA1), get_adc_volts(channel_ANA2),
				E.adc[channel_ANA4], E.adc[channel_ANA5], E.adc[channel_AND5], ha_daq_host.hindex, ha_daq_host.scaler4[ha_daq_host.hindex], ha_daq_host.scaler5[ha_daq_host.hindex]);
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
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "mqtt_di_16b", 64);
		cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], (double) E.di_16b);
		strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_adc0", 64);
		if (bmc.BOARD == bmcboard) {
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], E.adc[channel_ANA4]);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_adc1", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], E.adc[channel_ANA5]);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "build_date", 64);
		} else {
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], E.adc[channel_ANA0]);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "bmc_adc1", 64);
			cJSON_AddNumberToObject(json, (const char *) &ha_daq_host.hname[ha_daq_host.hindex], E.adc[channel_ANA1]);
			strncpy(&ha_daq_host.hname[ha_daq_host.hindex][5], "build_date", 64);
		}
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
	;
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