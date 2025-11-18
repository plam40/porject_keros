/**
 * @file HeatPumpModule.cpp
 * @purpose Implementation of heat pump control module
 * @dependencies HeatPumpModule.h, TemperatureSensorModule.h
 * @version 1.0.0
 * @performance_notes State machine update: <2ms, COP calculation: <100μs
 */

#include "modules/HeatPumpModule.h"
#include "modules/TemperatureSensorModule.h"
#include "ConfigManager.h"
#include "SafetyEngine.h"
#include "Skeleton.h"

HeatPumpModule::HeatPumpModule()
    : mode_(HeatPumpMode::OFF),
      state_(HeatPumpState::STOPPED),
      target_supply_temp_c_(50.0),
      supply_temp_c_(0.0),
      return_temp_c_(0.0),
      outdoor_temp_c_(0.0),
      evaporator_temp_c_(0.0),
      current_cop_(0.0),
      state_start_time_ms_(0),
      last_start_time_ms_(0),
      last_stop_time_ms_(0),
      last_defrost_time_ms_(0),
      total_runtime_ms_(0),
      compressor_starts_(0),
      defrost_cycles_(0),
      has_error_(false),
      init_time_ms_(0)
{
    state_ = ModuleState::UNINITIALIZED;

    // Default configuration
    config_.compressor_relay_pin = 27;
    config_.reversing_valve_pin = 0;
    config_.defrost_relay_pin = 0;
    config_.supply_temp_sensor = "supply";
    config_.return_temp_sensor = "return";
    config_.outdoor_temp_sensor = "outdoor";
    config_.evaporator_temp_sensor = "";
    config_.max_supply_temp_c = 65.0;
    config_.min_supply_temp_c = 20.0;
    config_.max_delta_t_c = 15.0;
    config_.min_outdoor_temp_c = -15.0;
    config_.min_on_time_ms = 300000;  // 5 minutes
    config_.min_off_time_ms = 300000;  // 5 minutes
    config_.startup_delay_ms = 30000;  // 30 seconds
    config_.defrost_strategy = DefrostStrategy::HYBRID;
    config_.defrost_enable_temp_c = 5.0;
    config_.defrost_interval_ms = 3600000;  // 1 hour
    config_.defrost_duration_ms = 600000;  // 10 minutes
    config_.defrost_cop_threshold = 2.0;
    config_.min_cop = 1.5;
    config_.nominal_cop = 3.5;
    config_.enabled = true;
}

ModuleMetadata HeatPumpModule::get_metadata() const {
    ModuleMetadata meta;
    meta.name = "HeatPumpModule";
    meta.version = "1.0.0";
    meta.description = "Heat pump control with COP optimization and defrost";
    meta.ram_usage_kb = 15;
    meta.flash_usage_kb = 35;
    meta.capabilities["modes"] = "heating,cooling,defrost";
    meta.capabilities["cop_monitoring"] = "yes";
    meta.capabilities["auto_defrost"] = "yes";
    return meta;
}

HealthStatus HeatPumpModule::health_check() {
    HealthStatus health;
    health.last_update_ms = millis();

    // Base health on COP and error state
    if (has_error_) {
        health.is_healthy = false;
        health.health_percentage = 0;
        health.status_message = error_message_;
    } else if (state_ == HeatPumpState::RUNNING && current_cop_ < config_.min_cop) {
        health.is_healthy = false;
        health.health_percentage = 50;
        health.status_message = "COP below minimum";
    } else if (state_ == HeatPumpState::RUNNING && current_cop_ < config_.nominal_cop * 0.7) {
        health.is_healthy = true;
        health.health_percentage = 70;
        health.status_message = "COP degraded";
    } else {
        health.is_healthy = true;
        health.health_percentage = 100;
        health.status_message = "Operating normally";
    }

    health.metrics["cop"] = current_cop_;
    health.metrics["supply_temp"] = supply_temp_c_;
    health.metrics["runtime_hours"] = total_runtime_ms_ / 3600000.0f;
    health.metrics["compressor_starts"] = compressor_starts_;

    return health;
}

bool HeatPumpModule::initialize() {
    Serial.println("[HeatPump] Initializing...");
    state_ = ModuleState::INITIALIZING;

    HAL& hal = HAL::get_instance();

    // Configure compressor relay
    hal.pin_mode(config_.compressor_relay_pin, PinMode::OUTPUT);
    hal.digital_write(config_.compressor_relay_pin, false);

    // Configure reversing valve if used
    if (config_.reversing_valve_pin > 0) {
        hal.pin_mode(config_.reversing_valve_pin, PinMode::OUTPUT);
        set_reversing_valve(true);  // Default to heating
    }

    // Configure defrost relay if used
    if (config_.defrost_relay_pin > 0) {
        hal.pin_mode(config_.defrost_relay_pin, PinMode::OUTPUT);
        hal.digital_write(config_.defrost_relay_pin, false);
    }

    // Initialize state
    mode_ = HeatPumpMode::STANDBY;
    state_ = HeatPumpState::STOPPED;
    state_start_time_ms_ = millis();
    last_stop_time_ms_ = millis();
    init_time_ms_ = millis();

    Serial.printf("[HeatPump] Initialized (compressor pin=%d)\n",
                 config_.compressor_relay_pin);

    state_ = ModuleState::RUNNING;
    return true;
}

