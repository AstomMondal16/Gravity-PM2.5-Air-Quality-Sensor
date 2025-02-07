#include "sen0460.h"
#include "driver/i2c.h"
#include "esp_log.h"


static const char *TAG = "MAIN";

// Initialize I2C for communication with the sensor
static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    // Initialize I2C controller
    esp_err_t err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) {
        return err;
    }

    err = i2c_driver_install(I2C_MASTER_NUM, I2C_MODE_MASTER, 0, 0, 0);
    return err;
}

// Application main function
void app_main(void)
{
    // Initialize I2C communication
    esp_err_t ret = i2c_master_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C");
        return;
    }

    // Initialize the sensor object
    sen0460_t sensor;
    ret = sen0460_init(&sensor, I2C_MASTER_NUM, SENSOR_ADDR);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SEN0460 sensor");
        return;
    }

    ESP_LOGI(TAG, "SEN0460 initialized successfully");

    // Variables to store air quality data
    uint16_t pm1_concentration;
    uint16_t pm2_5_concentration;
    uint16_t pm10_concentration;

    // Main loop to read sensor data every 1 second
    while (1) {
        // Read PM1.0 concentration (in µg/m³)
        ret = sen0460_get_particle_concentration(&sensor, PARTICLE_PM1_0_STANDARD, &pm1_concentration);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "PM1.0 Concentration: %u µg/m³", pm1_concentration);
        } else {
            ESP_LOGE(TAG, "Failed to read PM1.0 concentration");
        }

        // Read PM2.5 concentration (in µg/m³)
        ret = sen0460_get_particle_concentration(&sensor, PARTICLE_PM2_5_STANDARD, &pm2_5_concentration);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "PM2.5 Concentration: %u µg/m³", pm2_5_concentration);
        } else {
            ESP_LOGE(TAG, "Failed to read PM2.5 concentration");
        }

        // Read PM10 concentration (in µg/m³)
        ret = sen0460_get_particle_concentration(&sensor, PARTICLE_PM10_STANDARD, &pm10_concentration);
        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "PM10 Concentration: %u µg/m³", pm10_concentration);
        } else {
            ESP_LOGI(TAG, "Failed to read PM10 concentration");
        }

        ESP_LOGI(TAG, ".......................................");
        // Delay for 1 second before reading again
        vTaskDelay(pdMS_TO_TICKS(5000));

    }
}
