
#ifndef BMC_MQTT_H
#define BMC_MQTT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bmc.h"
#include "daq.h"

#define MQTT_RETRY 10

#define HA_SW_DELAY     400000  // usecs
#define TOKEN_DELAY     600
#define GTI_TOKEN_DELAY 300

#define MAIN_DELAY      100000 // 1msec comedi sample rate max

#define QOS             1

#define RDEV_SIZE        10

#define SLEEP_CODE      0
#define FLOAT_CODE      1
        //#define DEBUG_REC
        //#define GET_DEBUG

#define MBMQTT  1024

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

        struct ha_daq_hosts_type {
                const char hosts[4][NI_MAXHOST];
                const char clients[4][NI_MAXHOST];
                char hname[4][NI_MAXHOST];
                double scaler[4];
                uint8_t hindex;
        };

        extern struct ha_flag_type ha_flag_vars_ss, ha_daq_hosts_type;
        extern struct ha_daq_hosts_type ha_daq_host;

        void mqtt_bmc_data(MQTTClient, const char *);
        void delivered(void *, MQTTClient_deliveryToken);
        int32_t msgarrvd(void *, char *, int, MQTTClient_message *);
        void connlost(void *, char *);
        void showIP(void);
        void skeleton_daemon(void);
        void bmc_mqtt_init(void);
        char * log_time(bool);

        void timer_callback(int32_t);
        void comedi_push_mqtt(void);

#ifdef __cplusplus
}
#endif

#endif /* BMC_MQTT_H */

