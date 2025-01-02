#include "main.h"
#include "freertos/idf_additions.h"

#define LOG_LEVEL_LOCAL ESP_LOG_VERBOSE
#include "esp_log.h"
#define LOG_TAG "MAIN"

extern "C" void app_main(void) {
  while(true) {
    ESP_LOGI(LOG_TAG, "Hello World!");
    vTaskDelay(1000);   // Delay by 100 ticks
  }
}