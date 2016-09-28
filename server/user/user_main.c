

#include "esp_common.h"
#include "esp_misc.h"
#include "espressif/esp_softap.h"

void user_init(void)
{
    	printf("SDK version:%s\n", system_get_sdk_version());
 	printf("PATLAS\n");
	
	struct softap_config config;
	wifi_softap_get_config(&config);
	printf("SSID: %s, PASS: %s \n", config.ssid, config.password);

}

