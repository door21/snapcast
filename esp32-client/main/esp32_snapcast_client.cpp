#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include <freertos/event_groups.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <esp_wifi.h>
#include <esp_wifi_types.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <string>
#include <string.h>
#include <iostream>

#include <aixlog.hpp>

#include "snapcast-client.hpp"
#include "controller.hpp"


#include "secrets.h"

extern "C" {
    int app_main(void);
}
static const char * log_tag =  "snapcast-client-main";
static EventGroupHandle_t eventGroup;
esp_err_t event_handler(void *ctx, system_event_t *event)
{
   if (event->event_id == SYSTEM_EVENT_STA_GOT_IP) {
      ESP_LOGI(log_tag, "Our IP address is " IPSTR "\n",
         IP2STR(&event->event_info.got_ip.ip_info.ip));
      ESP_LOGI(log_tag, "Connected to WiFi\n");
      xEventGroupSetBits(eventGroup, EVENT_GROUP_WIFI_CONNECTED);
   }
   if (event->event_id == SYSTEM_EVENT_STA_START) {
      ESP_ERROR_CHECK(esp_wifi_connect());
   }
   else if(event->event_id == SYSTEM_EVENT_STA_DISCONNECTED){
      xEventGroupClearBits(eventGroup, EVENT_GROUP_WIFI_CONNECTED);
      ESP_ERROR_CHECK(esp_wifi_connect());
      xEventGroupSetBits(eventGroup, EVENT_GROUP_WIFI_RECONNECTING);
   }
   return ESP_OK;
}

void wifi_connect_func(void *pvParams){
   wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
   ESP_ERROR_CHECK(esp_wifi_init(&cfg) );
   ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
   ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA));
   wifi_config_t sta_config = { };
   strcpy((char *)sta_config.sta.ssid, SSID);
   strcpy((char *)sta_config.sta.password, PASSWORD);
   ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &sta_config));
   ESP_ERROR_CHECK(esp_wifi_start());
   vTaskDelete(NULL);
}

static std::string hostid = "esp32-snap-client";
static Controller controller(hostid, 1, NULL);
void run_connection_func(void *pv){

   while(true){
      xEventGroupWaitBits(eventGroup, EVENT_GROUP_WIFI_CONNECTED, pdTRUE, pdTRUE, portMAX_DELAY);
      ESP_LOGI(log_tag, "About to start controller");
      std::string str = SERVER_HOST;
      PcmDevice device;
      ESP_LOGI(log_tag, "Ready to call start...");
      controller.start(device, str, SERVER_PORT, 100);
   }
}

void disconnect_func(void *pv){
   while(true){
      xEventGroupWaitBits(eventGroup, EVENT_GROUP_WIFI_DISCONNECTED, pdTRUE, pdTRUE, portMAX_DELAY);
      //controller.stop();
   }
}

int app_main(void)
{
   eventGroup = xEventGroupCreate();
   xEventGroupClearBits(eventGroup, 0xff);
   nvs_flash_init();
   tcpip_adapter_init();
   AixLog::Log::init<AixLog::SinkEsp32Logging>("snapclient", AixLog::Severity::trace, AixLog::Type::all);
   ESP_ERROR_CHECK( esp_event_loop_init(event_handler, NULL) );
   TaskHandle_t wifi_task, run_task, disconnect_task;
   xTaskCreate(wifi_connect_func, "wifi_connect", 8192, NULL, 5, &wifi_task);
   xTaskCreate(run_connection_func, "run_connection", 8192, NULL, 5, &run_task);
   xTaskCreate(disconnect_func, "disconnect", 8192, NULL, 5, &disconnect_task);
   SLOG(NOTICE) << "daemon started" << std::endl;
   return 0;
}
