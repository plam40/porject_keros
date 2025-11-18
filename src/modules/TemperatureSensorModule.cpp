/**
 * @file TemperatureSensorModule.cpp
 * @purpose Implementation of temperature sensor management module
 * @dependencies TemperatureSensorModule.h, HAL.h, ConfigManager.h
 * @version 1.0.0
 * @performance_notes Sensor reads are non-blocking when possible
 */

#include "modules/TemperatureSensorModule.h"
#include "ConfigManager.h"
#include "SafetyEngine.h"
#include <math.h>

TemperatureSensorModule::TemperatureSensorModule()
    : onewire_pin_(4),  // Default OneWire pin
      read_interval_ms_(1000),  // Read every 1 second
      change_threshold_c_(0.5),  // 0.5°C change threshold
      last_read_ms_(0),
      successful_reads_(0),
      failed_reads_(0),
      total_reads_(0)
{
    state_ = ModuleState::UNINITIALIZED;
}

ModuleMetadata TemperatureSensorModule::get_metadata() const {
    ModuleMetadata meta;
    meta.name = "TemperatureSensorModule";
    meta.version = "1.0.0";
    meta.description = "DS18B20 temperature sensor management";
    meta.ram_usage_kb = 8;
    meta.flash_usage_kb = 20;
    meta.capabilities["sensor_type"] = "DS18B20";
    meta.capabilities["interface"] = "OneWire";
    meta.capabilities["max_sensors"] = "20";
    return meta;
}

HealthStatus TemperatureSensorModule::health_check() {
    HealthStatus health;
    health.last_update_ms = millis();

    // Calculate health percentage based on read success rate
    if (total_reads_ > 0) {
        float success_rate = (float)successful_reads_ / (float)total_reads_;
        health.health_percentage = (uint8_t)(success_rate * 100.0f);
    } else {
        health.health_percentage = 100;  // No reads yet, assume healthy
    }

    // Check if we have any active sensors
    int active_sensors = 0;
    for (const auto& pair : readings_) {
        if (pair.second.valid) {
            active_sensors++;
        }
    }

    if (active_sensors == 0 && !sensors_.empty()) {
        health.is_healthy = false;
        health.status_message = "No valid sensor readings";
    } else if (health.health_percentage < 50) {
        health.is_healthy = false;
        health.status_message = "High sensor read failure rate";
    } else {
        health.is_healthy = true;
        health.status_message = "Operating normally";
    }

    health.metrics["active_sensors"] = active_sensors;
    health.metrics["total_sensors"] = sensors_.size();
    health.metrics["success_rate"] = health.health_percentage;

    return health;
}

bool TemperatureSensorModule::initialize() {
    Serial.println("[TempSensor] Initializing...");
    state_ = ModuleState::INITIALIZING;

    // Load configuration
    ConfigManager& config = ConfigManager::get_instance();
    onewire_pin_ = config.get_int(ConfigSection::SENSORS, "onewire_pin", 4);
    read_interval_ms_ = config.get_int(ConfigSection::SENSORS, "read_interval_ms", 1000);
    change_threshold_c_ = config.get_float(ConfigSection::SENSORS, "change_threshold_c", 0.5);

    // Initialize HAL OneWire bus
    HAL& hal = HAL::get_instance();
    if (!hal.onewire_init(onewire_pin_)) {
        Serial.println("[TempSensor] ERROR: Failed to initialize OneWire bus");
        state_ = ModuleState::ERROR;
        return false;
    }

    // Scan for sensors
    int sensor_count = scan_sensors();
    Serial.printf("[TempSensor] Found %d sensors on bus\n", sensor_count);

    if (sensor_count == 0) {
        Serial.println("[TempSensor] WARNING: No sensors found, module will start in degraded mode");
        state_ = ModuleState::DEGRADED;
    } else {
        state_ = ModuleState::RUNNING;
    }

    Serial.println("[TempSensor] Initialization complete");
    return true;
}

void TemperatureSensorModule::shutdown() {
    Serial.println("[TempSensor] Shutting down...");

    // Clear all sensor data
    sensors_.clear();
    readings_.clear();
    name_to_address_.clear();

    state_ = ModuleState::SHUTDOWN;
    Serial.println("[TempSensor] Shutdown complete");
}

void TemperatureSensorModule::update() {
    if (state_ != ModuleState::RUNNING && state_ != ModuleState::DEGRADED) {
        return;
    }

    // Check if it's time to read sensors
    uint32_t now = millis();
    if (now - last_read_ms_ < read_interval_ms_) {
        return;
    }

    last_read_ms_ = now;
    read_all_sensors();
}

float TemperatureSensorModule::get_temperature(const String& name) const {
    auto it = name_to_address_.find(name);
    if (it == name_to_address_.end()) {
        return NAN;
    }

    return get_temperature(it->second);
}

float TemperatureSensorModule::get_temperature(uint64_t address) const {
    auto it = readings_.find(address);
    if (it == readings_.end() || !it->second.valid) {
        return NAN;
    }

    return it->second.temperature_c;
}

std::map<String, float> TemperatureSensorModule::get_all_temperatures() const {
    std::map<String, float> temps;

    for (const auto& pair : readings_) {
        if (pair.second.valid) {
            temps[pair.second.name] = pair.second.temperature_c;
        }
    }

    return temps;
}

