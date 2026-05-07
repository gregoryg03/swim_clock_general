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
static int64_t lastDebounce[BTN_COUNT] = {0}; 

//Debug
static const char *TAG = "sd_debug";

//Function to read the inputs and latch shift reg
static inline void latch_btns(void)
{
    gpio_set_level(pins.mode, 0);
    //esp_rom_delay_us(5);

    gpio_set_level(pins.mode, 1);
    //esp_rom_delay_us(5);
}

//Initilize buttons
void ShiftReg::buttons_init(gpio_num_t data_in, gpio_num_t clock_in, gpio_num_t mode_in)
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

    ret = gpio_set_level(pins.clock, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "CLK Fail Init");
    }

    ret = gpio_set_level(pins.mode, 0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Mode Fail Init");
    }

}

Event ShiftReg::poll()
{
    uint8_t reading = 0;
    uint8_t pressed = 0;

    reading = read_button(false);
    pressed = edge_detect(reading);

    ESP_LOGI(TAG, "Polling Buttons");

    if ((pressed >> 0) & 1)
        return Event::btn1press;

    if ((pressed >> 1) & 1)
        return Event::btn2press;
    
    if ((pressed >> 2) & 1)
        return Event::btn3press;
    
    if ((pressed >> 3) & 1)
        return Event::btn4press;

    if ((pressed >> 4) & 1)
        return Event::btn5press;
    
    return Event::none;
        
}

uint8_t ShiftReg::edge_detect(uint8_t reading)
{
    static uint8_t btnEdgeDetect = 0;
    uint8_t output = reading & ~btnEdgeDetect;
    btnEdgeDetect = reading;

    return output;
}

uint8_t ShiftReg::read_button(bool order)
{
    fill_reg();

    uint8_t value = 0;

    switch (order) {
        //MSBF
        case true:
            for (int i = 0; i < BTN_COUNT; i++) {
                value |= (digitalRead(pins.data) << i);
                digitalWrite(pins.clock, HIGH);
                digitalWrite(pins.clock, LOW);
            }
            break;
        //LSBF
        default:
        for (int i = 0; i < BTN_COUNT; i++) {
            value |= (digitalRead(pins.data) << (7-i));
            digitalWrite(pins.clock, HIGH);
            digitalWrite(pins.clock, LOW);
        }
        break;
    }

    return ~update_state(value);
}

uint8_t ShiftReg::update_state(uint8_t value)
{
  byte mask = 1;
  int64_t curtime = esp_timer_get_time();
  
  for (int i = 0; i < BTN_COUNT; i++) {
    if (((value >> i) & mask) != ((last_read >> i) & mask)) {
      lastDebounce[i] = curtime;
      last_read = (last_read & ~(1 << i)) | (((value >> i) & 1) << i);
    }
  }

  for (int i = 0; i < BTN_COUNT; i++) {
    if ((curtime - lastDebounce[i]) > DEBOUNCE) {
      bool currentBit = (value >> i) & 1;
      last_state = (last_state & ~(1 << i)) | (currentBit << i);
    }
  }

return last_state;  
}