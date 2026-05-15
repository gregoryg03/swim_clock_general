#include <stdio.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include <sys/stat.h>
#include "esp_log.h"
#include "esp_timer.h"

// typedef struct {
//     gpio_num_t data;
//     gpio_num_t clock;
// } button_pins_t;

// static button_pins_t pins;

// //GPIO type
// static const gpio_mode_t PIN_TYPE = GPIO_MODE_INPUT;


// //Debug
// static const char *TAG = "sd_debug";
// esp_err_t ret;


// extern "C" {

//     void app_main(void)
//     {
//         ESP_LOGI(TAG, "Main running");

//         const gpio_num_t DATA_PIN = GPIO_NUM_6;
//         const gpio_num_t CLK_PIN = GPIO_NUM_5;
        

//         pins.data = DATA_PIN;
//         pins.clock = CLK_PIN;
        
//         gpio_config_t io_config_in{};

//         io_config_in.mode = GPIO_MODE_INPUT;
//         io_config_in.pull_up_en = GPIO_PULLUP_DISABLE;
//         io_config_in.pull_down_en = GPIO_PULLDOWN_DISABLE;
//         io_config_in.intr_type = GPIO_INTR_DISABLE;
//         io_config_in.pin_bit_mask = BIT64(pins.data);


        
//         gpio_config_t io_config_out{};

//         io_config_out.mode = GPIO_MODE_OUTPUT;
//         io_config_out.pull_up_en = GPIO_PULLUP_DISABLE;
//         io_config_out.pull_down_en = GPIO_PULLDOWN_DISABLE;
//         io_config_out.intr_type = GPIO_INTR_DISABLE;
//         io_config_out.pin_bit_mask = BIT64(pins.clock);

//         ret = gpio_config(&io_config_in);
//         if (ret != ESP_OK) {
//             ESP_LOGE(TAG, "Input Fail Init");
//         }

//         ret = gpio_config(&io_config_out);
//         if (ret != ESP_OK) {
//             ESP_LOGE(TAG, "Output Fail Init");
//         }

//          ret = gpio_set_level(pins.clock, 0);
//         if (ret != ESP_OK) {
//             ESP_LOGE(TAG, "CLK Fail Init");
//         }

//         ESP_LOGI(TAG, "Initilizing Buttons Done");
    

//         while (1) {

//             static int i = 0;

//             int value = 0;
//             value = gpio_get_level(pins.data);
//             ESP_LOGI(TAG, "%d", value);
//             vTaskDelay(pdMS_TO_TICKS(100));
            
//             if (i) {
//                 gpio_set_level(pins.clock, i);
//             }
//             else 
//                 gpio_set_level(pins.clock, 0);
            
//             i ^= 1;
//         }

//     }

// }









#define BTN_COUNT 8
// #define DEBOUNCE 30000 //time in ms


typedef struct {
    gpio_num_t data;
    gpio_num_t clock;
    gpio_num_t mode;
} button_pins_t;

static button_pins_t pins;

//GPIO type
static const gpio_mode_t PIN_TYPE = GPIO_MODE_INPUT;

//Debounce
// static uint8_t last_read = 0xFF;
// static uint8_t last_state = 0xFF;
// static int64_t lastDebounce[BTN_COUNT] = {0}; 

//Debug
static const char *TAG = "sd_debug";

//Function to read the inputs and latch shift reg
static inline void latch_btns(void)
{
    gpio_set_level(pins.mode, 0);
    esp_rom_delay_us(5);

    gpio_set_level(pins.mode, 1);
    esp_rom_delay_us(5);
}

//Initilize buttons
void buttons_init(gpio_num_t data_in, gpio_num_t clock_in, gpio_num_t mode_in)
{
    ESP_LOGI(TAG, "Initilizing Buttons");
    
    esp_err_t ret;

    pins.data = data_in;
    pins.clock = clock_in;
    pins.mode = mode_in;
    
    gpio_config_t io_config_out{};

    io_config_out.mode = GPIO_MODE_OUTPUT;
    io_config_out.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config_out.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config_out.intr_type = GPIO_INTR_DISABLE;
    io_config_out.pin_bit_mask = BIT64(pins.clock) | BIT64(pins.mode);


    gpio_config_t io_config_in{};

    io_config_in.mode = GPIO_MODE_INPUT;
    io_config_in.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config_in.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config_in.intr_type = GPIO_INTR_DISABLE;
    io_config_in.pin_bit_mask = BIT64(pins.data);

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
    ESP_LOGI(TAG, "Initilizing Buttons Done");
}

extern "C" {

    void app_main(void)
    {
        ESP_LOGI(TAG, "Main running");

        const gpio_num_t DATA_PIN = GPIO_NUM_6;
        const gpio_num_t CLOCK_PIN = GPIO_NUM_4;
        const gpio_num_t MODE_PIN = GPIO_NUM_5;

        buttons_init(DATA_PIN, CLOCK_PIN, MODE_PIN);

        while (1) {

            latch_btns();

            uint8_t value = 0;
            int v1 = 0, v2 = 0;

            for (int i = 0; i < BTN_COUNT; i++) {
                value |= (gpio_get_level(pins.data) << (7-i));
                //ESP_LOGI(TAG, "%d", gpio_get_level(pins.data));
                gpio_set_level(pins.clock, 1);
                esp_rom_delay_us(5);
                gpio_set_level(pins.clock, 0);
                esp_rom_delay_us(5);
            }
            
            // v1 = gpio_get_level(pins.data);
            // gpio_set_level(pins.clock, 1);
            // esp_rom_delay_us(5);
            // gpio_set_level(pins.clock, 0);
            // esp_rom_delay_us(5);
            // v2 = gpio_get_level(pins.data);
            // gpio_set_level(pins.mode, 0);

            // ESP_LOGI(TAG, "V1 %d, V2 %d", v1, v2);
            
            ESP_LOGI(TAG, "0x%02X", value);
            
            vTaskDelay(pdMS_TO_TICKS(100));
            
        }

    }

}