/**
 * @file HumidityControlModule.cpp
 * @purpose Implementation of humidity control module
 * @dependencies HumidityControlModule.h, HAL.h
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes I2C sensor read: ~10-20ms, Control logic: <1ms
 * @module_type MODULE
 */

#include "modules/HumidityControlModule.h"
#include "ConfigManager.h"
#include "SafetyEngine.h"
#include <math.h>

HumidityControlModule::HumidityControlModule()
    : last_sensor_read_ms_(0),
      sensor_read_interval_ms_(5000)  // Read every 5 seconds
{
    state_ = ModuleState::UNINITIALIZED;
}

ModuleMetadata HumidityControlModule::get_metadata() const {
    ModuleMetadata meta;
    meta.name = "HumidityControlModule";
    meta.version = "1.0.0";
    meta.description = "Humidity control with dehumidification and mold prevention";
    meta.ram_usage_kb = 12;
    meta.flash_usage_kb = 30;
    meta.capabilities["sensors"] = "BME280,SHT31,DHT22";
    meta.capabilities["control"] = "dehumidifier,humidifier";
    meta.capabilities["mold_prevention"] = "yes";
    return meta;
}

HealthStatus HumidityControlModule::health_check() {
    HealthStatus health;
    health.last_update_ms = millis();

    int total_sensors = sensors_.size();
    int healthy_sensors = 0;
    int total_zones = zones_.size();
    int healthy_zones = 0;

    // Check sensors
    for (const auto& pair : sensors_) {
        if (pair.second.valid) {
            healthy_sensors++;
        }
    }

    // Check zones
    for (const auto& pair : zones_) {
        if (!pair.second.has_error) {
            healthy_zones++;
        }
    }

    // Calculate health percentage
    int total_components = total_sensors + total_zones;
    int healthy_components = healthy_sensors + healthy_zones;

    if (total_components > 0) {
        health.health_percentage = (healthy_components * 100) / total_components;
        health.is_healthy = (health.health_percentage >= 70);
    } else {
        health.health_percentage = 100;
        health.is_healthy = true;
    }

    if (health.is_healthy) {
        health.status_message = "Operating normally";
    } else {
        health.status_message = "Some sensors or zones have errors";
    }

    health.metrics["total_sensors"] = total_sensors;
    health.metrics["healthy_sensors"] = healthy_sensors;
    health.metrics["total_zones"] = total_zones;
    health.metrics["healthy_zones"] = healthy_zones;

    return health;
}

bool HumidityControlModule::initialize() {
    Serial.println("[Humidity] Initializing...");
    state_ = ModuleState::INITIALIZING;

    // I2C should already be initialized by HAL
    // Individual sensors will be initialized when added

    state_ = ModuleState::RUNNING;
    Serial.println("[Humidity] Initialized");
    return true;
}

void HumidityControlModule::shutdown() {
    Serial.println("[Humidity] Shutting down...");

    // Turn off all dehumidifiers and humidifiers
    for (auto& pair : zones_) {
        Zone& zone = pair.second;
        if (zone.config.dehumidifier_pin > 0) {
            set_device_output(zone.config.dehumidifier_pin, false);
        }
        if (zone.config.humidifier_pin > 0) {
            set_device_output(zone.config.humidifier_pin, false);
        }
    }

    state_ = ModuleState::SHUTDOWN;
    Serial.println("[Humidity] Shutdown complete");
}

void HumidityControlModule::update() {
    if (state_ != ModuleState::RUNNING) {
        return;
    }

    uint32_t now = millis();

    // Read sensors periodically
    if (now - last_sensor_read_ms_ >= sensor_read_interval_ms_) {
        last_sensor_read_ms_ = now;
        read_sensors();
    }

    // Update all zones
    for (auto& pair : zones_) {
        update_zone(pair.second);
    }
}

bool HumidityControlModule::add_sensor(const HumiditySensorConfig& config) {
    if (sensors_.find(config.name) != sensors_.end()) {
        Serial.printf("[Humidity] WARNING: Sensor %s already exists\n",
                     config.name.c_str());
        return false;
    }

    Sensor sensor;
    sensor.config = config;
    sensor.humidity_percent = 0.0;
    sensor.temperature_c = 0.0;
    sensor.dewpoint_c = 0.0;
    sensor.last_read_ms = 0;
    sensor.valid = false;

    sensors_[config.name] = sensor;

    Serial.printf("[Humidity] Added sensor: %s (type=%d)\n",
                 config.name.c_str(), static_cast<int>(config.type));

    return true;
}

