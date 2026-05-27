#include "sd_card.h"

static const char *TAG = "sd_debug";

typedef struct {
    gpio_num_t mosi;
    gpio_num_t miso;
    gpio_num_t clock;
    gpio_num_t cs;
} sd_pins_t;

sd_pins_t sd_pins;

void sd_init(gpio_num_t mosi, gpio_num_t miso, gpio_num_t clock, gpio_num_t cs)
{
    esp_err_t ret;

    sd_pins.mosi = mosi;
    sd_pins.miso = miso;
    sd_pins.clock = clock;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
    };

    sdmmc_card_t *card;
    const char mount_point[] = MOUNT_POINT;
    ESP_LOGI(TAG, "Initilizing SD Card");

    ESP_LOGI(TAG, "Using SPI Peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();

    spi_bus_config_t bus_cfg = {
        .mosi_io_num = sd_pins.mosi,
        .miso_io_num = sd_pins.miso,
        .sclk_io_num = sd_pins.clock,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
        .max_transfer_sz = 4000,
    };

    ret = spi_bus_initialize(host.slot, &bus_cfg, SDSPI_DEFAULT_DMA);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initilize SPI bus.");
        return;
    }

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = sd_pins.cs;
    slot_config.host_id = host.slot;

    ESP_LOGI(TAG, "Mounting Filesystem");

    ret = esp_vfs_fat_sdspi_mount(mount_point, &host, &slot_config, &mount_config, &card);
    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE(TAG, "Failed to mount filesystem");
        }
        else {
            ESP_LOGE(TAG, "Failed to initilize the card (%s).", esp_err_to_name(ret));
        }
        return;
    }

    ESP_LOGI(TAG, "Filesystem mounted");

}