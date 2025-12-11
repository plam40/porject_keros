/**
 * @file PumpModule.cpp
 * @purpose Implementation of circulation pump control module
 * @dependencies PumpModule.h, HAL.h, ConfigManager.h
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes Pump update cycle: <1ms per pump
 * @module_type MODULE
 */

#include "modules/PumpModule.h"
#include "ConfigManager.h"
#include "SafetyEngine.h"

PumpModule::PumpModule() : last_update_ms_(0) {
    state_ = ModuleState::UNINITIALIZED;
}

ModuleMetadata PumpModule::get_metadata() const {
    ModuleMetadata meta;
    meta.name = "PumpModule";
    meta.version = "1.0.0";
    meta.description = "Variable speed circulation pump control";
    meta.ram_usage_kb = 12;
    meta.flash_usage_kb = 25;
    meta.capabilities["max_pumps"] = "8";
    meta.capabilities["speed_control"] = "PWM";
    meta.capabilities["current_monitoring"] = "yes";
    return meta;
}

HealthStatus PumpModule::health_check() {
    HealthStatus health;
    health.last_update_ms = millis();
    health.is_healthy = true;
    health.health_percentage = 100;
    health.status_message = "Operating normally";

    int total_pumps = 0;
    int healthy_pumps = 0;
    int error_pumps = 0;

    for (const auto& pair : pumps_) {
        const Pump& pump = pair.second;
        total_pumps++;

        if (pump.has_error) {
            error_pumps++;
        } else {
            healthy_pumps++;
        }
    }

    if (total_pumps > 0) {
        health.health_percentage = (healthy_pumps * 100) / total_pumps;

        if (error_pumps > 0) {
            health.is_healthy = false;
            health.status_message = String(error_pumps) + " pump(s) in error state";
        }
    }

    health.metrics["total_pumps"] = total_pumps;
    health.metrics["healthy_pumps"] = healthy_pumps;
    health.metrics["error_pumps"] = error_pumps;

    return health;
}

bool PumpModule::initialize() {
    Serial.println("[PumpModule] Initializing...");
    state_ = ModuleState::INITIALIZING;

    HAL& hal = HAL::get_instance();

    // Initialize all configured pumps
    for (auto& pair : pumps_) {
        Pump& pump = pair.second;

        if (!pump.config.enabled) {
            continue;
        }

        // Initialize PWM for pump control
        if (!hal.pwm_init(pump.config.pwm_pin, pump.config.pwm_channel, 25000, 8)) {
            Serial.printf("[PumpModule] ERROR: Failed to initialize PWM for %s\n",
                         pump.config.name.c_str());
            state_ = ModuleState::ERROR;
            return false;
        }

        // Set initial state
        pump.mode = PumpMode::OFF;
        pump.state = PumpState::STOPPED;
        pump.target_speed = 0;
        pump.current_speed = 0;
        pump.state_start_time_ms = millis();
        pump.last_start_time_ms = 0;
        pump.last_stop_time_ms = millis();
        pump.total_runtime_ms = 0;
        pump.start_count = 0;
        pump.current_amps = 0.0;
        pump.has_error = false;

        // Ensure pump is off
        set_pwm_output(pump, 0);

        Serial.printf("[PumpModule] Initialized pump: %s (pin=%d, ch=%d)\n",
                     pump.config.name.c_str(),
                     pump.config.pwm_pin,
                     pump.config.pwm_channel);
    }

    state_ = ModuleState::RUNNING;
    Serial.println("[PumpModule] Initialization complete");
    return true;
}

void PumpModule::shutdown() {
    Serial.println("[PumpModule] Shutting down...");

    // Stop all pumps
    for (auto& pair : pumps_) {
        Pump& pump = pair.second;
        pump.target_speed = 0;
        pump.current_speed = 0;
        pump.state = PumpState::STOPPED;
        set_pwm_output(pump, 0);

        Serial.printf("[PumpModule] Stopped pump: %s\n", pump.config.name.c_str());
    }

    state_ = ModuleState::SHUTDOWN;
    Serial.println("[PumpModule] Shutdown complete");
}

