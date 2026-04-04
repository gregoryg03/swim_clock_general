#ifndef CLOCK_FUNCTIONS_H
#define CLOCK_FUNCTIONS_H

#include <Arduino.h>
#include "events.h"

#define segCount 4
#define BLANK 0

struct dispStruct {
    uint8_t digitarr[segCount];
    bool blinkState;
    bool blinkMask[segCount];
};

void displayinit(uint8_t, uint8_t, uint8_t);

void getInterval(void);
void countDown(void);
void countUp(void);
void data_entry_disp(dispStruct&, uint16_t);
bool dispData(dispStruct&);

    
#endif