TemperatureReading TemperatureSensorModule::get_reading(const String& name) const {
    auto it = name_to_address_.find(name);
    if (it == name_to_address_.end()) {
        TemperatureReading invalid;
        invalid.valid = false;
        return invalid;
    }

    auto reading_it = readings_.find(it->second);
    if (reading_it == readings_.end()) {
        TemperatureReading invalid;
        invalid.valid = false;
        return invalid;
    }

    return reading_it->second;
}

int TemperatureSensorModule::scan_sensors() {
    HAL& hal = HAL::get_instance();

    // Scan OneWire bus
    std::vector<uint64_t> addresses = hal.onewire_scan();

    // Add any new sensors (with auto-generated names)
    int added = 0;
    for (uint64_t addr : addresses) {
        if (sensors_.find(addr) == sensors_.end()) {
            // New sensor found
            TemperatureSensor sensor;
            sensor.address = addr;
            sensor.name = "sensor_" + String((unsigned long)addr, HEX);
            sensor.offset_c = 0.0;
            sensor.min_valid_temp_c = -30.0;
            sensor.max_valid_temp_c = 100.0;
            sensor.enabled = true;

            add_sensor(sensor);
            added++;
        }
    }

    Serial.printf("[TempSensor] Scan complete: %d sensors found, %d new\n",
                 addresses.size(), added);

    return addresses.size();
}

bool TemperatureSensorModule::add_sensor(const TemperatureSensor& sensor) {
    if (sensors_.find(sensor.address) != sensors_.end()) {
        Serial.printf("[TempSensor] WARNING: Sensor %016llX already exists\n",
                     sensor.address);
        return false;
    }

    sensors_[sensor.address] = sensor;
    name_to_address_[sensor.name] = sensor.address;

    Serial.printf("[TempSensor] Added sensor: %s (%016llX)\n",
                 sensor.name.c_str(), sensor.address);

    return true;
}

void TemperatureSensorModule::remove_sensor(const String& name) {
    auto it = name_to_address_.find(name);
    if (it == name_to_address_.end()) {
        return;
    }

    uint64_t address = it->second;
    sensors_.erase(address);
    readings_.erase(address);
    name_to_address_.erase(it);

    Serial.printf("[TempSensor] Removed sensor: %s\n", name.c_str());
}

std::vector<TemperatureSensor> TemperatureSensorModule::get_sensors() const {
    std::vector<TemperatureSensor> result;
    for (const auto& pair : sensors_) {
        result.push_back(pair.second);
    }
    return result;
}

void TemperatureSensorModule::read_all_sensors() {
    HAL& hal = HAL::get_instance();

    // Read all sensors
    for (auto& pair : sensors_) {
        TemperatureSensor& sensor = pair.second;

        if (!sensor.enabled) {
            continue;
        }

        total_reads_++;

        // Read temperature
        float raw_temp_c = hal.read_ds18b20(sensor.address);

        // Check for read error
        if (raw_temp_c == -127.0f) {
            failed_reads_++;
            Serial.printf("[TempSensor] ERROR: Failed to read sensor %s\n",
                         sensor.name.c_str());

            // Publish error event
            SafetyEventData event;
            event.source_module = "TemperatureSensorModule";
            event.violation_type = "SENSOR_READ_ERROR";
            event.affected_component = sensor.name;
            event.severity = 1;  // Warning
            event.remediation_action = "Check sensor connection";
            EventBus::get_instance().publish(EventType::TEMPERATURE_SENSOR_ERROR, &event);

            continue;
        }

        successful_reads_++;
        process_reading(sensor, raw_temp_c);
    }
}

void TemperatureSensorModule::process_reading(const TemperatureSensor& sensor,
                                              float raw_temp_c) {
    // Apply calibration offset
    float corrected_temp_c = raw_temp_c + sensor.offset_c;

    // Validate temperature
    if (!is_temperature_valid(sensor, corrected_temp_c)) {
        Serial.printf("[TempSensor] WARNING: Invalid temperature %.2f°C from %s (range: %.1f-%.1f°C)\n",
                     corrected_temp_c, sensor.name.c_str(),
                     sensor.min_valid_temp_c, sensor.max_valid_temp_c);
        return;
    }

    // Get previous reading
    float previous_temp_c = NAN;
    auto it = readings_.find(sensor.address);
    if (it != readings_.end() && it->second.valid) {
        previous_temp_c = it->second.temperature_c;
    }

    // Store reading
    TemperatureReading reading;
    reading.address = sensor.address;
    reading.name = sensor.name;
    reading.temperature_c = corrected_temp_c;
    reading.timestamp_ms = millis();
    reading.valid = true;
    readings_[sensor.address] = reading;

    // Check if temperature changed significantly
    if (isnan(previous_temp_c) ||
        fabs(corrected_temp_c - previous_temp_c) >= change_threshold_c_) {
        publish_temperature_event(sensor, corrected_temp_c, previous_temp_c);
    }
}

bool TemperatureSensorModule::is_temperature_valid(const TemperatureSensor& sensor,
                                                   float temp_c) {
    return temp_c >= sensor.min_valid_temp_c &&
           temp_c <= sensor.max_valid_temp_c;
}

void TemperatureSensorModule::publish_temperature_event(const TemperatureSensor& sensor,
                                                        float temperature_c,
                                                        float previous_temperature_c) {
    TemperatureEventData event;
    event.source_module = "TemperatureSensorModule";
    event.sensor_id = sensor.name;
    event.temperature_c = temperature_c;
    event.previous_temperature_c = previous_temperature_c;

    EventBus::get_instance().publish(EventType::TEMPERATURE_CHANGED, &event);
}