void PumpModule::update() {
    if (state_ != ModuleState::RUNNING) {
        return;
    }

    uint32_t now = millis();
    last_update_ms_ = now;

    // Update all pumps
    for (auto& pair : pumps_) {
        update_pump(pair.second);
    }
}

bool PumpModule::add_pump(const PumpConfig& config) {
    if (pumps_.find(config.name) != pumps_.end()) {
        Serial.printf("[PumpModule] WARNING: Pump %s already exists\n",
                     config.name.c_str());
        return false;
    }

    Pump pump;
    pump.config = config;
    pump.mode = PumpMode::OFF;
    pump.state = PumpState::STOPPED;
    pump.target_speed = 0;
    pump.current_speed = 0;
    pump.state_start_time_ms = millis();
    pump.last_start_time_ms = 0;
    pump.last_stop_time_ms = millis();
    pump.total_runtime_ms = 0;
    pump.start_count = 0;
    pump.current_amps = 0.0;
    pump.has_error = false;

    pumps_[config.name] = pump;

    Serial.printf("[PumpModule] Added pump: %s\n", config.name.c_str());
    return true;
}

bool PumpModule::set_pump_mode(const String& name, PumpMode mode) {
    auto it = pumps_.find(name);
    if (it == pumps_.end()) {
        return false;
    }

    Pump& pump = it->second;
    pump.mode = mode;

    Serial.printf("[PumpModule] Pump %s mode set to %d\n",
                 name.c_str(), static_cast<int>(mode));

    return true;
}

bool PumpModule::set_pump_speed(const String& name, uint8_t speed_percent) {
    auto it = pumps_.find(name);
    if (it == pumps_.end()) {
        return false;
    }

    Pump& pump = it->second;

    // Clamp to configured limits
    if (speed_percent > pump.config.max_speed_percent) {
        speed_percent = pump.config.max_speed_percent;
    }

    // If speed is below minimum, turn off
    if (speed_percent > 0 && speed_percent < pump.config.min_speed_percent) {
        speed_percent = pump.config.min_speed_percent;
    }

    pump.target_speed = speed_percent;

    Serial.printf("[PumpModule] Pump %s target speed set to %d%%\n",
                 name.c_str(), speed_percent);

    return true;
}

bool PumpModule::start_pump(const String& name) {
    auto it = pumps_.find(name);
    if (it == pumps_.end()) {
        return false;
    }

    Pump& pump = it->second;

    if (!pump.config.enabled) {
        Serial.printf("[PumpModule] WARNING: Pump %s is disabled\n", name.c_str());
        return false;
    }

    // Check protection conditions
    if (!check_pump_protection(pump)) {
        return false;
    }

    // Set mode to manual if not already set
    if (pump.mode == PumpMode::OFF) {
        pump.mode = PumpMode::MANUAL;
    }

    // Set target speed to minimum if not already set
    if (pump.target_speed == 0) {
        pump.target_speed = pump.config.min_speed_percent;
    }

    Serial.printf("[PumpModule] Starting pump: %s\n", name.c_str());
    return true;
}

bool PumpModule::stop_pump(const String& name) {
    auto it = pumps_.find(name);
    if (it == pumps_.end()) {
        return false;
    }

    Pump& pump = it->second;
    pump.target_speed = 0;
    pump.mode = PumpMode::OFF;

    Serial.printf("[PumpModule] Stopping pump: %s\n", name.c_str());
    return true;
}

PumpStatus PumpModule::get_pump_status(const String& name) const {
    PumpStatus status;
    status.name = name;
    status.has_error = true;
    status.error_message = "Pump not found";

    auto it = pumps_.find(name);
    if (it == pumps_.end()) {
        return status;
    }

    const Pump& pump = it->second;
    status.name = pump.config.name;
    status.mode = pump.mode;
    status.state = pump.state;
    status.target_speed_percent = pump.target_speed;
    status.actual_speed_percent = pump.current_speed;
    status.current_amps = pump.current_amps;
    status.runtime_seconds = pump.total_runtime_ms / 1000;
    status.total_runtime_hours = pump.total_runtime_ms / 3600000;
    status.start_count = pump.start_count;
    status.has_error = pump.has_error;
    status.error_message = pump.error_message;

    return status;
}

