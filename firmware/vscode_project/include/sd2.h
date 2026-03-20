#ifndef SD2_H
#define SD2_H

#include <Arduino.h>
#include "events.h"
#include <SPI.h>
#include <SD.h>

void sd_init(uint8_t);
void sd_clear(void);
void sd_write(uint16_t);
bool sd_read(uint16_t &);
void sd_close(void);
void sd_read_init(void);
void sd_start(void);
void sd_end(void);

extern File clockData;

struct sdAction {
    void(*clear)(void);
    void(*write)(uint16_t);
    bool(*read)(uint16_t &);
    void(*dismount)(void);
};

struct writeState {
    void(*enter)();
    void(*run)(uint16_t);
    void(*exit)(void);
};

#endif