bool HumidityControlModule::add_zone(const HumidityZoneConfig& config) {
    if (zones_.find(config.zone_name) != zones_.end()) {
        Serial.printf("[Humidity] WARNING: Zone %s already exists\n",
                     config.zone_name.c_str());
        return false;
    }

    // Check if sensor exists
    if (sensors_.find(config.sensor_name) == sensors_.end()) {
        Serial.printf("[Humidity] ERROR: Sensor %s not found for zone %s\n",
                     config.sensor_name.c_str(), config.zone_name.c_str());
        return false;
    }

    Zone zone;
    zone.config = config;
    zone.mode = HumidityMode::AUTO;
    zone.dehumidifier_state = HumidityDeviceState::OFF;
    zone.humidifier_state = HumidityDeviceState::OFF;
    zone.device_start_time_ms = 0;
    zone.device_stop_time_ms = millis();
    zone.dehumidifier_runtime_ms = 0;
    zone.humidifier_runtime_ms = 0;
    zone.high_humidity_start_ms = 0;
    zone.mold_risk_active = false;
    zone.has_error = false;

    // Configure GPIO pins
    HAL& hal = HAL::get_instance();
    if (config.dehumidifier_pin > 0) {
        hal.pin_mode(config.dehumidifier_pin, PinMode::OUTPUT);
        hal.digital_write(config.dehumidifier_pin, false);
    }
    if (config.humidifier_pin > 0) {
        hal.pin_mode(config.humidifier_pin, PinMode::OUTPUT);
        hal.digital_write(config.humidifier_pin, false);
    }

    zones_[config.zone_name] = zone;

    Serial.printf("[Humidity] Added zone: %s (target=%.1f%%, dehumid_pin=%d)\n",
                 config.zone_name.c_str(),
                 config.target_humidity_percent,
                 config.dehumidifier_pin);

    return true;
}

bool HumidityControlModule::set_zone_mode(const String& zone_name, HumidityMode mode) {
    auto it = zones_.find(zone_name);
    if (it == zones_.end()) {
        return false;
    }

    it->second.mode = mode;
    Serial.printf("[Humidity] Zone %s mode set to %d\n",
                 zone_name.c_str(), static_cast<int>(mode));
    return true;
}

bool HumidityControlModule::set_zone_target(const String& zone_name, float target_percent) {
    auto it = zones_.find(zone_name);
    if (it == zones_.end()) {
        return false;
    }

    it->second.config.target_humidity_percent = target_percent;
    Serial.printf("[Humidity] Zone %s target set to %.1f%%\n",
                 zone_name.c_str(), target_percent);
    return true;
}

HumidityReading HumidityControlModule::get_sensor_reading(const String& sensor_name) const {
    HumidityReading reading;
    reading.sensor_name = sensor_name;
    reading.valid = false;

    auto it = sensors_.find(sensor_name);
    if (it == sensors_.end()) {
        return reading;
    }

    const Sensor& sensor = it->second;
    reading.relative_humidity_percent = sensor.humidity_percent;
    reading.temperature_c = sensor.temperature_c;
    reading.dewpoint_c = sensor.dewpoint_c;
    reading.timestamp_ms = sensor.last_read_ms;
    reading.valid = sensor.valid;

    return reading;
}

HumidityZoneStatus HumidityControlModule::get_zone_status(const String& zone_name) const {
    HumidityZoneStatus status;
    status.zone_name = zone_name;
    status.has_error = true;
    status.error_message = "Zone not found";

    auto it = zones_.find(zone_name);
    if (it == zones_.end()) {
        return status;
    }

    const Zone& zone = it->second;

    // Get current humidity from sensor
    auto sensor_it = sensors_.find(zone.config.sensor_name);
    float current_humidity = 0.0;
    float dewpoint = 0.0;
    if (sensor_it != sensors_.end()) {
        current_humidity = sensor_it->second.humidity_percent;
        dewpoint = sensor_it->second.dewpoint_c;
    }

    status.zone_name = zone.config.zone_name;
    status.mode = zone.mode;
    status.dehumidifier_state = zone.dehumidifier_state;
    status.humidifier_state = zone.humidifier_state;
    status.current_humidity_percent = current_humidity;
    status.target_humidity_percent = zone.config.target_humidity_percent;
    status.dewpoint_c = dewpoint;
    status.mold_risk = zone.mold_risk_active;
    status.condensation_risk = false;  // TODO: Implement condensation risk
    status.dehumidifier_runtime_hours = zone.dehumidifier_runtime_ms / 3600000;
    status.humidifier_runtime_hours = zone.humidifier_runtime_ms / 3600000;
    status.has_error = zone.has_error;
    status.error_message = zone.error_message;

    return status;
}

