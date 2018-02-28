#ifndef __LED_STATUS_API_H__
#define __LED_STATUS_API_H__

extern int g_led_status;

enum LED_STATUS {
	/*network airkiss & updating are the same priority*/
	NETWORK_AIRKISS = 0, 
	UPDATING,
	LOWPOWER,
	WIFI_NOT_CONNECTED,
	/*play stop & wechat not read are the same priority*/
	PLAY_STOP,
	WECHAT_NOT_READ,
	NETWORK_CONNECTED,
	SYSTEM_STARTING = 7
};

int led_set(int *sys_status, int status);
int led_status_clear(int *sys_status, int status);
int led_update(int *status, int s, int flag);
int led_status_set_update(int *sys, int status);
int led_status_clear_update(int *sys, int status);

#endif
