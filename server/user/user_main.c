

#include "esp_common.h"
#include "esp_misc.h"
#include "espressif/esp_softap.h"
#include "espressif/esp_timer.h"
#include "../fatfs/diskio.h"
#include "../driver/gpio.h"
#include "../driver/spi.h"


#include "access_point.h"
#include "tcp_server.h"



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
	/*	while(1)
{
	//spi_tx8(HSPI,0xAB);
	//printf("%x\n",spi_rx8(HSPI));
	printf("%x ---\n",spi_rxtx8(HSPI, 0xAC));
	vTaskDelay(300/portTICK_RATE_MS);
	
}*/
	vTaskSuspend(NULL);
}

void user_init(void)
{
    printf("SDK version:%s\n", system_get_sdk_version());
 	printf("PATLAS\n");


 	start_os_timer_mmc();

	printf("PATLAS2\n");


	start_ap("AQUARIOUS", "patlas", 9, 6);
	printf("TCP server: %d\n", start_server());

	//xTaskCreate(DiskTask, (signed char *)"DiskTest", 256, NULL, 2, NULL);


}