std::vector<HumidityZoneStatus> HumidityControlModule::get_all_zones_status() const {
    std::vector<HumidityZoneStatus> statuses;
    for (const auto& pair : zones_) {
        statuses.push_back(get_zone_status(pair.first));
    }
    return statuses;
}

void HumidityControlModule::set_zone_enabled(const String& zone_name, bool enabled) {
    auto it = zones_.find(zone_name);
    if (it == zones_.end()) {
        return;
    }

    it->second.config.enabled = enabled;

    if (!enabled) {
        // Turn off devices
        if (it->second.config.dehumidifier_pin > 0) {
            set_device_output(it->second.config.dehumidifier_pin, false);
        }
        if (it->second.config.humidifier_pin > 0) {
            set_device_output(it->second.config.humidifier_pin, false);
        }
        it->second.dehumidifier_state = HumidityDeviceState::OFF;
        it->second.humidifier_state = HumidityDeviceState::OFF;
    }

    Serial.printf("[Humidity] Zone %s %s\n",
                 zone_name.c_str(), enabled ? "enabled" : "disabled");
}

void HumidityControlModule::read_sensors() {
    for (auto& pair : sensors_) {
        Sensor& sensor = pair.second;

        if (!sensor.config.enabled) {
            continue;
        }

        bool success = false;
        float prev_humidity = sensor.humidity_percent;

        // Read based on sensor type
        switch (sensor.config.type) {
            case HumiditySensorType::BME280:
                success = read_bme280(sensor);
                break;

            case HumiditySensorType::SHT31:
                success = read_sht31(sensor);
                break;

            case HumiditySensorType::DHT22:
                success = read_dht22(sensor);
                break;

            case HumiditySensorType::INTERNAL:
                // Use temperature module + rough estimation
                // This is a fallback, not accurate
                sensor.valid = false;
                break;
        }

        if (success) {
            // Apply calibration
            sensor.humidity_percent += sensor.config.humidity_offset;
            sensor.temperature_c += sensor.config.temperature_offset_c;

            // Calculate dewpoint
            sensor.dewpoint_c = calculate_dewpoint(sensor.temperature_c,
                                                   sensor.humidity_percent);

            sensor.last_read_ms = millis();
            sensor.valid = true;

            // Publish event if humidity changed significantly
            if (fabs(sensor.humidity_percent - prev_humidity) >= 2.0) {
                publish_humidity_event(sensor.config.name,
                                     sensor.humidity_percent,
                                     prev_humidity);
            }
        } else {
            sensor.valid = false;
            Serial.printf("[Humidity] Failed to read sensor: %s\n",
                         sensor.config.name.c_str());
        }
    }
}

bool HumidityControlModule::read_bme280(Sensor& sensor) {
    // BME280 I2C read implementation
    // This is a simplified version - full implementation would use Adafruit BME280 library
    HAL& hal = HAL::get_instance();

    // Read humidity register (0xFD-0xFE for BME280)
    uint8_t hum_msb, hum_lsb;
    if (!hal.i2c_read_byte(sensor.config.i2c_address, 0xFD, &hum_msb) ||
        !hal.i2c_read_byte(sensor.config.i2c_address, 0xFE, &hum_lsb)) {
        return false;
    }

    // Read temperature register (0xFA-0xFC for BME280)
    uint8_t temp_msb, temp_lsb, temp_xlsb;
    if (!hal.i2c_read_byte(sensor.config.i2c_address, 0xFA, &temp_msb) ||
        !hal.i2c_read_byte(sensor.config.i2c_address, 0xFB, &temp_lsb) ||
        !hal.i2c_read_byte(sensor.config.i2c_address, 0xFC, &temp_xlsb)) {
        return false;
    }

    // Simplified conversion (actual BME280 requires compensation)
    uint16_t raw_humidity = (hum_msb << 8) | hum_lsb;
    sensor.humidity_percent = (float)raw_humidity / 1024.0;  // Simplified

    int32_t raw_temp = (temp_msb << 12) | (temp_lsb << 4) | (temp_xlsb >> 4);
    sensor.temperature_c = (float)raw_temp / 5120.0;  // Simplified

    return true;
}

