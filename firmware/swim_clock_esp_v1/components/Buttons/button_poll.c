#include "button_poll.h"

//74HC165 Shift Register
//Data Out Inverse
static gpio_num_t DATA_PIN;
//Clock
static gpio_num_t CLOCK_PIN;
//Shift/Load High = Shift data & Low = Load Data
static gpio_num_t MODE_PIN;

//GPIO type
static const gpio_mode_t PIN_TYPE = GPIO_MODE_INPUT;

//Debounce
static uint8_t last_read = 0xFF;
static uint8_t last_state = 0xFF;
static unsigned long lastDebounce[BTN_COUNT] = {0};

//Debug
static const char *TAG = "sd_debug";

//Initilize buttons
void buttons_init(gpio_num_t data, gpio_num_t clock, gpio_num_t mode)
{
    ESP_LOGI(TAG, "Initilizing Buttons");

    esp_err_t ret;

    gpio_config_t io_config_out = {};
    gpio_config_t io_config_in = {};

    io_config_out.mode = GPIO_MODE_OUTPUT;
    


    DATA_PIN = data;
    CLOCK_PIN = clock;
    MODE_PIN = mode;

    ret = gpio_set_direction(DATA_PIN, GPIO_MODE_INPUT);
    if (ret != ESP_OK) {
        ESP_LOGI(TAG, "DATA Fail Init");
    }

    ret gpio_set_direction(CLOCK_PIN, GPIO_MODE_OUTPUT);
    if (ret != ESP_OK) {
        ESP_LOGI(TAG, "CLK Fail Init");
    }

    ret gpio_set_direction(MODE_PIN, GPIO_MODE_OUTPUT);
    if (ret != ESP_OK) {
        ESP_LOGI(TAG, "CLK Fail Init");
    }

    // pinMode(DATA_PIN, INPUT);
    // pinMode(CLOCK_PIN, OUTPUT);
    // pinMode(MODE_PIN, OUTPUT);

    // digitalWrite(CLOCK_PIN, LOW);
    // digitalWrite(MODE_PIN, LOW);
}