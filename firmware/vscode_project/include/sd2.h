#ifndef SD2_H
#define SD2_H

#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include "events.h"

enum sdAction {
    SD_OPEN,
    SD_WRITE,
    SD_READ,
    SD_CLOSE
};

enum sdMode {
    READ_MODE = FILE_READ,
    WRITE_MODE = FILE_WRITE
};

struct sdData {
    sdMode mode;
    uint16_t intervalIn;    //Interval being written to SD card
    uint16_t intervalOut;   //Interval being read from SD card
    bool success;           //Sucess flag for the action requested
};

using sdType = bool (*)(sdData&);

class SD_CARD {
    public:
        void sd_init(uint8_t);
        void sd_shutdown(void);

        bool call(sdAction, sdData&);

    private:
        static sdType actions[4];
};

bool sd_open(sdData&);
bool sd_write(sdData&);
bool sd_read(sdData&);
bool sd_close(sdData&);

#endif