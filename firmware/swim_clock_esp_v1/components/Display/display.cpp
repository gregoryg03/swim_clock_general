#include "display.h"

typedef struct {
    gpio_num_t latch;
    gpio_num_t clock;
    gpio_num_t data;
} disp_pins_t;

static disp_pins_t disp_pins;

//digit array 0-9 without DP
const int datArray[10] = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11100110};

//updside down digit array 0-9 without DP
const int invdatArray[10] = {0b11111100, 0b00001100, 0b11011010, 0b10011110, 0b00101110, 0b10110110, 0b11110110, 0b00011100, 0b11111110, 0b00111110};

//Random display items       d            n           u         p            g
const int symArray[5] = {0b01111010, 0b00101010, 0b00111000, 0b11001110, 0b11110110};
const int symArrayinv[5] = {0b11001110, 0b00101010, 0b11000100, 0b01111010, 0b11110110};

//Testing intervals to cycle through (sec)
const int timearr[10] = {20, 5, 20, 5, 30, 10, 60, 10, 20, 3000};
int intevaltime = 0;

//decimal Pt
uint8_t dp = 0b00000000;

//Debugging
esp_err_t ret;
static const char *TAG = "display";


void display_init(gpio_num_t LATCH, gpio_num_t CLOCK, gpio_num_t DATA)
{
    disp_pins.latch = LATCH;
    disp_pins.clock = CLOCK;
    disp_pins.data = DATA;

    gpio_config_t io_config_out{};

    io_config_out.mode = GPIO_MODE_OUTPUT;
    io_config_out.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config_out.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config_out.intr_type = GPIO_INTR_DISABLE;
    io_config_out.pin_bit_mask = BIT64(disp_pins.latch) | BIT64(disp_pins.clock) | BIT64(disp_pins.data);

    ret = gpio_config(&io_config_out);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Output Fail Init");
    }

    gpio_set_level(disp_pins.latch, 0);

}
