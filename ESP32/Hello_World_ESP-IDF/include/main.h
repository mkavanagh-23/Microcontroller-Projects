#ifndef MAIN_H
#define MAIN_H

#include "freertos/FreeRTOS.h"

class Main final {
  public:
    esp_err_t setup(void);
    void run(void);
};

#endif
