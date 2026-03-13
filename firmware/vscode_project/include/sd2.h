#ifndef SD2_H
#define SD2_H

#include <Arduino.h>
#include "events.h"
#include <SPI.h>
#include <SD.h>

void sd_init(uint8_t);
void sd_clear(void);
void sd_write(uint16_t);
uint16_t sd_read(void);

struct sdAction {
    void(*clear)(void);
    void(*write)(uint16_t);
    uint16_t(*read)(void);
};

#endif