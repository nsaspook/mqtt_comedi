
#ifndef BMC_MQTT_H
#define BMC_MQTT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "bmc.h"
#include "daq.h"
#include "mqtt_rec.h"
#include "mqtt_vars.h"

#define MQTT_RETRY 10

        extern struct ha_flag_type ha_flag_vars_ss;

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

