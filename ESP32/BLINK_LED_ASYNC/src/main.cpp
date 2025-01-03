// A simple example Asynchronous program to illustrate basic task scheduling
// The built-in LED blinks at a constant rate on one thread.
// Another thread blinks a series of 4 colored LEDs in sequence at a dynamic rate
// A potentiometer is used in another thread to set the rate of blinking for the LED sequence
// Special care must be taken to prevent access to the delay variable by multiple threads at the same time

#include <array>
#include <stdio.h>
#include"../include/main.h"

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "hal/gpio_types.h"
#include "portmacro.h"
#include "soc/gpio_num.h"

#define LOG_TAG "MAIN"

// Define output pins for LEDs
#define LED_BUILTIN GPIO_NUM_2
#define LED_RED GPIO_NUM_16
#define LED_BLUE GPIO_NUM_17
#define LED_GREEN GPIO_NUM_18
#define LED_YELLOW GPIO_NUM_19

// Define potentiometer input values
#define POT_PIN GPIO_NUM_36
#define POT_ADC ADC1_CHANNEL_0
#define ADC_WIDTH ADC_WIDTH_BIT_12      // 12-bit ADC resolution (values from 0 to 4095)
#define ADC_ATTEN ADC_ATTEN_DB_11       // No attenuation, 0dB, works for the 0-3.3V range

// Delay variables
constexpr int STATIC_DELAY{2000};
int dynamicDelay{200};
int delayTime{dynamicDelay};
static SemaphoreHandle_t delayMutex = NULL; // Create a mutex to prevent a race condition

// Store the LEDs in an array
std::array<gpio_num_t, 4> LEDs = {
  LED_RED,
  LED_BLUE,
  LED_GREEN,
  LED_YELLOW
};

static Main my_main;
extern "C" void app_main(void) {
  ESP_ERROR_CHECK(my_main.setup());
    my_main.run();
}

esp_err_t Main::setup(void){
  char* ourTaskName = pcTaskGetName(NULL);
  ESP_LOGI(ourTaskName,"Starting up program...");

  // Initialize all LED pins
  ESP_LOGI("SETUP", "Setting GPIO pin modes...");
  gpio_set_direction(LED_BUILTIN, GPIO_MODE_OUTPUT);
  for(gpio_num_t LED : LEDs) {
    gpio_set_direction(LED, GPIO_MODE_OUTPUT);
  }

  // Initialize potentiometer
  ESP_LOGI("SETUP", "Initializing potentiometer ADC...");
  gpio_set_direction(POT_PIN, GPIO_MODE_INPUT);
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(POT_ADC, ADC_ATTEN);

  // Initialize the delay mutex
  delayMutex = xSemaphoreCreateMutex();
  if(delayMutex == NULL) {
    ESP_LOGE("SETUP", "Failed to create delay mutex");
    return ESP_FAIL;
  }

  return ESP_OK;
}

void Main::run(void){
  ESP_LOGI(LOG_TAG, "Starting built-in LED asynchronously...");
  xTaskCreate(builtinTask, "Blink Built-In", 2048, NULL, 2, NULL);
  ESP_LOGI(LOG_TAG, "Starting colored LED asynchronously...");
  xTaskCreate(colorTask, "Blink Colored", 2048, NULL, 3, NULL);
  ESP_LOGI(LOG_TAG, "Begin listening for potentiometer input");
  xTaskCreate(setDelay, "Set LED Delay Rate", 2048, NULL, 2, NULL);
}

void colorTask(void *pvParameter) {
  while(true){
    for(gpio_num_t LED : LEDs){
      gpio_set_level(LED, 1);
      updateDelay(delayTime);
      vTaskDelay(pdMS_TO_TICKS(delayTime));
      gpio_set_level(LED, 0);
      updateDelay(delayTime);
      vTaskDelay(pdMS_TO_TICKS(delayTime));
    }
  }
}

void builtinTask(void *pvParameter) {
  while(true){
    gpio_set_level(LED_BUILTIN, 1);
    vTaskDelay(pdMS_TO_TICKS(STATIC_DELAY));  // Delay by 1000 ms
    gpio_set_level(LED_BUILTIN, 0);
    vTaskDelay(pdMS_TO_TICKS(STATIC_DELAY));  // Delay by 1000 ms
  }
}

void setDelay(void *pvParameter) {
  while(true){
    int newValue = potMap(adc1_get_raw(POT_ADC));

    if(xSemaphoreTake(delayMutex, portMAX_DELAY) == pdTRUE) {
      if(abs(newValue - dynamicDelay) > 2) {
        dynamicDelay = newValue;
        ESP_LOGI("DELAY", "Colored LED delay set to: %d ms", dynamicDelay);
      }
      xSemaphoreGive(delayMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(300));
  }
}

void updateDelay(int& delay) {
  if(xSemaphoreTake(delayMutex, portMAX_DELAY) == pdTRUE) {
    delay = dynamicDelay; // Update delaytime reference variable
    xSemaphoreGive(delayMutex);
  }
}

int potMap(int value) {
  // Input range (0 to 4095)
  int input_min = 0;
  int input_max = 4095;

  // Output range (200 to 5000)
  int output_min = 50;
  int output_max = 1000;

  // Apply the linear mapping formula
  int output_value = ((value - input_min) * (output_max - output_min)) / (input_max - input_min) + output_min;

  return output_value;
}
