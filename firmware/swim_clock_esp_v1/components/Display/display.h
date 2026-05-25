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

struct dispStruct{
    uint8_t digitarr[4];
    bool dp_t[4];
    bool blinkState;
    bool counting;
    bool count_flag; //this is set true when countdown is complete
    bool blinkMask[4];
};

typedef struct {
    uint8_t numbers[10];
    uint8_t inv_numbers[10];
    uint8_t symbols[10];
    uint8_t inv_symbols[10];
} disp_data_t;

typedef enum {
    DISP_DIG,
    //DISP_DIG_I,
    DISP_SYM,
    //DISP_SYM_I,

} disp_mode_t;

struct DispTables
{
    const uint8_t *normal;
    const uint8_t *inverse;
};

void disp_set(dispStruct *disp_t, 
                disp_mode_t mode
            );

void display_init(gpio_num_t, gpio_num_t, gpio_num_t);

void remTime(void);

void shift_out(uint8_t);

void reset_disp();


uint16_t sd_data_in_format(uint8_t[]);

#endif