#include "display.h"

typedef struct {
    gpio_num_t latch;
    gpio_num_t clock;
    gpio_num_t data;
} disp_pins_t;

static disp_pins_t disp_pins;


static const disp_data_t disp_data = {
    //digit array 0-9 without DP
    .numbers = {0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11100110},

    //updside down digit array 0-9 without DP
    .inv_numbers = {0b11111100, 0b00001100, 0b11011010, 0b10011110, 0b00101110, 0b10110110, 0b11110110, 0b00011100, 0b11111110, 0b00111110},

    //Random display items
    //             d            n           u            p           g          0           null
    .symbols = {0b01111010, 0b00101010, 0b00111000, 0b11001110, 0b11110110, 0b11111100, 0b00000000},
    .inv_symbols = {0b11001110, 0b00101010, 0b11000100, 0b01111010, 0b11110110, 0b11111100, 0b00000000}
};

const DispTables DigTable = {
    disp_data.numbers,
    disp_data.inv_numbers
};

const DispTables SymTable = {
    disp_data.symbols,
    disp_data.inv_symbols
};

//Testing intervals to cycle through (sec)

int intevaltime = 0;

//decimal Pt
uint8_t DP_MASK = 1;

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
    gpio_set_level(disp_pins.clock, 0);

    shift_out(0x00);
    gpio_set_level(disp_pins.latch, 0);
    
    vTaskDelay(pdMS_TO_TICKS(1000));
    gpio_set_level(disp_pins.latch, 1);
    esp_rom_delay_us(5);
    gpio_set_level(disp_pins.latch, 0);
    shift_out(disp_data.numbers[0]);
    shift_out(disp_data.numbers[0]);
    shift_out(disp_data.inv_numbers[0]);
    shift_out(disp_data.numbers[0]);
    gpio_set_level(disp_pins.latch, 1);


}

void shift_out(uint8_t value)
{
    for (int i = 0; i < 8; i++) {
        gpio_set_level(disp_pins.data, (1 & (value >> i)));
        esp_rom_delay_us(1);
        gpio_set_level(disp_pins.clock, 1);
        esp_rom_delay_us(1);
        gpio_set_level(disp_pins.clock, 0);
        esp_rom_delay_us(1);
    }
}

void disp_set(dispStruct *disp_t, 
                disp_mode_t mode
            )
{
    ESP_LOGI(TAG, "disp_set running");
    const DispTables *t = nullptr;


    switch (mode)
    {
        case DISP_DIG:
            t = &DigTable;
            break;
        
        case DISP_SYM:
            t = &SymTable;
            break;
    }
    gpio_set_level(disp_pins.latch, 0);
    for (int i = 3; i >= 0; i--) {
        const uint8_t *table = t->normal;

        if (i == 1) {
            table = t->inverse;
        }

        uint8_t idx = disp_t->digitarr[i];
        bool dp = disp_t->dp_t[i];

        shift_out(table[idx] | (dp ? DP_MASK : 0));
    }
    gpio_set_level(disp_pins.latch, 1);
    esp_rom_delay_us(5);
    gpio_set_level(disp_pins.latch, 0);

    // ESP_LOGI(TAG, "Dig %d", disp_t->digitarr[0]);
    // ESP_LOGI(TAG, "Dig %d", disp_t->digitarr[1]);
    // ESP_LOGI(TAG, "Dig %d", disp_t->digitarr[2]);
    // ESP_LOGI(TAG, "Dig %d", disp_t->digitarr[3]);
}

void reset_disp(void)
{
    for (int i = 0; i < 4; i++) {
        gpio_set_level(disp_pins.latch, 0);
        shift_out(disp_data.numbers[0]);
        gpio_set_level(disp_pins.latch, 1);
        esp_rom_delay_us(5);
        gpio_set_level(disp_pins.latch, 0);
    }
}