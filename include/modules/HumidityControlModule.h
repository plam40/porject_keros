/**
 * @file HumidityControlModule.h
 * @purpose Humidity control with dehumidification, humidification, and mold prevention
 * @dependencies ModuleInterface.h, HAL.h, EventBus.h
 * @version 1.0.0
 * @performance_notes Humidity sensor read: ~10-20ms, Control update: <1ms
 *
 * This module manages humidity control for HVAC zones including:
 * - Humidity monitoring via I2C sensors (BME280, SHT31, DHT22)
 * - Dehumidifier control with hysteresis
 * - Optional humidifier control
 * - Multi-zone support
 * - Mold prevention alerts
 * - Condensation risk detection
 * - Dewpoint calculation
 *
 * Humidity Control Strategy:
 * - Dehumidification when RH > target + hysteresis
 * - Humidification when RH < target - hysteresis
 * - Mold risk alert when RH > 65% for extended period
 * - Condensation risk when dewpoint approaches surface temperature
 */

#ifndef HUMIDITY_CONTROL_MODULE_H
#define HUMIDITY_CONTROL_MODULE_H

#include "ModuleInterface.h"
#include "HAL.h"
#include "EventBus.h"
#include <map>
#include <vector>

/**
 * Humidity sensor types
 */
enum class HumiditySensorType {
    BME280,         // I2C temperature, humidity, pressure sensor
    SHT31,          // I2C temperature and humidity sensor
    DHT22,          // Digital temperature and humidity sensor
    INTERNAL        // Use temperature module + estimation
};

/**
 * Humidity control mode
 */
enum class HumidityMode {
    OFF,            // No humidity control
    DEHUMIDIFY_ONLY,// Only dehumidification
    HUMIDIFY_ONLY,  // Only humidification
    AUTO            // Automatic control (both)
};

/**
 * Dehumidifier/Humidifier state
 */
enum class HumidityDeviceState {
    OFF,
    RUNNING,
    COOLDOWN,       // Cooldown period after running
    ERROR
};

/**
 * Humidity sensor configuration
 */
struct HumiditySensorConfig {
    String name;                    // Sensor name (e.g., "living_room")
    HumiditySensorType type;
    uint8_t i2c_address;            // I2C address (if applicable)
    uint8_t data_pin;               // Data pin for DHT22 (if applicable)
    float humidity_offset;          // Calibration offset (%)
    float temperature_offset_c;     // Temperature calibration offset
    bool enabled;
};

/**
 * Humidity zone configuration
 */
struct HumidityZoneConfig {
    String zone_name;               // Zone name
    String sensor_name;             // Associated humidity sensor
    uint8_t dehumidifier_pin;       // Dehumidifier relay/control pin (0 = none)
    uint8_t humidifier_pin;         // Humidifier relay/control pin (0 = none)

    // Control parameters
    float target_humidity_percent;  // Target relative humidity (%)
    float hysteresis_percent;       // Hysteresis (e.g., 5%)
    uint32_t min_run_time_ms;       // Minimum run time (prevent cycling)
    uint32_t min_off_time_ms;       // Minimum off time

    // Safety thresholds
    float max_humidity_percent;     // Maximum allowed humidity
    float min_humidity_percent;     // Minimum allowed humidity
    float mold_risk_humidity;       // Mold risk threshold (typically 65%)
    uint32_t mold_risk_duration_ms; // Duration before mold alert

    bool enabled;
};

/**
 * Humidity reading
 */
struct HumidityReading {
    String sensor_name;
    float relative_humidity_percent;
    float temperature_c;
    float dewpoint_c;
    uint32_t timestamp_ms;
    bool valid;
};

/**
 * Zone humidity status
 */
struct HumidityZoneStatus {
    String zone_name;
    HumidityMode mode;
    HumidityDeviceState dehumidifier_state;
    HumidityDeviceState humidifier_state;
    float current_humidity_percent;
    float target_humidity_percent;
    float dewpoint_c;
    bool mold_risk;
    bool condensation_risk;
    uint32_t dehumidifier_runtime_hours;
    uint32_t humidifier_runtime_hours;
    bool has_error;
    String error_message;
};