void HeatPumpModule::shutdown() {
    Serial.println("[HeatPump] Shutting down...");

    // Stop compressor
    set_compressor(false);

    mode_ = HeatPumpMode::OFF;
    state_ = HeatPumpState::STOPPED;

    state_ = ModuleState::SHUTDOWN;
    Serial.println("[HeatPump] Shutdown complete");
}

void HeatPumpModule::update() {
    if (state_ != ModuleState::RUNNING) {
        return;
    }

    // Read temperatures
    read_temperatures();

    // Calculate COP if running
    if (state_ == HeatPumpState::RUNNING) {
        calculate_cop();
    }

    // Update state machine
    update_state_machine();
}

bool HeatPumpModule::set_mode(HeatPumpMode mode) {
    if (!config_.enabled) {
        Serial.println("[HeatPump] WARNING: Heat pump is disabled");
        return false;
    }

    // Don't allow mode changes during defrost
    if (state_ == HeatPumpState::DEFROSTING) {
        Serial.println("[HeatPump] WARNING: Cannot change mode during defrost");
        return false;
    }

    mode_ = mode;

    // Set reversing valve based on mode
    if (config_.reversing_valve_pin > 0) {
        set_reversing_valve(mode == HeatPumpMode::HEATING);
    }

    Serial.printf("[HeatPump] Mode set to %d\n", static_cast<int>(mode));

    return true;
}

HeatPumpStatus HeatPumpModule::get_status() const {
    HeatPumpStatus status;
    status.mode = mode_;
    status.state = state_;
    status.supply_temp_c = supply_temp_c_;
    status.return_temp_c = return_temp_c_;
    status.outdoor_temp_c = outdoor_temp_c_;
    status.delta_t_c = supply_temp_c_ - return_temp_c_;
    status.current_cop = current_cop_;
    status.runtime_seconds = total_runtime_ms_ / 1000;
    status.total_runtime_hours = total_runtime_ms_ / 3600000;
    status.compressor_starts = compressor_starts_;
    status.defrost_cycles = defrost_cycles_;
    status.has_error = has_error_;
    status.error_message = error_message_;

    // Calculate time until next action
    uint32_t now = millis();
    if (state_ == HeatPumpState::STOPPED && mode_ != HeatPumpMode::OFF) {
        uint32_t elapsed = now - last_stop_time_ms_;
        if (elapsed < config_.min_off_time_ms) {
            status.time_until_next_action_ms = config_.min_off_time_ms - elapsed;
        }
    } else if (state_ == HeatPumpState::RUNNING) {
        uint32_t elapsed = now - last_start_time_ms_;
        if (elapsed < config_.min_on_time_ms) {
            status.time_until_next_action_ms = config_.min_on_time_ms - elapsed;
        }
    }

    return status;
}

bool HeatPumpModule::start_defrost() {
    if (state_ == HeatPumpState::RUNNING) {
        enter_defrost_mode();
        return true;
    }
    return false;
}

