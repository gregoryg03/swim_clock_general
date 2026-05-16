idf_component_register(
    SRCS "display.cpp"
    INCLUDE_DIRS "."
    REQUIRES esp_driver_gpio Events esp_timer Events
    )