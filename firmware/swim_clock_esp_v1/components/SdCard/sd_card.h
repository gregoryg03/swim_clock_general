#ifndef SD_CARD_H
#define SD_CARD_H

#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>

#include "driver/gpio.h"
#include "driver/spi_common.h"
#include "driver/sdspi_host.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "freertos/FreeRTOS.h"
#include "esp_log.h"

#define MAX_CHAR_SIZE 64
#define MOUNT_POINT "/sdcard"


struct sdData {
    bool countFlag;
    uint8_t data[4]; 
};

//Function prototypes
void sd_init(gpio_num_t, gpio_num_t, gpio_num_t, gpio_num_t);

static esp_err_t write_file(const char *, char *);

//edit this to take in a pointer for the output so it can be used in main
static esp_err_t read_file(const char *);


#endif