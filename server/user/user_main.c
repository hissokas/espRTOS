

#include "esp_common.h"
#include "esp_misc.h"
#include "espressif/esp_softap.h"
#include "espressif/esp_timer.h"
#include "diskio.h"

os_timer_t mmc_timer;

void disk_timerproc(void);

// start timer that invoke delay period for FatFs - disk_timerproc() -> mmc_esp8266.c
static void start_os_timer_mmc(void)
{
	os_timer_disarm(&mmc_timer);
	os_timer_setfn(&mmc_timer, (os_timer_func_t *)disk_timerproc, NULL);
	os_timer_arm(&mmc_timer, 5); // 5ms is minimal timeout for os_timer
}

void user_init(void)
{
    printf("SDK version:%s\n", system_get_sdk_version());
 	printf("PATLAS\n");
 	start_os_timer_mmc();
 	//TODO add printf to timer to see if works

	
	struct softap_config config;
	wifi_softap_get_config(&config);
	printf("SSID: %s, PASS: %s \n", config.ssid, config.password);

}

