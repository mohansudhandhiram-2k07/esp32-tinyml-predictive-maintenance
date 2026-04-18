idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES edge_impulse driver esp_timer nvs_flash esp_wifi esp_netif mqtt
    PRIV_REQUIRES driver
)