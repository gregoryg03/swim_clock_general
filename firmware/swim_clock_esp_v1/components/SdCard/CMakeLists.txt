idf_component_register(
    SRCS "sd_card.c"
    INCLUDE_DIRS "."
    REQUIRES Display
    REQUIRES esp_driver_gpio
    REQUIRES esp_driver_sdspi
    REQUIRES fatfs
    REQUIRES sdmmc
    )
