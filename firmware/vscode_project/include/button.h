#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "events.h"

#define BTN_COUNT 8
#define DEBOUNCE 20 //time in ms

class buttons {
    public: 
    event poll();
};

void buttons_init(uint8_t, uint8_t, uint8_t);
void fill_reg(void);
byte read_button(bool);
byte update_state(byte);
byte edge_detect(byte);


#endif