bool HumidityControlModule::read_sht31(Sensor& sensor) {
    // SHT31 I2C read implementation
    HAL& hal = HAL::get_instance();

    // Send measurement command (0x2C06 for high repeatability)
    if (!hal.i2c_write_byte(sensor.config.i2c_address, 0x2C, 0x06)) {
        return false;
    }

    // Wait for measurement (max 15ms for SHT31)
    delay(20);

    // Read 6 bytes (temp MSB, temp LSB, temp CRC, hum MSB, hum LSB, hum CRC)
    uint8_t data[6];
    if (!hal.i2c_read_bytes(sensor.config.i2c_address, 0x00, data, 6)) {
        return false;
    }

    // Convert temperature
    uint16_t raw_temp = (data[0] << 8) | data[1];
    sensor.temperature_c = -45.0 + (175.0 * raw_temp / 65535.0);

    // Convert humidity
    uint16_t raw_hum = (data[3] << 8) | data[4];
    sensor.humidity_percent = 100.0 * raw_hum / 65535.0;

    return true;
}

bool HumidityControlModule::read_dht22(Sensor& sensor) {
    // DHT22 is more complex - requires precise timing
    // This is a placeholder - full implementation would use DHT library
    // For now, return false (not implemented)
    return false;
}

float HumidityControlModule::calculate_dewpoint(float temp_c, float rh_percent) {
    // Magnus formula for dewpoint calculation
    // Valid for: 0°C < T < 60°C, 1% < RH < 100%

    const float a = 17.27;
    const float b = 237.7;

    float alpha = ((a * temp_c) / (b + temp_c)) + log(rh_percent / 100.0);
    float dewpoint = (b * alpha) / (a - alpha);

    return dewpoint;
}

void HumidityControlModule::update_zone(Zone& zone) {
    if (!zone.config.enabled) {
        return;
    }

    // Get current humidity from sensor
    auto sensor_it = sensors_.find(zone.config.sensor_name);
    if (sensor_it == sensors_.end() || !sensor_it->second.valid) {
        zone.has_error = true;
        zone.error_message = "Sensor not available";
        return;
    }

    float current_humidity = sensor_it->second.humidity_percent;

    // Check mold risk
    check_mold_risk(zone, current_humidity);

    // Control devices based on mode
    switch (zone.mode) {
        case HumidityMode::OFF:
            // Turn off both devices
            if (zone.dehumidifier_state != HumidityDeviceState::OFF) {
                set_device_output(zone.config.dehumidifier_pin, false);
                zone.dehumidifier_state = HumidityDeviceState::OFF;
            }
            if (zone.humidifier_state != HumidityDeviceState::OFF) {
                set_device_output(zone.config.humidifier_pin, false);
                zone.humidifier_state = HumidityDeviceState::OFF;
            }
            break;

        case HumidityMode::DEHUMIDIFY_ONLY:
            control_dehumidifier(zone, current_humidity);
            break;

        case HumidityMode::HUMIDIFY_ONLY:
            control_humidifier(zone, current_humidity);
            break;

        case HumidityMode::AUTO:
            control_dehumidifier(zone, current_humidity);
            control_humidifier(zone, current_humidity);
            break;
    }

    zone.has_error = false;
    zone.error_message = "";
}

void HumidityControlModule::check_mold_risk(Zone& zone, float current_humidity) {
    uint32_t now = millis();

    if (current_humidity > zone.config.mold_risk_humidity) {
        if (zone.high_humidity_start_ms == 0) {
            zone.high_humidity_start_ms = now;
        }

        uint32_t duration = now - zone.high_humidity_start_ms;
        if (duration >= zone.config.mold_risk_duration_ms && !zone.mold_risk_active) {
            zone.mold_risk_active = true;

            Serial.printf("[Humidity] MOLD RISK: Zone %s has high humidity %.1f%% for %d seconds\n",
                         zone.config.zone_name.c_str(),
                         current_humidity,
                         duration / 1000);

            // Report to safety engine
            SafetyViolation violation;
            violation.type = ViolationType::HARDWARE_FAULT;  // Closest match
            violation.level = SafetyLevel::WARNING;
            violation.module_name = "HumidityControlModule";
            violation.description = "Mold risk in zone " + zone.config.zone_name;
            violation.value = current_humidity;
            violation.threshold = zone.config.mold_risk_humidity;
            violation.timestamp_ms = now;
            violation.acknowledged = false;
            violation.auto_recovery_attempted = false;
            SafetyEngine::get_instance().report_violation(violation);
        }
    } else {
        zone.high_humidity_start_ms = 0;
        zone.mold_risk_active = false;
    }
}