void HeatPumpModule::update_state_machine() {
    HeatPumpState previous_state = state_;
    uint32_t now = millis();

    // Check safety limits
    if (!check_safety_limits()) {
        if (state_ == HeatPumpState::RUNNING || state_ == HeatPumpState::DEFROSTING) {
            set_compressor(false);
            state_ = HeatPumpState::ERROR;
        }
        return;
    }

    switch (state_) {
        case HeatPumpState::STOPPED:
            // Check if should start
            if (mode_ == HeatPumpMode::HEATING || mode_ == HeatPumpMode::COOLING) {
                // Check protection
                if (!check_protection()) {
                    break;
                }

                // Check startup delay
                if (now - init_time_ms_ < config_.startup_delay_ms) {
                    break;
                }

                // Check temperature demand
                bool need_heating = (mode_ == HeatPumpMode::HEATING &&
                                   supply_temp_c_ < target_supply_temp_c_ - 2.0);
                bool need_cooling = (mode_ == HeatPumpMode::COOLING &&
                                   supply_temp_c_ > target_supply_temp_c_ + 2.0);

                if (need_heating || need_cooling) {
                    state_ = HeatPumpState::STARTING;
                    state_start_time_ms_ = now;
                }
            }
            break;

        case HeatPumpState::STARTING:
            // Brief starting delay (could be used for pre-circulation, etc.)
            if (now - state_start_time_ms_ > 5000) {
                state_ = HeatPumpState::RUNNING;
                state_start_time_ms_ = now;
                last_start_time_ms_ = now;
                compressor_starts_++;
                set_compressor(true);
            }
            break;

        case HeatPumpState::RUNNING:
            // Update runtime
            total_runtime_ms_ += (now - state_start_time_ms_);
            state_start_time_ms_ = now;

            // Check if defrost is needed
            if (is_defrost_needed()) {
                enter_defrost_mode();
                break;
            }

            // Check if should stop
            bool stop_heating = (mode_ == HeatPumpMode::HEATING &&
                               supply_temp_c_ > target_supply_temp_c_ + 2.0);
            bool stop_cooling = (mode_ == HeatPumpMode::COOLING &&
                               supply_temp_c_ < target_supply_temp_c_ - 2.0);
            bool stop_requested = (mode_ == HeatPumpMode::OFF || mode_ == HeatPumpMode::STANDBY);

            if ((stop_heating || stop_cooling || stop_requested) &&
                now - last_start_time_ms_ >= config_.min_on_time_ms) {
                state_ = HeatPumpState::STOPPING;
                state_start_time_ms_ = now;
            }
            break;

        case HeatPumpState::STOPPING:
            // Brief stopping delay
            if (now - state_start_time_ms_ > 2000) {
                set_compressor(false);
                state_ = HeatPumpState::STOPPED;
                state_start_time_ms_ = now;
                last_stop_time_ms_ = now;
            }
            break;

        case HeatPumpState::DEFROSTING:
            // Check if defrost complete
            if (now - state_start_time_ms_ >= config_.defrost_duration_ms) {
                exit_defrost_mode();
            }
            break;

        case HeatPumpState::ERROR:
            // Error must be cleared manually
            set_compressor(false);
            break;
    }

    // Publish event if state changed
    if (state_ != previous_state) {
        publish_state_event(previous_state);
    }
}

void HeatPumpModule::read_temperatures() {
    // Get temperature sensor module
    Skeleton& system = Skeleton::get_instance();
    TemperatureSensorModule* temp_module =
        static_cast<TemperatureSensorModule*>(system.get_module("TemperatureSensorModule"));

    if (!temp_module) {
        Serial.println("[HeatPump] ERROR: Temperature sensor module not found");
        return;
    }

    // Read temperatures
    supply_temp_c_ = temp_module->get_temperature(config_.supply_temp_sensor);
    return_temp_c_ = temp_module->get_temperature(config_.return_temp_sensor);
    outdoor_temp_c_ = temp_module->get_temperature(config_.outdoor_temp_sensor);

    if (!config_.evaporator_temp_sensor.isEmpty()) {
        evaporator_temp_c_ = temp_module->get_temperature(config_.evaporator_temp_sensor);
    }
}

void HeatPumpModule::calculate_cop() {
    // COP = Heat Output / Electrical Input
    // Simplified calculation: COP ≈ ΔT_water * flow_rate / (assumed power input)
    // For now, use a simplified model based on outdoor temperature and delta-T

    if (isnan(supply_temp_c_) || isnan(return_temp_c_) || isnan(outdoor_temp_c_)) {
        current_cop_ = 0.0;
        return;
    }

    float delta_t_water = supply_temp_c_ - return_temp_c_;

    // Simplified COP model
    // COP decreases as outdoor temp decreases and as delta-T increases
    float outdoor_factor = 1.0 + (outdoor_temp_c_ / 50.0);  // 0.5 to 1.5
    float delta_t_factor = 1.0 - (delta_t_water / 30.0);    // 0.5 to 1.0

    current_cop_ = config_.nominal_cop * outdoor_factor * delta_t_factor;

    // Clamp to reasonable range
    if (current_cop_ < 1.0) current_cop_ = 1.0;
    if (current_cop_ > 6.0) current_cop_ = 6.0;

    // Add to history
    cop_history_.push_back(current_cop_);
    if (cop_history_.size() > COP_HISTORY_SIZE) {
        cop_history_.erase(cop_history_.begin());
    }

    // Publish COP update
    ValueEventData event;
    event.source_module = "HeatPumpModule";
    event.parameter_name = "cop";
    event.value = current_cop_;
    event.previous_value = cop_history_.size() > 1 ? cop_history_[cop_history_.size()-2] : 0.0;
    event.units = "";
    EventBus::get_instance().publish(EventType::COP_UPDATED, &event);
}

