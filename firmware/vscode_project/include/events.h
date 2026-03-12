#ifndef EVENTS_H
#define EVENTS_H

#include <Arduino.h>
#include "functions.h"

enum class event {
    none,
    btn1press,
    btn2press,
    btn3press,
    btn4press,
    btn5press
    };

enum class mode {
    countUp,
    countDown,
    dataEntry,
    MAX_MODES
};

struct modeState {
    void(*enter)();
    void(*run)();
    void(*exit)();
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

extern modeState modeTable[];

enum class actions {
    running,
    paused,
    cycle,
    enter
};



mode handle_events(event);

void set_action (event);

extern actions action;

extern mode m;

void remTime(void);

void next_mode(mode);

void disp(mode);

void reset_disp();

void change_state(mode);

#endif