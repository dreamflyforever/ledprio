#include <led_status_api.h>

#if 1
#define log(format, ...)\
	{printf("[%s : %d] ", \
	__func__, __LINE__); \
	printf(format, ##__VA_ARGS__);}

#else
#define pf(format, ...)
#endif

#define linux 1
typedef enum{
	LED_RED = 0,
	LED_GREEN,
	LED_BLUE,
} led_color_t;

#define C_LED_ON 1
#define C_LED_OFF 0
#define C_LED_FASTF 1
#define C_LED_SLOWF 0

#if linux
int set_led_onoff(led_color_t c,int mode)
{
	log("c: %d, mode : %d\n", c, mode);
	return 0;
}

int set_led_flashing(led_color_t c,int mode)
{
	log("c: %d, mode : %d\n", c, mode);
	return 0;
}
#endif

int led_set(int *sys_status, int status)
{
	if (status < 0) return 0;
	*sys_status = *sys_status | (1 << status);
	log(" sys_status: 0x%x\n",  *sys_status);
	return 0;
}

int led_status_clear(int *sys_status, int status)
{
	*sys_status = *sys_status & (~(1 << status));
	return 0;
}

int led_update(int *status, int s, int flag)
{
	/*init status*/
	static int st = -1;
	static int stt = -1;
	int i;
	/*prirority search*/
	for (i = 0; i < SYSTEM_STARTING; i++) {
		if (0 != (*status & (1 << i))) break;
	}
	log("i: %d, status : 0x%x, s: %d\n", i, *status, s);
	if ((st == i) && (stt == s)) {
		return 0;
	}
	st = i;
	stt = s;

	switch (i) {
	case NETWORK_AIRKISS:
		set_led_onoff(LED_GREEN, C_LED_OFF);
		set_led_onoff(LED_BLUE, C_LED_OFF);
		set_led_onoff(LED_RED, C_LED_OFF);

		set_led_flashing(LED_RED, C_LED_FASTF);
		set_led_flashing(LED_GREEN, C_LED_FASTF);
		break;
	case UPDATING:
		set_led_onoff(LED_GREEN, C_LED_OFF);
		set_led_onoff(LED_BLUE, C_LED_OFF);
		set_led_onoff(LED_RED, C_LED_OFF);

		set_led_flashing(LED_BLUE, C_LED_FASTF);
		break;
	case LOWPOWER:
		set_led_onoff(LED_GREEN, C_LED_OFF);
		set_led_onoff(LED_BLUE, C_LED_OFF);
		set_led_onoff(LED_RED, C_LED_OFF);

		set_led_flashing(LED_RED, C_LED_FASTF);
		break;
	case NETWORK_CONNECTED:
		set_led_onoff(LED_GREEN, C_LED_OFF);
		set_led_onoff(LED_BLUE, C_LED_OFF);
		set_led_onoff(LED_RED, C_LED_OFF);

		/*yellow led flash*/
		set_led_onoff(LED_GREEN, C_LED_ON);
		set_led_onoff(LED_RED, C_LED_ON);
		break;
	case WIFI_NOT_CONNECTED:
		set_led_onoff(LED_GREEN, C_LED_OFF);
		set_led_onoff(LED_BLUE, C_LED_OFF);
		set_led_onoff(LED_RED, C_LED_OFF);

		set_led_onoff(LED_RED,C_LED_ON);
		break;
	case WECHAT_NOT_READ:
	case PLAY_STOP:
		if ((s != WECHAT_NOT_READ) && (s != PLAY_STOP)) {
			break;
		}
		set_led_onoff(LED_GREEN, C_LED_OFF);
		set_led_onoff(LED_BLUE, C_LED_OFF);
		set_led_onoff(LED_RED, C_LED_OFF);

		if (flag == 1) {
			if (s != WECHAT_NOT_READ) {
				set_led_flashing(LED_GREEN, C_LED_SLOWF);
				set_led_flashing(LED_RED, C_LED_SLOWF);
			} else {
				set_led_flashing(LED_RED, C_LED_SLOWF);
			}

		} else {
			if (s == WECHAT_NOT_READ) {
				set_led_flashing(LED_GREEN, C_LED_SLOWF);
				set_led_flashing(LED_RED, C_LED_SLOWF);
			} else {
				set_led_flashing(LED_RED, C_LED_SLOWF);
			}
		}
		break;
	default:
		set_led_onoff(LED_GREEN, C_LED_OFF);
		set_led_onoff(LED_BLUE, C_LED_OFF);
		set_led_onoff(LED_RED, C_LED_OFF);

		set_led_onoff(LED_BLUE, C_LED_ON);
	}
end:
	return 0;
}

int led_status_set_update(int *sys, int status)
{
	log(">>>>>>>>>>>>set status %d <<<<<<<<\n", status);
	led_set(sys, status);
	led_update(sys, status, 0);
	return 0;
}

int led_status_clear_update(int *sys, int status)
{
	log(">>>>>>>>>>>>clear status %d <<<<<<<<\n", status);
	led_status_clear(sys, status);
	led_update(sys, status, 1);
	return 0;
}

#if linux
int g_led_status = 0;
int main()
{
	/*system starting*/
	led_status_set_update(&g_led_status, SYSTEM_STARTING);
	/*wifi connect*/
	led_status_set_update(&g_led_status, NETWORK_CONNECTED);
	/*play stop*/
	led_status_set_update(&g_led_status, PLAY_STOP);
	/*play resume*/
	led_status_clear_update(&g_led_status, PLAY_STOP);
	/*UPDATING*/
	led_status_set_update(&g_led_status, UPDATING);
	/*UPDATING end*/
	led_status_clear_update(&g_led_status, UPDATING);

	return 0;
}
#endif
