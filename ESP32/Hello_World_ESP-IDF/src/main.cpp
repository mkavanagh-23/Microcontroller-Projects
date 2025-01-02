#include "main.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#define LOG_TAG "MAIN"

static Main my_main;

extern "C" void app_main(void) {
  ESP_ERROR_CHECK(my_main.setup());
  while(true) {
    my_main.run();
  }
}

esp_err_t Main::setup(void){
  esp_err_t status{ESP_OK};
  return status;
}

void Main::run(void){
  ESP_LOGI(LOG_TAG, "Hello, world!");
  vTaskDelay(pdMS_TO_TICKS(1000));  // Delay by 1000 ms
}
