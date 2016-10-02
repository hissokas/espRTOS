

#include "esp_common.h"
#include "esp_misc.h"
#include "espressif/esp_softap.h"
#include "espressif/esp_timer.h"
#include "../fatfs/diskio.h"
#include "../driver/gpio.h"

os_timer_t mmc_timer;

void disk_timerproc(void);

// start timer that invoke delay period for FatFs - disk_timerproc() -> mmc_esp8266.c
static void start_os_timer_mmc(void)
{
	os_timer_disarm(&mmc_timer);
	os_timer_setfn(&mmc_timer, (os_timer_func_t *)disk_timerproc, NULL);
	os_timer_arm(&mmc_timer, 5, 0x01); // 5ms is minimal timeout for os_timer
}

void LEDBlinkTask (void *pvParameters)
{
    while(1)
    {
    // Delay and turn on
    vTaskDelay (300/portTICK_RATE_MS);
        GPIO_OUTPUT_SET (2, 1);

    // Delay and LED off
        vTaskDelay (300/portTICK_RATE_MS);
        GPIO_OUTPUT_SET (2, 0);
    }
}

void DiskTask (void *pvParameters)
{
	printf("status %d\n", disk_initialize(0));
	vTaskSuspend(NULL);
}

void user_init(void)
{
    printf("SDK version:%s\n", system_get_sdk_version());
 	printf("PATLAS\n");
 	start_os_timer_mmc();
// 	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO4_U, FUNC_GPIO4);
// 	GPIO_AS_OUTPUT(1<<4);
//	GPIO_OUTPUT_SET(4, 1);
 	xTaskCreate(DiskTask, (signed char *)"Blink", 256, NULL, 2, NULL);
	printf("PATLAS2\n");
//	disk_initialize(0);
//printf("status %d\n", disk_initialize(0));
 	//TODO add printf to timer to see if works


	//struct softap_config config;
	//wifi_softap_get_config(&config);
	//printf("SSID: %s, PASS: %s \n", config.ssid, config.password);

}

