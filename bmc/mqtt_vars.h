

#ifndef MQTT_VARS_H
#define MQTT_VARS_H

#ifdef __cplusplus
extern "C" {
#endif

#define HA_SW_DELAY     400000  // usecs
#define TOKEN_DELAY     250
#define GTI_TOKEN_DELAY 300

#define QOS             1

	void mqtt_ha_switch(MQTTClient, const char *, const bool);
	void mqtt_ha_pid(MQTTClient, const char *);
	void mqtt_ha_shutdown(MQTTClient, const char *);
	bool mqtt_gti_power(MQTTClient, const char *, char *, uint32_t);
	bool mqtt_gti_time(MQTTClient, const char *, char *);


#ifdef __cplusplus
}
#endif

#endif /* MQTT_VARS_H */

