#ifndef SEN0460_H
#define SEN0460_H

#include "esp_err.h"
#include "driver/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_MASTER_SCL_IO 0          // GPIO for I2C SCL
#define I2C_MASTER_SDA_IO 45         // GPIO for I2C SDA
#define I2C_MASTER_NUM    I2C_NUM_0  // I2C port number for master
#define I2C_MASTER_FREQ_HZ 100000    // I2C clock speed
#define SENSOR_ADDR 0x19             // Default I2C address for SEN0460


// PM sensor types
#define PARTICLE_PM1_0_STANDARD   0x05
#define PARTICLE_PM2_5_STANDARD   0x07
#define PARTICLE_PM10_STANDARD    0x09
#define PARTICLE_PM1_0_ATMOSPHERE 0x0B
#define PARTICLE_PM2_5_ATMOSPHERE 0x0D
#define PARTICLE_PM10_ATMOSPHERE  0x0F

#define PARTICLENUM_0_3_UM_EVERY0_1L_AIR 0x11
#define PARTICLENUM_0_5_UM_EVERY0_1L_AIR 0x13
#define PARTICLENUM_1_0_UM_EVERY0_1L_AIR 0x15
#define PARTICLENUM_2_5_UM_EVERY0_1L_AIR 0x17
#define PARTICLENUM_5_0_UM_EVERY0_1L_AIR 0x19
#define PARTICLENUM_10_UM_EVERY0_1L_AIR  0x1B

#define PARTICLENUM_GAIN_VERSION 0x1D

typedef struct {
    i2c_port_t i2c_port;
    uint8_t i2c_addr;
} sen0460_t;

// Function Prototypes
esp_err_t sen0460_init(sen0460_t *sensor, i2c_port_t port, uint8_t addr);
esp_err_t sen0460_get_particle_concentration(sen0460_t *sensor, uint8_t type, uint16_t *concentration);
esp_err_t sen0460_get_particle_count(sen0460_t *sensor, uint8_t type, uint16_t *count);
esp_err_t sen0460_get_version(sen0460_t *sensor, uint8_t *version);
esp_err_t sen0460_set_low_power(sen0460_t *sensor);
esp_err_t sen0460_awake(sen0460_t *sensor);

#ifdef __cplusplus
}
#endif

#endif // SEN0460_H
