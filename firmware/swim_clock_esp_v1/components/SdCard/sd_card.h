#ifndef SD_CARD_H
#define SD_CARD_H

#include <sys/stat.h>
#include "esp_log.h"

struct sdData {
    bool countFlag;
    uint8_t data[4]; 
};

#endif