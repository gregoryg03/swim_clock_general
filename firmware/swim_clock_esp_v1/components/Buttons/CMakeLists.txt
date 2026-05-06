idf_component_register(
    SRCS "button_poll.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio
    REQUIRES SdCard Events
    )