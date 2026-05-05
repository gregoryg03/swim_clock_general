#include "button_poll.h"

//74HC165 Shift Register
//Data Out Inverse
//Clock
//Shift/Load High = Shift data & Low = Load Data

typedef struct {
    gpio_num_t data;
    gpio_num_t clock;
    gpio_num_t mode;
} button_pins_t;

static button_pins_t pins;

//GPIO type
static const gpio_mode_t PIN_TYPE = GPIO_MODE_INPUT;

//Debounce
static uint8_t last_read = 0xFF;
static uint8_t last_state = 0xFF;
//static unsigned long lastDebounce[BTN_COUNT] = {0};  RTOS timing Update, Use ticks

//Debug
static const char *TAG = "sd_debug";

//Initilize buttons
void buttons_init(gpio_num_t data_in, gpio_num_t clock_in, gpio_num_t mode_in)
{
    ESP_LOGI(TAG, "Initilizing Buttons");
    
    esp_err_t ret;

    pins.data = data_in;
    pins.clock = clock_in;
    pins.mode = mode_in;
    
    gpio_config_t io_config_out = {0};

    io_config_out.mode = GPIO_MODE_OUTPUT;
    io_config_out.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config_out.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config_out.intr_type = GPIO_INTR_DISABLE;
    io_config_out.pin_bit_mask = BIT(pins.clock) | BIT(pins.mode);


    gpio_config_t io_config_in = {0};

    io_config_in.mode = GPIO_MODE_INPUT;
    io_config_in.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config_in.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config_in.intr_type = GPIO_INTR_DISABLE;
    io_config_in.pin_bit_mask = BIT(pins.data);

    ret = gpio_config(&io_config_out);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Output Fail Init");
    }

    ret = gpio_config(&io_config_in);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Input Fail Init");
    }

    ret = gpio_set_level(CLOCK_PIN, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "CLK Fail Init");
    }

    ret = gpio_set_level(MODE_PIN, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Mode Fail Init");
    }

}