std::map<String, PumpStatus> PumpModule::get_all_pump_status() const {
    std::map<String, PumpStatus> statuses;

    for (const auto& pair : pumps_) {
        statuses[pair.first] = get_pump_status(pair.first);
    }

    return statuses;
}

void PumpModule::set_pump_enabled(const String& name, bool enabled) {
    auto it = pumps_.find(name);
    if (it == pumps_.end()) {
        return;
    }

    it->second.config.enabled = enabled;

    if (!enabled) {
        stop_pump(name);
    }

    Serial.printf("[PumpModule] Pump %s %s\n",
                 name.c_str(), enabled ? "enabled" : "disabled");
}

void PumpModule::update_pump(Pump& pump) {
    if (!pump.config.enabled) {
        return;
    }

    PumpState previous_state = pump.state;
    uint32_t now = millis();

    // Monitor current if configured
    if (pump.config.current_sense_pin > 0) {
        monitor_pump_current(pump);
    }

    // State machine
    switch (pump.state) {
        case PumpState::STOPPED:
            if (pump.target_speed > 0 && pump.mode != PumpMode::OFF) {
                // Check if minimum off-time has elapsed
                if (now - pump.last_stop_time_ms >= pump.config.min_off_time_ms) {
                    pump.state = PumpState::STARTING;
                    pump.state_start_time_ms = now;
                    pump.last_start_time_ms = now;
                    pump.start_count++;
                }
            }
            break;

        case PumpState::STARTING:
            // Soft start - gradually increase speed
            update_pump_speed(pump);

            // Check if soft start complete
            if (now - pump.state_start_time_ms >= pump.config.soft_start_time_ms) {
                pump.state = PumpState::RUNNING;
                pump.state_start_time_ms = now;
            }
            break;

        case PumpState::RUNNING:
            update_pump_speed(pump);

            // Update runtime
            pump.total_runtime_ms += (now - last_update_ms_);

            // Check if should stop
            if (pump.target_speed == 0 || pump.mode == PumpMode::OFF) {
                // Check if minimum on-time has elapsed
                if (now - pump.last_start_time_ms >= pump.config.min_on_time_ms) {
                    pump.state = PumpState::STOPPING;
                    pump.state_start_time_ms = now;
                }
            }
            break;

        case PumpState::STOPPING:
            // Gradually decrease speed
            if (pump.current_speed > 0) {
                pump.current_speed = max(0, pump.current_speed - 5);
                set_pwm_output(pump, pump.current_speed);
            } else {
                pump.state = PumpState::STOPPED;
                pump.state_start_time_ms = now;
                pump.last_stop_time_ms = now;
            }
            break;

        case PumpState::ERROR:
            // Stop pump
            pump.current_speed = 0;
            set_pwm_output(pump, 0);
            // Error must be cleared manually
            break;
    }

    // Publish event if state changed
    if (pump.state != previous_state) {
        publish_pump_event(pump, previous_state);
    }
}

void PumpModule::update_pump_speed(Pump& pump) {
    // Calculate target speed based on state
    uint8_t target;

    if (pump.state == PumpState::STARTING) {
        // Soft start - linear ramp
        uint32_t elapsed = millis() - pump.state_start_time_ms;
        float progress = (float)elapsed / (float)pump.config.soft_start_time_ms;
        progress = min(1.0f, progress);
        target = (uint8_t)(pump.target_speed * progress);
    } else {
        target = pump.target_speed;
    }

    // Gradually adjust current speed (prevents sudden changes)
    if (target > pump.current_speed) {
        pump.current_speed = min(target, pump.current_speed + 2);
    } else if (target < pump.current_speed) {
        pump.current_speed = max(target, pump.current_speed - 2);
    }

    // Update hardware PWM
    set_pwm_output(pump, pump.current_speed);
}

