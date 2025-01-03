#ifndef MAIN_H
#define MAIN_H

#include "esp_err.h"

class Main final {
  public:
    esp_err_t setup();
    void run();
};

void colorTask(void *pvParameter);
void setDelay(void *pvParameter);
void updateDelay(int& delay);
int potMap(int value);

#endif
