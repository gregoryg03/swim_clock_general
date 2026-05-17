#ifndef EVENTS
#define EVENTS

#include "esp_timer.h"
#include <stdio.h>
#include <stdint.h>
#include "esp_log.h"
#include "display.h"

#define BLINK_RATE 500 //time in ms

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



#endif