/**
 * @class HumidityControlModule
 * @brief Manages humidity control with dehumidification and mold prevention
 *
 * This module provides comprehensive humidity control across multiple zones.
 * It monitors humidity levels, controls dehumidifiers/humidifiers, and prevents
 * mold growth and condensation issues.
 *
 * Configuration Example:
 * humidity:
 *   zones:
 *     living_room:
 *       sensor: "living_room_sensor"
 *       dehumidifier_pin: 33
 *       target_humidity_percent: 50.0
 *       hysteresis_percent: 5.0
 *       mold_risk_humidity: 65.0
 *
 * Usage Example:
 * HumidityControlModule humidity_module;
 * system.register_module(&humidity_module, 40);
 *
 * // Add humidity sensor
 * HumiditySensorConfig sensor;
 * sensor.name = "living_room_sensor";
 * sensor.type = HumiditySensorType::SHT31;
 * sensor.i2c_address = 0x44;
 * humidity_module.add_sensor(sensor);
 *
 * // Add zone
 * HumidityZoneConfig zone;
 * zone.zone_name = "living_room";
 * zone.sensor_name = "living_room_sensor";
 * zone.dehumidifier_pin = 33;
 * zone.target_humidity_percent = 50.0;
 * humidity_module.add_zone(zone);
 */
class HumidityControlModule : public ModuleInterface {
public:
    HumidityControlModule();
    ~HumidityControlModule() override = default;

    // ModuleInterface implementation
    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override { return "1.0"; }

    /**
     * Update humidity control (called periodically)
     */
    void update();

    /**
     * Add humidity sensor
     */
    bool add_sensor(const HumiditySensorConfig& config);

    /**
     * Add humidity zone
     */
    bool add_zone(const HumidityZoneConfig& config);

    /**
     * Set zone mode
     */
    bool set_zone_mode(const String& zone_name, HumidityMode mode);

    /**
     * Set zone target humidity
     */
    bool set_zone_target(const String& zone_name, float target_percent);

    /**
     * Get humidity reading for a sensor
     */
    HumidityReading get_sensor_reading(const String& sensor_name) const;

    /**
     * Get zone status
     */
    HumidityZoneStatus get_zone_status(const String& zone_name) const;

    /**
     * Get all zone statuses
     */
    std::vector<HumidityZoneStatus> get_all_zones_status() const;

    /**
     * Enable/disable a zone
     */
    void set_zone_enabled(const String& zone_name, bool enabled);

private:
    /**
     * Sensor data
     */
    struct Sensor {
        HumiditySensorConfig config;
        float humidity_percent;
        float temperature_c;
        float dewpoint_c;
        uint32_t last_read_ms;
        bool valid;
    };

    /**
     * Zone data
     */
    struct Zone {
        HumidityZoneConfig config;
        HumidityMode mode;
        HumidityDeviceState dehumidifier_state;
        HumidityDeviceState humidifier_state;
        uint32_t device_start_time_ms;
        uint32_t device_stop_time_ms;
        uint32_t dehumidifier_runtime_ms;
        uint32_t humidifier_runtime_ms;
        uint32_t high_humidity_start_ms;
        bool mold_risk_active;
        bool has_error;
        String error_message;
    };

    /**
     * Read all humidity sensors
     */
    void read_sensors();

    /**
     * Read specific sensor type
     */
    bool read_bme280(Sensor& sensor);
    bool read_sht31(Sensor& sensor);
    bool read_dht22(Sensor& sensor);

    /**
     * Calculate dewpoint
     * @param temp_c Temperature in Celsius
     * @param rh_percent Relative humidity in percent
     * @return Dewpoint in Celsius
     */
    float calculate_dewpoint(float temp_c, float rh_percent);

    /**
     * Update zone control
     */
    void update_zone(Zone& zone);

    /**
     * Check mold risk
     */
    void check_mold_risk(Zone& zone, float current_humidity);

    /**
     * Control dehumidifier
     */
    void control_dehumidifier(Zone& zone, float current_humidity);

    /**
     * Control humidifier
     */
    void control_humidifier(Zone& zone, float current_humidity);

    /**
     * Set device output
     */
    void set_device_output(uint8_t pin, bool on);

    /**
     * Publish humidity event
     */
    void publish_humidity_event(const String& sensor_name, float humidity, float previous);

    // Sensors and zones
    std::map<String, Sensor> sensors_;
    std::map<String, Zone> zones_;

    // Timing
    uint32_t last_sensor_read_ms_;
    uint32_t sensor_read_interval_ms_;
};

#endif // HUMIDITY_CONTROL_MODULE_H
