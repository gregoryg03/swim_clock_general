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
    dataEntry
};

enum class actions {
    running,
    paused,
    cycle,
    enter
};

mode handle_events(event);

void set_action (event);

void perform_action(void);

extern actions action;

extern mode m;

void remTime(void);

mode next_mode(mode);

void disp(mode);

#endif