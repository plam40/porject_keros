/**
 * @file HAL.h
 * @purpose Hardware Abstraction Layer for HVAC Controller
 * @dependencies Arduino.h, ESP32 peripheral libraries
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes GPIO operations: <1μs, ADC: ~10-20μs, PWM setup: ~50μs
 * @module_type SKELETON
 *
 * The HAL provides a clean, consistent interface to all hardware peripherals.
 * This abstraction allows for easier testing, portability, and maintainability.
 *
 * Supported peripherals:
 * - Digital GPIO (input/output with pull-up/pull-down)
 * - Analog inputs (ADC with averaging and calibration)
 * - PWM outputs (motor control, heating elements)
 * - OneWire bus (DS18B20 temperature sensors)
 * - I2C bus (various sensors and peripherals)
 * - SPI bus (future expansion)
 * - UART (Modbus, debugging)
 */

#ifndef HAL_H
#define HAL_H

#include <Arduino.h>
#include <vector>
#include <map>

/**
 * GPIO pin modes
 */
enum class PinMode {
    INPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN,
    OUTPUT,
    OUTPUT_OPEN_DRAIN
};

/**
 * Analog reference voltage (ESP32 specific)
 */
enum class AnalogReference {
    DEFAULT_3V3,        // 3.3V reference (default)
    ATTEN_0DB,          // 0dB attenuation, 0-1.1V range
    ATTEN_2_5DB,        // 2.5dB attenuation, 0-1.5V range
    ATTEN_6DB,          // 6dB attenuation, 0-2.2V range
    ATTEN_11DB          // 11dB attenuation, 0-3.3V range
};

/**
 * @class HAL
 * @brief Hardware Abstraction Layer providing uniform access to peripherals
 *
 * The HAL is implemented as a singleton to ensure consistent hardware state
 * management across the entire system. It provides both low-level access
 * (for performance-critical code) and high-level convenience functions.
 *
 * Pin Configuration Best Practices:
 * - Configure all pins during initialization
 * - Use INPUT_PULLUP for switches/buttons
 * - Use OUTPUT for relays, LEDs
 * - Reserved pins (0, 1, 2, 3, 6-11) should be avoided
 *
 * Usage Example:
 * HAL& hal = HAL::get_instance();
 * hal.pin_mode(LED_PIN, PinMode::OUTPUT);
 * hal.digital_write(LED_PIN, true);
 *
 * // Read temperature sensor
 * float temp_c = hal.read_ds18b20(TEMP_SENSOR_ADDR);
 *
 * // Control pump with PWM
 * hal.pwm_write(PUMP_PIN, 75);  // 75% duty cycle
 */
class HAL {
public:
    /**
     * Get singleton instance
     */
    static HAL& get_instance() {
        static HAL instance;
        return instance;
    }

    /**
     * Initialize HAL subsystem
     * @return true if initialization successful
     */
    bool initialize();

    // ========== GPIO Functions ==========

    /**
     * Set pin mode
     * @param pin GPIO pin number
     * @param mode Pin mode (input/output/pullup/etc)
     */
    void pin_mode(uint8_t pin, PinMode mode);

    /**
     * Write digital value to pin
     * @param pin GPIO pin number
     * @param value true = HIGH, false = LOW
     */
    void digital_write(uint8_t pin, bool value);

    /**
     * Read digital value from pin
     * @param pin GPIO pin number
     * @return true = HIGH, false = LOW
     */
    bool digital_read(uint8_t pin);

    /**
     * Toggle pin state
     * @param pin GPIO pin number
     */
    void digital_toggle(uint8_t pin);

    // ========== Analog Functions ==========

    /**
     * Read analog value from ADC
     * @param pin ADC pin number (32-39 on ESP32)
     * @param samples Number of samples to average (default 10)
     * @return Raw ADC value (0-4095 for 12-bit ADC)
     */
    uint16_t analog_read(uint8_t pin, uint8_t samples = 10);

    /**
     * Read analog voltage
     * @param pin ADC pin number
     * @param samples Number of samples to average
     * @return Voltage in volts (0.0 - 3.3V)
     */
    float analog_read_voltage(uint8_t pin, uint8_t samples = 10);

    /**
     * Set ADC attenuation for a pin
     * @param pin ADC pin number
     * @param ref Analog reference/attenuation
     */
    void analog_set_attenuation(uint8_t pin, AnalogReference ref);

