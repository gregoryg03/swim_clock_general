#ifndef CLOCK_FUNCTIONS_H
#define CLOCK_FUNCTIONS_H

#include <Arduino.h>
#include "events.h"

#define segCount 4
#define BLANK 0

void displayinit(uint8_t, uint8_t, uint8_t);

void getInterval(void);
void countDown(void);
void countUp(void);
void data_entry_disp(uint8_t, uint16_t);


    
#endif