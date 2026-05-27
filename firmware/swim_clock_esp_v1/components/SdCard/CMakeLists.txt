idf_component_register(
    SRCS "sd_card.cpp"
    INCLUDE_DIRS "."
    REQUIRES SdCard Display
    REQUIRES esp_driver_gpio
    REQUIRES esp_driver_sdspi
    REQUIRES fatfs
    REQUIRES sdmmc
    )