    // ========== PWM Functions ==========

    /**
     * Initialize PWM on a pin
     * @param pin GPIO pin number
     * @param channel PWM channel (0-15 on ESP32)
     * @param frequency PWM frequency in Hz (default 5kHz)
     * @param resolution PWM resolution in bits (default 8-bit = 0-255)
     * @return true if successful
     */
    bool pwm_init(uint8_t pin, uint8_t channel, uint32_t frequency = 5000, uint8_t resolution = 8);

    /**
     * Write PWM duty cycle
     * @param channel PWM channel
     * @param duty_percent Duty cycle percentage (0-100)
     */
    void pwm_write(uint8_t channel, uint8_t duty_percent);

    /**
     * Write raw PWM duty cycle value
     * @param channel PWM channel
     * @param duty_value Raw duty value (0 to 2^resolution - 1)
     */
    void pwm_write_raw(uint8_t channel, uint32_t duty_value);

    /**
     * Set PWM frequency
     * @param channel PWM channel
     * @param frequency Frequency in Hz
     */
    void pwm_set_frequency(uint8_t channel, uint32_t frequency);

    // ========== OneWire / DS18B20 Functions ==========

    /**
     * Initialize OneWire bus
     * @param pin OneWire data pin
     * @return true if successful
     */
    bool onewire_init(uint8_t pin);

    /**
     * Scan OneWire bus for devices
     * @return Vector of 64-bit device addresses
     */
    std::vector<uint64_t> onewire_scan();

    /**
     * Read temperature from DS18B20 sensor
     * @param address 64-bit device address
     * @return Temperature in Celsius, or -127.0 on error
     */
    float read_ds18b20(uint64_t address);

    /**
     * Read all DS18B20 sensors on bus
     * @return Map of address to temperature
     */
    std::map<uint64_t, float> read_all_ds18b20();

    // ========== I2C Functions ==========

    /**
     * Initialize I2C bus
     * @param sda_pin SDA pin number
     * @param scl_pin SCL pin number
     * @param frequency I2C frequency in Hz (default 100kHz)
     * @return true if successful
     */
    bool i2c_init(uint8_t sda_pin, uint8_t scl_pin, uint32_t frequency = 100000);

    /**
     * Scan I2C bus for devices
     * @return Vector of I2C device addresses found
     */
    std::vector<uint8_t> i2c_scan();

    /**
     * Write byte to I2C device
     * @param address I2C device address
     * @param reg Register address
     * @param value Value to write
     * @return true if successful
     */
    bool i2c_write_byte(uint8_t address, uint8_t reg, uint8_t value);

    /**
     * Read byte from I2C device
     * @param address I2C device address
     * @param reg Register address
     * @param value Pointer to store read value
     * @return true if successful
     */
    bool i2c_read_byte(uint8_t address, uint8_t reg, uint8_t* value);

    /**
     * Read multiple bytes from I2C device
     * @param address I2C device address
     * @param reg Register address
     * @param buffer Buffer to store data
     * @param length Number of bytes to read
     * @return true if successful
     */
    bool i2c_read_bytes(uint8_t address, uint8_t reg, uint8_t* buffer, uint8_t length);

    // ========== Utility Functions ==========

    /**
     * Get ESP32 chip temperature (internal sensor)
     * @return Temperature in Celsius
     */
    float get_chip_temperature();

    /**
     * Get Hall sensor reading (magnetic field)
     * @return Hall sensor value
     */
    int get_hall_sensor();

    /**
     * Check if pin is valid for given operation
     * @param pin Pin number
     * @param mode Intended pin mode
     * @return true if pin can be used in this mode
     */
    bool is_pin_valid(uint8_t pin, PinMode mode);

    // Prevent copying
    HAL(const HAL&) = delete;
    HAL& operator=(const HAL&) = delete;

private:
    HAL() : initialized_(false), onewire_pin_(255) {}

    bool initialized_;

    // PWM channel tracking
    struct PWMChannel {
        uint8_t pin;
        uint8_t resolution;
        uint32_t frequency;
        bool in_use;
    };
    PWMChannel pwm_channels_[16];  // ESP32 has 16 PWM channels

    // OneWire configuration
    uint8_t onewire_pin_;

    // I2C configuration
    bool i2c_initialized_ = false;

    // Pin state tracking
    std::map<uint8_t, bool> pin_states_;
};

#endif // HAL_H
