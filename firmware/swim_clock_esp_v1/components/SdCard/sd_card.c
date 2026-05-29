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
    sd_pins.cs = cs;

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
    //Using POSTIX and C standard library to work with the files

    //Create the file
    const char *file_hello = MOUNT_POINT"/hello.txt";
    char data[MAX_CHAR_SIZE];
    snprintf(data, MAX_CHAR_SIZE, "%s %s!\n", "Hello", card->cid.name);
    ret = write_file(file_hello, data);
    if (ret != ESP_OK) {
        return;
    }
    
    const char *file_foo = MOUNT_POINT"/foo.txt";

    //check if file name already existis and delete if it does
    struct stat st;
    if (stat(file_foo, &st) == 0) {
        unlink(file_foo);
    }

    //rename file
    ESP_LOGI(TAG, "Remaming file %s to %s", file_hello, file_foo);
    if (rename(file_hello, file_foo) != 0) {
        ESP_LOGE(TAG, "Rename Failed");
        return;
    }

    //Write
    const char *file_nihao = MOUNT_POINT"/nihao.txt";
    memset(data, 0, MAX_CHAR_SIZE);
    snprintf(data, MAX_CHAR_SIZE, "%s %s!\n", "Nihao", card->cid.name);
    ret = write_file(file_nihao, data);

    if (ret != ESP_OK) {
        return;
    }

    //Open to read
    ret = read_file(file_nihao);
    if (ret != ESP_OK) {
        return;
    }

    esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(TAG, "Card unmounted");

    spi_bus_free(host.slot);
}


static esp_err_t write_file(const char *path, char *data)
{
    ESP_LOGI(TAG, "Opening file %s", path);
    FILE *f = fopen(path, "w");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for writing");
        return ESP_FAIL;
    }
    fprintf(f, data);
    fclose(f);

    ESP_LOGI(TAG, "File written");

    return ESP_OK;
}

static esp_err_t read_file(const char *path)
{
    ESP_LOGI(TAG, "Reading file %s", path);
    FILE *f = fopen(path, "r");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open file for reading");
        return ESP_FAIL;
    }
    char line[MAX_CHAR_SIZE];
    fgets(line, sizeof(line), f);
    fclose(f);

    //Remove new line
    char *pos = strchr(line, '\n');
    if (pos) {
        *pos = '\0';
    }

    ESP_LOGI(TAG, "Read from file: '%s'", line);

    return ESP_OK;
}