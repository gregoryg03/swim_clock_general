#ifndef BUTTON_POLL
#define BUTTON_POLL

#include <stdio.h>
#include <stdint.h>
#include "events.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include <sys/stat.h>
#include "esp_log.h"
#include "esp_timer.h"


#define BTN_COUNT 8
#define DEBOUNCE 30 //time in ms

class ShiftReg {
    public: 
    Event poll();
    void buttons_init(gpio_num_t, gpio_num_t, gpio_num_t);

    private:
    uint8_t read_button(bool);
    uint8_t update_state(uint8_t);
    uint8_t edge_detect(uint8_t);
};

#endif 