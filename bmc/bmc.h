/* 
 * File:   bmc.h
 * Author: root
 *
 * Created on September 21, 2012, 12:54 PM
 */

#ifndef BMC_H
#define BMC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h> /* for printf() */
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/time.h>
#include <errno.h>
#include <cjson/cJSON.h>
#include <curl/curl.h>
#include <pthread.h>
#include <sys/stat.h>
#include <syslog.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include "MQTTClient.h"
#include "bmc_mqtt.h"

#define LOG_VERSION     "V0.05"
#define MQTT_VERSION    "V3.11"
#define TNAME  "maint9"
#define LADDRESS        "tcp://127.0.0.1:1883"
#ifdef __amd64
#define ADDRESS         "tcp://10.1.1.172:1883"
#else
#define ADDRESS         "tcp://10.1.1.172:1883"
#endif
#define CLIENTID1       "Energy_Mqtt_BMC1"
#define CLIENTID2       "Energy_Mqtt_BMC2"
#define CLIENTID3       "Energy_Mqtt_BMC3"
#define TOPIC_P         
#define TOPIC_SPAM      "comedi/bmc/data/spam"
#define TOPIC_PACA      "home-assistant/comedi/bmc"
        //#define TOPIC_PACB      "mateq84/data/#"
#define TOPIC_AI        "comedi/bmc/data/ai"
#define TOPIC_AO        "comedi/bmc/data/ao"
#define TOPIC_DI        "comedi/bmc/data/di"
#define TOPIC_DO        "comedi/bmc/data/do"
#define QOS             1

#define TIMEOUT         10000L
#define SPACING_USEC    500 * 1000
#define USEC_SEC        1000000L

#define CMD_SEC         1
#define TIME_SYNC_SEC   30

#define SBUF_SIZ        16  // short buffer string size
#define RBUF_SIZ        82
#define SYSLOG_SIZ      512

#define LOG_TO_FILE         "/var/log/bmc/bmc_comedi.log"
#define LOG_TO_FILE_ALT     "/tmp/bmc_comedi.log"

#define MQTT_RECONN     3
#define KAI             60

        /*
         * system testing defines
         * all should be undefined for normal operation
         */
        //#define DAC_TESTING
        //digital only
        //#define DIGITAL_ONLY

        extern FILE* fout; // logging stream
        extern struct energy_type E;

        struct energy_type {
                volatile bool once_gti, once_ac, iammeter, fm80, dumpload, homeassistant, once_gti_zero, comedi;
                volatile double gti_low_adj, ac_low_adj, dl_excess_adj;
                volatile bool ac_sw_on, gti_sw_on, ac_sw_status, gti_sw_status, solar_shutdown, solar_mode, startup, ac_mismatch, dc_mismatch, mode_mismatch, dl_excess;
                volatile uint32_t speed_go, im_delay, im_display, gti_delay, sequence, mqtt_count;
                volatile int32_t rc, sane;
                volatile uint32_t thirty_sec_clock, log_spam, log_time_reset;
                pthread_mutex_t ha_lock;
                volatile int16_t di_16b, do_16b;
                double adc[16], dac[16];
                MQTTClient client_p, client_sd, client_ha;
        };

        void led_lightshow(int);

#ifdef __cplusplus
}
#endif

#endif /* BMC_H */

