/**
 * @file VentilationModule.cpp
 * @purpose Implementation of ventilation control module
 * @dependencies VentilationModule.h
 * @version 1.0.0
 */

#include "modules/VentilationModule.h"
#include "ConfigManager.h"

VentilationModule::VentilationModule()
    : mode_(VentilationMode::OFF),
      supply_speed_(0),
      exhaust_speed_(0),
      total_runtime_ms_(0),
      has_error_(false)
{
    state_ = ModuleState::UNINITIALIZED;

    config_.supply_fan_pin = 14;
    config_.supply_fan_channel = 2;
    config_.exhaust_fan_pin = 12;
    config_.exhaust_fan_channel = 3;
    config_.min_speed_percent = 20;
    config_.max_speed_percent = 100;
    config_.enabled = true;
}

ModuleMetadata VentilationModule::get_metadata() const {
    ModuleMetadata meta;
    meta.name = "VentilationModule";
    meta.version = "1.0.0";
    meta.description = "HRV/ERV ventilation control";
    meta.ram_usage_kb = 5;
    meta.flash_usage_kb = 15;
    meta.capabilities["fans"] = "supply,exhaust";
    meta.capabilities["speed_control"] = "PWM";
    return meta;
}

HealthStatus VentilationModule::health_check() {
    HealthStatus health;
    health.last_update_ms = millis();
    health.is_healthy = !has_error_;
    health.health_percentage = has_error_ ? 0 : 100;
    health.status_message = has_error_ ? error_message_ : "Operating normally";
    health.metrics["supply_speed"] = supply_speed_;
    health.metrics["exhaust_speed"] = exhaust_speed_;
    return health;
}

bool VentilationModule::initialize() {
    Serial.println("[Ventilation] Initializing...");
    state_ = ModuleState::INITIALIZING;

    HAL& hal = HAL::get_instance();

    // Initialize PWM for fans
    if (!hal.pwm_init(config_.supply_fan_pin, config_.supply_fan_channel, 25000, 8)) {
        Serial.println("[Ventilation] ERROR: Failed to init supply fan PWM");
        state_ = ModuleState::ERROR;
        return false;
    }

    if (!hal.pwm_init(config_.exhaust_fan_pin, config_.exhaust_fan_channel, 25000, 8)) {
        Serial.println("[Ventilation] ERROR: Failed to init exhaust fan PWM");
        state_ = ModuleState::ERROR;
        return false;
    }

    // Set fans to off
    hal.pwm_write(config_.supply_fan_channel, 0);
    hal.pwm_write(config_.exhaust_fan_channel, 0);

    state_ = ModuleState::RUNNING;
    Serial.println("[Ventilation] Initialized");
    return true;
}

void VentilationModule::shutdown() {
    Serial.println("[Ventilation] Shutting down...");

    HAL& hal = HAL::get_instance();
    hal.pwm_write(config_.supply_fan_channel, 0);
    hal.pwm_write(config_.exhaust_fan_channel, 0);

    supply_speed_ = 0;
    exhaust_speed_ = 0;
    mode_ = VentilationMode::OFF;

    state_ = ModuleState::SHUTDOWN;
    Serial.println("[Ventilation] Shutdown complete");
}

void VentilationModule::update() {
    if (state_ != ModuleState::RUNNING) {
        return;
    }

    // Update runtime if fans are running
    if (supply_speed_ > 0 || exhaust_speed_ > 0) {
        total_runtime_ms_ += 100;  // Approximate
    }
}

bool VentilationModule::set_mode(VentilationMode mode) {
    mode_ = mode;

    uint8_t speed;
    switch (mode) {
        case VentilationMode::OFF:
            speed = 0;
            break;
        case VentilationMode::LOW:
            speed = 30;
            break;
        case VentilationMode::MEDIUM:
            speed = 60;
            break;
        case VentilationMode::HIGH:
            speed = 90;
            break;
        case VentilationMode::BOOST:
            speed = 100;
            break;
        case VentilationMode::AUTO:
            speed = 50;  // Default auto speed
            break;
        default:
            speed = 0;
    }

    return set_fan_speed(speed, speed);
}

bool VentilationModule::set_fan_speed(uint8_t supply_percent, uint8_t exhaust_percent) {
    if (!config_.enabled) {
        return false;
    }

    // Clamp to limits
    if (supply_percent > config_.max_speed_percent) supply_percent = config_.max_speed_percent;
    if (exhaust_percent > config_.max_speed_percent) exhaust_percent = config_.max_speed_percent;

    // Apply minimum or turn off
    if (supply_percent > 0 && supply_percent < config_.min_speed_percent) {
        supply_percent = config_.min_speed_percent;
    }
    if (exhaust_percent > 0 && exhaust_percent < config_.min_speed_percent) {
        exhaust_percent = config_.min_speed_percent;
    }

    supply_speed_ = supply_percent;
    exhaust_speed_ = exhaust_percent;

    HAL& hal = HAL::get_instance();
    hal.pwm_write(config_.supply_fan_channel, supply_speed_);
    hal.pwm_write(config_.exhaust_fan_channel, exhaust_speed_);

    Serial.printf("[Ventilation] Fan speeds: supply=%d%%, exhaust=%d%%\n",
                 supply_speed_, exhaust_speed_);

    // Publish event
    StateChangeEventData event;
    event.source_module = "VentilationModule";
    event.component_id = "fans";
    event.new_state = String(supply_speed_) + "/" + String(exhaust_speed_);
    EventBus::get_instance().publish(EventType::FAN_STATE_CHANGED, &event);

    return true;
}

VentilationStatus VentilationModule::get_status() const {
    VentilationStatus status;
    status.mode = mode_;
    status.supply_speed_percent = supply_speed_;
    status.exhaust_speed_percent = exhaust_speed_;
    status.runtime_hours = total_runtime_ms_ / 3600000;
    status.has_error = has_error_;
    status.error_message = error_message_;
    return status;
}
