#ifndef DISPLAY
#define DISPLAY

#include <stdio.h>
#include <stdint.h>
#include "events.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include <sys/stat.h>
#include "esp_log.h"
#include "esp_timer.h"

#define BLINK_RATE 500 //time in ms

struct dispStruct {
    uint8_t digitarr[4];
    bool dp_t[4];
    bool blinkState;
    bool counting;
    bool count_flag; //this is set true when countdown is complete
    bool blinkMask[4];
};

void disp_set(dispStruct *);

void display_init(gpio_num_t, gpio_num_t, gpio_num_t);

void remTime(void);

void shift_out(uint8_t);

void next_mode(Mode);

void disp(Mode);

void reset_disp();

void change_state(Mode);

uint16_t sd_data_in_format(uint8_t[]);

#endif