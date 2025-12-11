/**
 * @file TemperatureSensorModule.h
 * @purpose Temperature sensor management module for DS18B20 sensors
 * @dependencies ModuleInterface.h, HAL.h, EventBus.h
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes Reading all sensors: ~10-20ms per sensor with default resolution
 * @module_type MODULE
 *
 * This module manages all temperature sensors in the HVAC system. It:
 * - Discovers DS18B20 sensors on the OneWire bus
 * - Reads temperatures periodically
 * - Publishes temperature events when values change significantly
 * - Detects sensor failures and reports safety violations
 * - Provides temperature history for trending
 *
 * Sensor Naming:
 * Sensors are assigned names based on their location (e.g., "tank_top", "supply",
 * "return", "outdoor"). Names are mapped to addresses in configuration.
 */

#ifndef TEMPERATURE_SENSOR_MODULE_H
#define TEMPERATURE_SENSOR_MODULE_H

#include "ModuleInterface.h"
#include "HAL.h"
#include "EventBus.h"
#include <map>
#include <vector>

/**
 * Temperature sensor configuration
 */
struct TemperatureSensor {
    uint64_t address;           // OneWire address
    String name;                // Sensor name (e.g., "tank_top")
    float offset_c;             // Calibration offset in °C
    float min_valid_temp_c;     // Minimum valid temperature
    float max_valid_temp_c;     // Maximum valid temperature
    bool enabled;               // Is sensor enabled?
};

/**
 * Temperature reading
 */
struct TemperatureReading {
    uint64_t address;
    String name;
    float temperature_c;
    uint32_t timestamp_ms;
    bool valid;
};

/**
 * @class TemperatureSensorModule
 * @brief Manages temperature sensors and publishes temperature events
 *
 * This module provides centralized temperature sensor management. It handles
 * sensor discovery, reading, calibration, and fault detection.
 *
 * Configuration:
 * sensors:
 *   onewire_pin: 4
 *   read_interval_ms: 1000
 *   change_threshold_c: 0.5
 *   sensors:
 *     - address: 0x28FF123456789ABC
 *       name: tank_top
 *       offset_c: -0.5
 *       min_valid_temp_c: -20.0
 *       max_valid_temp_c: 100.0
 *
 * Events Published:
 * - TEMPERATURE_CHANGED: When temperature changes by > threshold
 * - TEMPERATURE_THRESHOLD_EXCEEDED: When temperature exceeds safety limits
 * - TEMPERATURE_SENSOR_ERROR: When sensor read fails
 *
 * Usage Example:
 * TemperatureSensorModule temp_module;
 * system.register_module(&temp_module, 10);  // High priority
 * system.initialize();
 *
 * // Get current temperature
 * float temp = temp_module.get_temperature("tank_top");
 */
class TemperatureSensorModule : public ModuleInterface {
public:
    TemperatureSensorModule();
    ~TemperatureSensorModule() override = default;

    // ModuleInterface implementation
    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override { return "1.0"; }

    /**
     * Update sensors (called periodically)
     * @note This should be called from a timer or main loop
     */
    void update();

    /**
     * Get temperature by sensor name
     * @param name Sensor name
     * @return Temperature in Celsius, or NaN if not found/invalid
     */
    float get_temperature(const String& name) const;

    /**
     * Get temperature by sensor address
     * @param address OneWire address
     * @return Temperature in Celsius, or NaN if not found/invalid
     */
    float get_temperature(uint64_t address) const;

    /**
     * Get all current temperatures
     * @return Map of sensor name to temperature
     */
    std::map<String, float> get_all_temperatures() const;

    /**
     * Get last reading for a sensor
     * @param name Sensor name
     * @return Last temperature reading
     */
    TemperatureReading get_reading(const String& name) const;

    /**
     * Scan for new sensors on the bus
     * @return Number of sensors found
     */
    int scan_sensors();

    /**
     * Add sensor configuration
     * @param sensor Sensor configuration
     * @return true if added successfully
     */
    bool add_sensor(const TemperatureSensor& sensor);

    /**
     * Remove sensor by name
     * @param name Sensor name
     */
    void remove_sensor(const String& name);

    /**
     * Get all configured sensors
     * @return Vector of sensor configurations
     */
    std::vector<TemperatureSensor> get_sensors() const;

    /**
     * Set read interval
     * @param interval_ms Interval in milliseconds
     */
    void set_read_interval(uint32_t interval_ms) {
        read_interval_ms_ = interval_ms;
    }

    /**
     * Set change threshold for event publishing
     * @param threshold_c Temperature change threshold in °C
     */
    void set_change_threshold(float threshold_c) {
        change_threshold_c_ = threshold_c;
    }

private:
    /**
     * Read all sensors
     */
    void read_all_sensors();

    /**
     * Process temperature reading
     */
    void process_reading(const TemperatureSensor& sensor, float raw_temp_c);

    /**
     * Check if temperature is valid
     */
    bool is_temperature_valid(const TemperatureSensor& sensor, float temp_c);

    /**
     * Publish temperature event
     */
    void publish_temperature_event(const TemperatureSensor& sensor,
                                   float temperature_c,
                                   float previous_temperature_c);

    // Configuration
    uint8_t onewire_pin_;
    uint32_t read_interval_ms_;
    float change_threshold_c_;

    // Sensor configurations
    std::map<uint64_t, TemperatureSensor> sensors_;  // Address -> Config
    std::map<String, uint64_t> name_to_address_;     // Name -> Address

    // Current readings
    std::map<uint64_t, TemperatureReading> readings_;

    // Timing
    uint32_t last_read_ms_;

    // Health tracking
    uint32_t successful_reads_;
    uint32_t failed_reads_;
    uint32_t total_reads_;
};

#endif // TEMPERATURE_SENSOR_MODULE_H
