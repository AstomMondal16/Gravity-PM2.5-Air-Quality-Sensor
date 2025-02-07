#include "sen0460.h"

esp_err_t sen0460_init(sen0460_t *sensor, i2c_port_t port, uint8_t addr) {
    if (!sensor) {
        return ESP_ERR_INVALID_ARG;
    }
    sensor->i2c_port = port;
    sensor->i2c_addr = addr;
    return ESP_OK;
}

static esp_err_t sen0460_write_register(sen0460_t *sensor, uint8_t reg, uint8_t *data, size_t len) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (sensor->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_write(cmd, data, len, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(sensor->i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

static esp_err_t sen0460_read_register(sen0460_t *sensor, uint8_t reg, uint8_t *data, size_t len) {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (sensor->i2c_addr << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, reg, true);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (sensor->i2c_addr << 1) | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, len, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(sensor->i2c_port, cmd, 1000 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

esp_err_t sen0460_get_particle_concentration(sen0460_t *sensor, uint8_t type, uint16_t *concentration) {
    if (!sensor || !concentration) {
        return ESP_ERR_INVALID_ARG;
    }
    uint8_t data[2];
    esp_err_t ret = sen0460_read_register(sensor, type, data, 2);
    if (ret != ESP_OK) {
        return ret;
    }
    *concentration = ((uint16_t)data[0] << 8) | data[1];
    return ESP_OK;
}

esp_err_t sen0460_get_particle_count(sen0460_t *sensor, uint8_t type, uint16_t *count) {
    if (!sensor || !count) {
        return ESP_ERR_INVALID_ARG;
    }
    uint8_t data[2];
    esp_err_t ret = sen0460_read_register(sensor, type, data, 2);
    if (ret != ESP_OK) {
        return ret;
    }
    *count = ((uint16_t)data[0] << 8) | data[1];
    return ESP_OK;
}

esp_err_t sen0460_get_version(sen0460_t *sensor, uint8_t *version) {
    if (!sensor || !version) {
        return ESP_ERR_INVALID_ARG;
    }
    return sen0460_read_register(sensor, PARTICLENUM_GAIN_VERSION, version, 1);
}

esp_err_t sen0460_set_low_power(sen0460_t *sensor) {
    uint8_t mode = 1;
    return sen0460_write_register(sensor, 0x01, &mode, 1);
}

esp_err_t sen0460_awake(sen0460_t *sensor) {
    uint8_t mode = 2;
    return sen0460_write_register(sensor, 0x01, &mode, 1);
}