bool HeatPumpModule::is_defrost_needed() {
    if (mode_ != HeatPumpMode::HEATING) {
        return false;  // Only defrost in heating mode
    }

    if (outdoor_temp_c_ > config_.defrost_enable_temp_c) {
        return false;  // Too warm for frost
    }

    uint32_t now = millis();

    switch (config_.defrost_strategy) {
        case DefrostStrategy::TIME_BASED:
            // Defrost based on time interval
            return (now - last_defrost_time_ms_ >= config_.defrost_interval_ms);

        case DefrostStrategy::DEMAND_BASED:
            // Defrost when COP drops
            return (current_cop_ < config_.defrost_cop_threshold);

        case DefrostStrategy::HYBRID:
            // Defrost if either condition is met
            return ((now - last_defrost_time_ms_ >= config_.defrost_interval_ms) ||
                   (current_cop_ < config_.defrost_cop_threshold));
    }

    return false;
}

void HeatPumpModule::enter_defrost_mode() {
    Serial.println("[HeatPump] Entering defrost mode");

    state_ = HeatPumpState::DEFROSTING;
    state_start_time_ms_ = millis();
    defrost_cycles_++;

    // Activate defrost relay if configured
    if (config_.defrost_relay_pin > 0) {
        HAL::get_instance().digital_write(config_.defrost_relay_pin, true);
    }

    // Publish event
    EventData event;
    event.source_module = "HeatPumpModule";
    EventBus::get_instance().publish(EventType::DEFROST_STARTED, &event);
}

void HeatPumpModule::exit_defrost_mode() {
    Serial.println("[HeatPump] Exiting defrost mode");

    // Deactivate defrost relay
    if (config_.defrost_relay_pin > 0) {
        HAL::get_instance().digital_write(config_.defrost_relay_pin, false);
    }

    last_defrost_time_ms_ = millis();
    state_ = HeatPumpState::RUNNING;
    state_start_time_ms_ = millis();

    // Publish event
    EventData event;
    event.source_module = "HeatPumpModule";
    EventBus::get_instance().publish(EventType::DEFROST_COMPLETED, &event);
}

bool HeatPumpModule::check_protection() {
    uint32_t now = millis();

    // Check minimum off-time
    if (now - last_stop_time_ms_ < config_.min_off_time_ms) {
        return false;
    }

    // Check for error state
    if (has_error_) {
        return false;
    }

    return true;
}

bool HeatPumpModule::check_safety_limits() {
    // Check supply temperature
    if (supply_temp_c_ > config_.max_supply_temp_c) {
        has_error_ = true;
        error_message_ = "Supply temperature too high: " + String(supply_temp_c_) + "°C";

        SafetyViolation violation;
        violation.type = ViolationType::OVERTEMPERATURE;
        violation.level = SafetyLevel::CRITICAL;
        violation.module_name = "HeatPumpModule";
        violation.description = error_message_;
        violation.value = supply_temp_c_;
        violation.threshold = config_.max_supply_temp_c;
        violation.timestamp_ms = millis();
        violation.acknowledged = false;
        violation.auto_recovery_attempted = false;
        SafetyEngine::get_instance().report_violation(violation);

        return false;
    }

    // Check outdoor temperature
    if (outdoor_temp_c_ < config_.min_outdoor_temp_c) {
        has_error_ = true;
        error_message_ = "Outdoor temperature too low: " + String(outdoor_temp_c_) + "°C";

        SafetyViolation violation;
        violation.type = ViolationType::UNDERTEMPERATURE;
        violation.level = SafetyLevel::WARNING;
        violation.module_name = "HeatPumpModule";
        violation.description = error_message_;
        violation.value = outdoor_temp_c_;
        violation.threshold = config_.min_outdoor_temp_c;
        violation.timestamp_ms = millis();
        violation.acknowledged = false;
        violation.auto_recovery_attempted = false;
        SafetyEngine::get_instance().report_violation(violation);

        return false;
    }

    // Clear error if conditions are now OK
    if (has_error_) {
        has_error_ = false;
        error_message_ = "";
    }

    return true;
}

void HeatPumpModule::set_compressor(bool on) {
    HAL& hal = HAL::get_instance();
    hal.digital_write(config_.compressor_relay_pin, on);

    Serial.printf("[HeatPump] Compressor %s\n", on ? "ON" : "OFF");
}

void HeatPumpModule::set_reversing_valve(bool heating) {
    if (config_.reversing_valve_pin > 0) {
        HAL& hal = HAL::get_instance();
        hal.digital_write(config_.reversing_valve_pin, heating);
    }
}

void HeatPumpModule::publish_state_event(HeatPumpState previous_state) {
    StateChangeEventData event;
    event.source_module = "HeatPumpModule";
    event.component_id = "compressor";
    event.previous_state = String(static_cast<int>(previous_state));
    event.new_state = String(static_cast<int>(state_));
    event.reason = "State transition";

    EventBus::get_instance().publish(EventType::COMPRESSOR_STATE_CHANGED, &event);

    Serial.printf("[HeatPump] State: %d -> %d\n",
                 static_cast<int>(previous_state),
                 static_cast<int>(state_));
}