void HumidityControlModule::control_dehumidifier(Zone& zone, float current_humidity) {
    if (zone.config.dehumidifier_pin == 0) {
        return;  // No dehumidifier configured
    }

    uint32_t now = millis();
    float target = zone.config.target_humidity_percent;
    float hysteresis = zone.config.hysteresis_percent;

    switch (zone.dehumidifier_state) {
        case HumidityDeviceState::OFF:
            // Start if humidity too high
            if (current_humidity > target + hysteresis) {
                // Check minimum off time
                if (now - zone.device_stop_time_ms >= zone.config.min_off_time_ms) {
                    set_device_output(zone.config.dehumidifier_pin, true);
                    zone.dehumidifier_state = HumidityDeviceState::RUNNING;
                    zone.device_start_time_ms = now;

                    Serial.printf("[Humidity] Zone %s: Starting dehumidifier (RH=%.1f%%)\n",
                                 zone.config.zone_name.c_str(), current_humidity);
                }
            }
            break;

        case HumidityDeviceState::RUNNING:
            // Update runtime
            zone.dehumidifier_runtime_ms += (now - zone.device_start_time_ms);
            zone.device_start_time_ms = now;

            // Stop if humidity reached target
            if (current_humidity < target - hysteresis) {
                // Check minimum run time
                if (now - zone.device_start_time_ms >= zone.config.min_run_time_ms) {
                    set_device_output(zone.config.dehumidifier_pin, false);
                    zone.dehumidifier_state = HumidityDeviceState::OFF;
                    zone.device_stop_time_ms = now;

                    Serial.printf("[Humidity] Zone %s: Stopping dehumidifier (RH=%.1f%%)\n",
                                 zone.config.zone_name.c_str(), current_humidity);
                }
            }
            break;

        case HumidityDeviceState::COOLDOWN:
        case HumidityDeviceState::ERROR:
            // Handle these states if needed
            break;
    }
}

void HumidityControlModule::control_humidifier(Zone& zone, float current_humidity) {
    if (zone.config.humidifier_pin == 0) {
        return;  // No humidifier configured
    }

    uint32_t now = millis();
    float target = zone.config.target_humidity_percent;
    float hysteresis = zone.config.hysteresis_percent;

    switch (zone.humidifier_state) {
        case HumidityDeviceState::OFF:
            // Start if humidity too low
            if (current_humidity < target - hysteresis) {
                // Check minimum off time
                if (now - zone.device_stop_time_ms >= zone.config.min_off_time_ms) {
                    set_device_output(zone.config.humidifier_pin, true);
                    zone.humidifier_state = HumidityDeviceState::RUNNING;
                    zone.device_start_time_ms = now;

                    Serial.printf("[Humidity] Zone %s: Starting humidifier (RH=%.1f%%)\n",
                                 zone.config.zone_name.c_str(), current_humidity);
                }
            }
            break;

        case HumidityDeviceState::RUNNING:
            // Update runtime
            zone.humidifier_runtime_ms += (now - zone.device_start_time_ms);
            zone.device_start_time_ms = now;

            // Stop if humidity reached target
            if (current_humidity > target + hysteresis) {
                // Check minimum run time
                if (now - zone.device_start_time_ms >= zone.config.min_run_time_ms) {
                    set_device_output(zone.config.humidifier_pin, false);
                    zone.humidifier_state = HumidityDeviceState::OFF;
                    zone.device_stop_time_ms = now;

                    Serial.printf("[Humidity] Zone %s: Stopping humidifier (RH=%.1f%%)\n",
                                 zone.config.zone_name.c_str(), current_humidity);
                }
            }
            break;

        case HumidityDeviceState::COOLDOWN:
        case HumidityDeviceState::ERROR:
            // Handle these states if needed
            break;
    }
}

void HumidityControlModule::set_device_output(uint8_t pin, bool on) {
    if (pin > 0) {
        HAL::get_instance().digital_write(pin, on);
    }
}

void HumidityControlModule::publish_humidity_event(const String& sensor_name,
                                                   float humidity,
                                                   float previous) {
    ValueEventData event;
    event.source_module = "HumidityControlModule";
    event.parameter_name = sensor_name;
    event.value = humidity;
    event.previous_value = previous;
    event.units = "%RH";

    EventBus::get_instance().publish(EventType::HUMIDITY_CHANGED, &event);
}
