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
    idle,
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

actions action = actions::paused;
mode m = mode::idle;

#endif