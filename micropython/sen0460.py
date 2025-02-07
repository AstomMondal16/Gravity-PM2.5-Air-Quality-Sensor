import machine
import time

class SEN0460:
    # Define constants for selecting different particle measurement types
    PARTICLE_PM1_0_STANDARD   = 0x05
    PARTICLE_PM2_5_STANDARD   = 0x07
    PARTICLE_PM10_STANDARD    = 0x09
    PARTICLE_PM1_0_ATMOSPHERE = 0x0B
    PARTICLE_PM2_5_ATMOSPHERE = 0x0D
    PARTICLE_PM10_ATMOSPHERE  = 0x0F
    PARTICLENUM_0_3_UM_EVERY0_1L_AIR = 0x11
    PARTICLENUM_0_5_UM_EVERY0_1L_AIR = 0x13
    PARTICLENUM_1_0_UM_EVERY0_1L_AIR = 0x15
    PARTICLENUM_2_5_UM_EVERY0_1L_AIR = 0x17
    PARTICLENUM_5_0_UM_EVERY0_1L_AIR = 0x19
    PARTICLENUM_10_UM_EVERY0_1L_AIR  = 0x1B
    PARTICLENUM_GAIN_VERSION = 0x1D

    def __init__(self, i2c, addr):
        """Initialize the sensor with the I2C interface and address."""
        self.i2c = i2c
        self.addr = addr

    def gain_particle_concentration_ugm3(self, PMtype):
        """Get the particle concentration in µg/m³ for a specified PM type."""
        buf = self.read_reg(PMtype, 2)
        if buf and len(buf) == 2:  # Ensure we received two bytes
            return (buf[0] << 8) | buf[1]
        return None  # Return None if read fails

    def gain_particlenum_every0_1l(self, PMtype):
        """Get the particle count per 0.1L of air for a specified PM type."""
        buf = self.read_reg(PMtype, 2)
        if buf and len(buf) == 2:
            return (buf[0] << 8) | buf[1]
        return None

    def gain_version(self):
        """Get the sensor's firmware version."""
        version = self.read_reg(self.PARTICLENUM_GAIN_VERSION, 1)
        if version and len(version) == 1:
            return version[0]
        return None  # Return None on failure

    def set_lowpower(self):
        """Set the sensor to low power mode."""
        self.write_reg(0x01, bytearray([0x01]))

    def awake(self):
        """Wake up the sensor from low power mode."""
        self.write_reg(0x01, bytearray([0x02]))

    def write_reg(self, reg, data):
        """Write data to a specific register on the sensor."""
        try:
            self.i2c.writeto(self.addr, bytearray([reg]) + data)
        except OSError:
            print("Please check connection!")
            time.sleep(1)

    def read_reg(self, reg, length):
        """Read a specified number of bytes from a register."""
        try:
            self.i2c.writeto(self.addr, bytearray([reg]))  # Request register data
            return self.i2c.readfrom(self.addr, length)   # Read data
        except OSError:
            print("I2C Read Failed!")
            return None  # Return None instead of -1

# change the pins no.'s as per the connection of the sensor
i2c = machine.SoftI2C(scl=machine.Pin(45), sda=machine.Pin(0))

# Create an instance of the air quality sensor
sensor = SEN0460(i2c, 0x19)

def main():
    while True:
        sensor.awake()
        time.sleep(1)  # Give time for the sensor to wake up

        # Get firmware version
        version = sensor.gain_version()
        if version is not None:
            print("Sensor version:", version)
        else:
            print("Failed to get sensor version!")

        # Read PM concentrations
        pm1_0_std = sensor.gain_particle_concentration_ugm3(SEN0460.PARTICLE_PM1_0_STANDARD)
        pm2_5_std = sensor.gain_particle_concentration_ugm3(SEN0460.PARTICLE_PM2_5_STANDARD)
        pm10_std = sensor.gain_particle_concentration_ugm3(SEN0460.PARTICLE_PM10_STANDARD)

        print(f"PM1.0 (Standard): {pm1_0_std if pm1_0_std is not None else 'Error'} µg/m³")
        print(f"PM2.5 (Standard): {pm2_5_std if pm2_5_std is not None else 'Error'} µg/m³")
        print(f"PM10 (Standard): {pm10_std if pm10_std is not None else 'Error'} µg/m³")

        # Read particle counts
        num_0_3_um = sensor.gain_particlenum_every0_1l(SEN0460.PARTICLENUM_0_3_UM_EVERY0_1L_AIR)
        num_0_5_um = sensor.gain_particlenum_every0_1l(SEN0460.PARTICLENUM_0_5_UM_EVERY0_1L_AIR)

        print(f"Particles 0.3µm: {num_0_3_um if num_0_3_um is not None else 'Error'} per 0.1L air")
        print(f"Particles 0.5µm: {num_0_5_um if num_0_5_um is not None else 'Error'} per 0.1L air")

        # Set sensor to low power mode
        sensor.set_lowpower()
        print("----------------------------------------------------\n")
        time.sleep(3)

if __name__ == "__main__":
    main()



