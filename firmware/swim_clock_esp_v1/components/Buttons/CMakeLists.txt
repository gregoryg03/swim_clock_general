idf_component_register(
    SRCS "button_poll.c" "sd_card.c"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio
    )