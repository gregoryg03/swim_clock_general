#ifndef EVENTS_H
#define EVENTS_H

#include <Arduino.h>

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

void handle_events(void);

#endif