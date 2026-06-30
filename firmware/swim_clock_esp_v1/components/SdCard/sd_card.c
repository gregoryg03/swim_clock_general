#include "sd_card.h"

static const char *TAG = "sd_debug";

typedef struct {
    gpio_num_t mosi;
    gpio_num_t miso;
    gpio_num_t clock;
    gpio_num_t cs;
} sd_pins_t;

sd_pins_t sd_pins;

sdmmc_host_t host = SDSPI_HOST_DEFAULT();
sdmmc_card_t *card;
const char mount_point[] = MOUNT_POINT;

const char *path = MOUNT_POINT"/intervals.txt";

size_t position = 0;

void sd_init(gpio_num_t mosi, gpio_num_t miso, gpio_num_t clock, gpio_num_t cs)
{
    esp_err_t ret;

    position = 0;

    sd_pins.mosi = mosi;
    sd_pins.miso = miso;
    sd_pins.clock = clock;
    sd_pins.cs = cs;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
    };


    ESP_LOGI(TAG, "Initilizing SD Card");

    ESP_LOGI(TAG, "Using SPI Peripheral");

    

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
    
    char data[MAX_CHAR_SIZE];

    //check if file name already existis and delete if it does
    struct stat st;
    if (stat(path, &st) == 0) {
        unlink(path);
    }
    
    // snprintf(data, MAX_CHAR_SIZE, "%s %s!\n", "Hello", card->cid.name);
    // ret = write_file(file_intervals, data);
    // if (ret != ESP_OK) {
    //     return;
    // }
    
   

    // //Write
    // const char *file_nihao = MOUNT_POINT"/nihao.txt";
    // memset(data, 0, MAX_CHAR_SIZE);
    // snprintf(data, MAX_CHAR_SIZE, "%s %s!\n", "Nihao", card->cid.name);
    // ret = write_file(file_nihao, data);

    // if (ret != ESP_OK) {
    //     return;
    // }

    // //Open to read
    // ret = read_file(file_nihao);
    // if (ret != ESP_OK) {
    //     return;
    // }

    // esp_vfs_fat_sdcard_unmount(mount_point, card);
    // ESP_LOGI(TAG, "Card unmounted");

    // spi_bus_free(host.slot);
}

// esp_err_t sd_open(void)
// {

// }

esp_err_t sd_write_data(uint8_t *interval_in)
{
    ESP_LOGI(TAG, "Opening file %s for w", path);
    FILE *f = fopen(path, "ab");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open for write");
        return ESP_FAIL;
    }
    fwrite(interval_in, sizeof(uint8_t), 1, f);
    ESP_LOGI(TAG, "Write byte: %u", (unsigned int)*interval_in);
    position++;

    fclose(f);

    ESP_LOGI(TAG, "Wrote");
    return ESP_OK;
}



esp_err_t sd_read_data(uint8_t *interval_out)
{
    ESP_LOGI(TAG, "Reading file %s", path);
    FILE *f = fopen(path, "rb");
    if (f == NULL) {
        ESP_LOGE(TAG, "Failed to open SD for reading");
        *interval_out = 0;
        return ESP_FAIL;
    }

    if (position == 0) { //changed from 1 6/29
        *interval_out = 0;
        return ESP_OK;
    }

    if (fseek(f, --position, SEEK_SET) != 0) {
        ESP_LOGE(TAG, "Read Fail Seek");
        return ESP_FAIL;
    }

    fread(interval_out, sizeof(uint8_t), 1, f);
    
    fclose(f);
    ESP_LOGI(TAG, "Read");
    return ESP_OK;
}

void sd_shutdown(void)
{
    esp_vfs_fat_sdcard_unmount(mount_point, card);
    ESP_LOGI(TAG, "Card unmounted");

    spi_bus_free(host.slot);
}


// static esp_err_t write_file(const char *path, char *data)
// {
//     ESP_LOGI(TAG, "Opening file %s", path);
//     FILE *f = fopen(path, "w");
//     if (f == NULL) {
//         ESP_LOGE(TAG, "Failed to open file for writing");
//         return ESP_FAIL;
//     }
//     fprintf(f, data);
//     fclose(f);

//     ESP_LOGI(TAG, "File written");

//     return ESP_OK;
// }

// static esp_err_t read_file(const char *path)
// {
//     ESP_LOGI(TAG, "Reading file %s", path);
//     FILE *f = fopen(path, "r");
//     if (f == NULL) {
//         ESP_LOGE(TAG, "Failed to open file for reading");
//         return ESP_FAIL;
//     }
//     char line[MAX_CHAR_SIZE];
//     fgets(line, sizeof(line), f);
//     fclose(f);

//     //Remove new line
//     char *pos = strchr(line, '\n');
//     if (pos) {
//         *pos = '\0';
//     }

//     ESP_LOGI(TAG, "Read from file: '%s'", line);

//     return ESP_OK;
// }