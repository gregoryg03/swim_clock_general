#ifndef EVENTS
#define EVENTS

#include "esp_timer.h"
#include <stdio.h>
#include <stdint.h>
#include "esp_log.h"
//#include "sd_card.h"


#define BLINK_RATE 500000 //time in ms

struct dispStruct;

enum class Event {
    none,
    btn1press,
    btn2press,
    btn3press,
    btn4press,
    btn5press,
    };

enum class Mode {
    countUp,
    countDown,
    dataEntry,
    shutdown,
    MAX_MODES
};

struct modeState {
    void(*enter)();
    void(*run)();
    void(*exit)();
};

enum class actions {
    running,
    paused,
    cycle,
    enter
};

void enter_countUp();
void run_countUp();
void exit_countUp();

void enter_countDown();
void run_countDown();
void exit_countDown();

void enter_dataEntry();
void run_dataEntry();
void exit_dataEntry();

void enter_shutdown();
void run_shutdown();
void exit_shutdown();

extern modeState modeTable[];

enum class Actions {
    running,
    paused,
    cycle,
    enter
};

void init_events(void);

Mode handle_events(Event);

void set_action(Event);

void get_interval(dispStruct *);

void count_down(dispStruct *);

void next_mode(Mode);

void change_state(Mode, Mode);

void countDown(dispStruct *);

void countUp(dispStruct *);

void helper_disp_pg(Mode);

void data_entry_disp(uint16_t);

uint16_t sd_data_in_format(uint8_t []);


#endif