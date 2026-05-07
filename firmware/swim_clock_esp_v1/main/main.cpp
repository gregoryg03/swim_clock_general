#include <stdio.h>
#include <stdint.h>
#include "sd_card.h"
#include "button_poll.h"
#include "events.h"
#include <sys/stat.h>
#include "esp_log.h"

static const char *TAG = "main_debug";

const gpio_num_t LATCH_PIN = GPIO_NUM_16;
const gpio_num_t DISP_CLOCK_PIN = GPIO_NUM_19;
const gpio_num_t DISP_DATA_PIN = GPIO_NUM_17;
const gpio_num_t DATA_PIN = GPIO_NUM_6;
const gpio_num_t CLOCK_PIN = GPIO_NUM_4;
const gpio_num_t MODE_PIN = GPIO_NUM_5;
const gpio_num_t CS_PIN = GPIO_NUM_7;
const gpio_num_t MOSI = GPIO_NUM_22;
const gpio_num_t MISO = GPIO_NUM_21;
const gpio_num_t SD_CLK = GPIO_NUM_23;

ShiftReg shiftReg;

Event event;


void app_main(void)
{
    ESP_LOGI(TAG, "Main running");

    shiftReg.buttons_init(DATA_PIN, CLOCK_PIN, MODE_PIN);


while (1) {
    event = shiftReg.poll();

    vTaskDelay(pdMS_TO_TICKS(5));

    ESPL(TAG, "%d", static_cast<int>(event));
}

}