void PumpModule::monitor_pump_current(Pump& pump) {
    HAL& hal = HAL::get_instance();

    // Read current sensor (average multiple samples)
    float voltage = hal.analog_read_voltage(pump.config.current_sense_pin, 10);

    // Convert voltage to current (depends on current sensor)
    pump.current_amps = voltage * pump.config.current_calibration;

    // Check for overcurrent
    if (pump.current_amps > pump.config.max_current_amps) {
        pump.has_error = true;
        pump.error_message = "Overcurrent detected: " + String(pump.current_amps) + "A";
        pump.state = PumpState::ERROR;

        Serial.printf("[PumpModule] ERROR: Pump %s overcurrent: %.2fA (max %.2fA)\n",
                     pump.config.name.c_str(), pump.current_amps, pump.config.max_current_amps);

        // Report to safety engine
        SafetyViolation violation;
        violation.type = ViolationType::OVERCURRENT;
        violation.level = SafetyLevel::CRITICAL;
        violation.module_name = "PumpModule";
        violation.description = "Pump " + pump.config.name + " overcurrent";
        violation.value = pump.current_amps;
        violation.threshold = pump.config.max_current_amps;
        violation.timestamp_ms = millis();
        violation.acknowledged = false;
        violation.auto_recovery_attempted = false;
        SafetyEngine::get_instance().report_violation(violation);
    }

    // Check for dry running (current too low while running)
    if (pump.state == PumpState::RUNNING &&
        pump.current_speed > 50 &&
        pump.current_amps < pump.config.min_current_amps) {

        pump.has_error = true;
        pump.error_message = "Dry running detected: " + String(pump.current_amps) + "A";
        pump.state = PumpState::ERROR;

        Serial.printf("[PumpModule] ERROR: Pump %s dry running: %.2fA (min %.2fA)\n",
                     pump.config.name.c_str(), pump.current_amps, pump.config.min_current_amps);

        // Report to safety engine
        SafetyViolation violation;
        violation.type = ViolationType::HARDWARE_FAULT;
        violation.level = SafetyLevel::WARNING;
        violation.module_name = "PumpModule";
        violation.description = "Pump " + pump.config.name + " dry running";
        violation.value = pump.current_amps;
        violation.threshold = pump.config.min_current_amps;
        violation.timestamp_ms = millis();
        violation.acknowledged = false;
        violation.auto_recovery_attempted = false;
        SafetyEngine::get_instance().report_violation(violation);
    }
}

bool PumpModule::check_pump_protection(Pump& pump) {
    uint32_t now = millis();

    // Check minimum off-time
    if (now - pump.last_stop_time_ms < pump.config.min_off_time_ms) {
        Serial.printf("[PumpModule] WARNING: Pump %s minimum off-time not elapsed\n",
                     pump.config.name.c_str());
        return false;
    }

    // Check for error state
    if (pump.has_error) {
        Serial.printf("[PumpModule] WARNING: Pump %s in error state: %s\n",
                     pump.config.name.c_str(), pump.error_message.c_str());
        return false;
    }

    return true;
}

void PumpModule::set_pwm_output(Pump& pump, uint8_t speed_percent) {
    HAL& hal = HAL::get_instance();
    hal.pwm_write(pump.config.pwm_channel, speed_percent);
}

void PumpModule::publish_pump_event(const Pump& pump, PumpState previous_state) {
    StateChangeEventData event;
    event.source_module = "PumpModule";
    event.component_id = pump.config.name;
    event.previous_state = String(static_cast<int>(previous_state));
    event.new_state = String(static_cast<int>(pump.state));
    event.reason = "State transition";

    EventBus::get_instance().publish(EventType::PUMP_STATE_CHANGED, &event);

    Serial.printf("[PumpModule] Pump %s state: %d -> %d\n",
                 pump.config.name.c_str(),
                 static_cast<int>(previous_state),
                 static_cast<int>(pump.state));
}
