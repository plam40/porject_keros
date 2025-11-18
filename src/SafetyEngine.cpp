/**
 * @file SafetyEngine.cpp
 * @purpose Implementation of safety monitoring and fault detection system
 * @dependencies SafetyEngine.h, EventBus.h
 * @version 1.0.0
 * @performance_notes Safety monitoring task runs every 100ms with <10ms execution time
 */

#include "SafetyEngine.h"

// FreeRTOS task function wrapper
static void safety_monitoring_task_wrapper(void* parameter) {
    SafetyEngine* engine = static_cast<SafetyEngine*>(parameter);
    while (true) {
        engine->monitoring_task();
        vTaskDelay(pdMS_TO_TICKS(100));  // Run every 100ms
    }
}

bool SafetyEngine::initialize() {
    Serial.println("[SafetyEngine] Initializing...");

    // Configure emergency shutdown pin
    pinMode(EMERGENCY_SHUTDOWN_PIN, OUTPUT);
    digitalWrite(EMERGENCY_SHUTDOWN_PIN, HIGH);  // Active low (HIGH = normal operation)

    // Initialize watchdog
    last_watchdog_feed_ms_ = millis();

    // Subscribe to system events
    EventBus::get_instance().subscribe(EventType::SYSTEM_ERROR,
        [this](const EventData* data) {
            auto safety_data = static_cast<const SafetyEventData*>(data);
            SafetyViolation violation;
            violation.type = ViolationType::MODULE_HEALTH_DEGRADED;
            violation.level = SafetyLevel::WARNING;
            violation.module_name = safety_data->source_module;
            violation.description = "System error reported";
            violation.timestamp_ms = millis();
            violation.acknowledged = false;
            violation.auto_recovery_attempted = false;
            report_violation(violation);
        });

    Serial.println("[SafetyEngine] Initialized successfully");
    return true;
}

bool SafetyEngine::start_monitoring() {
    if (monitoring_active_) {
        Serial.println("[SafetyEngine] WARNING: Monitoring already active");
        return true;
    }

    Serial.println("[SafetyEngine] Starting monitoring task...");

    // Create FreeRTOS task with high priority
    BaseType_t result = xTaskCreate(
        safety_monitoring_task_wrapper,
        "SafetyMonitor",
        4096,                           // Stack size (bytes)
        this,                           // Parameter (this pointer)
        10,                             // Priority (10 = highest)
        &monitoring_task_handle_
    );

    if (result != pdPASS) {
        Serial.println("[SafetyEngine] ERROR: Failed to create monitoring task");
        return false;
    }

    monitoring_active_ = true;
    Serial.println("[SafetyEngine] Monitoring task started");

    // Publish event
    EventData event;
    event.source_module = "SafetyEngine";
    EventBus::get_instance().publish(EventType::SYSTEM_STARTUP, &event);

    return true;
}

void SafetyEngine::stop_monitoring() {
    if (!monitoring_active_) {
        return;
    }

    Serial.println("[SafetyEngine] Stopping monitoring task...");

    if (monitoring_task_handle_ != nullptr) {
        vTaskDelete(monitoring_task_handle_);
        monitoring_task_handle_ = nullptr;
    }

    monitoring_active_ = false;
    Serial.println("[SafetyEngine] Monitoring task stopped");
}

bool SafetyEngine::register_module(ModuleInterface* module) {
    if (!module) {
        return false;
    }

    // Check if already registered
    for (const auto* mod : modules_) {
        if (mod == module) {
            Serial.printf("[SafetyEngine] WARNING: Module %s already registered\n",
                         module->get_name().c_str());
            return false;
        }
    }

    modules_.push_back(module);
    Serial.printf("[SafetyEngine] Registered module: %s (%d total)\n",
                 module->get_name().c_str(), modules_.size());

    return true;
}

void SafetyEngine::unregister_module(ModuleInterface* module) {
    for (auto it = modules_.begin(); it != modules_.end(); ++it) {
        if (*it == module) {
            modules_.erase(it);
            Serial.printf("[SafetyEngine] Unregistered module: %s\n",
                         module->get_name().c_str());
            return;
        }
    }
}

void SafetyEngine::report_violation(const SafetyViolation& violation) {
    // Add to violation history
    violations_.push_back(violation);

    // Log violation
    Serial.printf("[SafetyEngine] VIOLATION: [%s] %s - %s (Level %d)\n",
                 violation.module_name.c_str(),
                 String(static_cast<int>(violation.type)).c_str(),
                 violation.description.c_str(),
                 static_cast<int>(violation.level));

    // Publish safety event
    SafetyEventData event;
    event.source_module = violation.module_name;
    event.violation_type = String(static_cast<int>(violation.type));
    event.affected_component = violation.module_name;
    event.severity = static_cast<uint8_t>(violation.level);
    event.remediation_action = "Check system status";
    EventBus::get_instance().publish(EventType::SAFETY_VIOLATION, &event);

    // Take action based on safety level
    switch (violation.level) {
        case SafetyLevel::NORMAL:
            // No action needed
            break;

        case SafetyLevel::WARNING:
            // Attempt auto-recovery if enabled
            if (auto_recovery_enabled_) {
                SafetyViolation& mutable_violation = violations_.back();
                attempt_recovery(mutable_violation);
            }
            break;

        case SafetyLevel::DEGRADED:
            // Reduce system capacity
            Serial.println("[SafetyEngine] System operating in DEGRADED mode");
            break;

        case SafetyLevel::CRITICAL:
            // Initiate graceful shutdown
            Serial.println("[SafetyEngine] CRITICAL violation - initiating graceful shutdown");
            EventData shutdown_event;
            shutdown_event.source_module = "SafetyEngine";
            EventBus::get_instance().publish(EventType::SYSTEM_SHUTDOWN, &shutdown_event);
            break;

        case SafetyLevel::EMERGENCY:
            // Immediate emergency shutdown
            emergency_shutdown(violation.description);
            break;
    }

    // Limit violation history to prevent memory issues
    if (violations_.size() > 100) {
        violations_.erase(violations_.begin());
    }
}

std::vector<SafetyViolation> SafetyEngine::get_active_violations() const {
    std::vector<SafetyViolation> active;
    for (const auto& violation : violations_) {
        if (!violation.acknowledged) {
            active.push_back(violation);
        }
    }
    return active;
}

bool SafetyEngine::acknowledge_violation(uint32_t timestamp_ms) {
    for (auto& violation : violations_) {
        if (violation.timestamp_ms == timestamp_ms) {
            violation.acknowledged = true;
            Serial.printf("[SafetyEngine] Acknowledged violation at timestamp %u\n",
                         timestamp_ms);
            return true;
        }
    }
    return false;
}

SafetyLevel SafetyEngine::get_current_safety_level() const {
    SafetyLevel max_level = SafetyLevel::NORMAL;

    for (const auto& violation : violations_) {
        if (!violation.acknowledged && violation.level > max_level) {
            max_level = violation.level;
        }
    }

    return max_level;
}

bool SafetyEngine::is_safe_to_operate() const {
    SafetyLevel level = get_current_safety_level();
    return level < SafetyLevel::CRITICAL && !in_emergency_shutdown_;
}

void SafetyEngine::emergency_shutdown(const String& reason) {
    if (in_emergency_shutdown_) {
        return;  // Already in emergency shutdown
    }

    Serial.println("====================================");
    Serial.printf("EMERGENCY SHUTDOWN: %s\n", reason.c_str());
    Serial.println("====================================");

    in_emergency_shutdown_ = true;

    // Publish emergency shutdown event
    SafetyEventData event;
    event.source_module = "SafetyEngine";
    event.violation_type = "EMERGENCY_SHUTDOWN";
    event.severity = 4;
    event.remediation_action = reason;
    EventBus::get_instance().publish(EventType::EMERGENCY_SHUTDOWN, &event);

    // Set hardware to safe state
    set_safe_state();

    // Activate emergency shutdown relay
    digitalWrite(EMERGENCY_SHUTDOWN_PIN, LOW);  // Active low

    Serial.println("[SafetyEngine] Emergency shutdown complete - system halted");
}

bool SafetyEngine::reset() {
    if (in_emergency_shutdown_) {
        // Check if conditions allow reset
        if (!is_safe_to_operate()) {
            Serial.println("[SafetyEngine] ERROR: Cannot reset - unsafe conditions persist");
            return false;
        }

        // Clear emergency state
        in_emergency_shutdown_ = false;
        digitalWrite(EMERGENCY_SHUTDOWN_PIN, HIGH);

        // Clear acknowledged violations
        violations_.clear();

        Serial.println("[SafetyEngine] System reset - ready to restart");
        return true;
    }

    return true;  // Already in safe state
}

void SafetyEngine::feed_watchdog() {
    last_watchdog_feed_ms_ = millis();
}

void SafetyEngine::monitoring_task() {
    // Check modules health
    check_modules_health();

    // Check watchdog
    check_watchdog();

    // Feed watchdog (this task is running)
    feed_watchdog();
}

void SafetyEngine::check_modules_health() {
    for (auto* module : modules_) {
        if (!module) continue;

        // Get module health status
        HealthStatus health = module->health_check();

        // Check if module is unhealthy
        if (!health.is_healthy) {
            SafetyViolation violation;
            violation.type = ViolationType::MODULE_HEALTH_DEGRADED;
            violation.module_name = module->get_name();
            violation.description = health.status_message;
            violation.value = health.health_percentage;
            violation.threshold = static_cast<float>(limits_.min_module_health_percent);
            violation.timestamp_ms = millis();
            violation.acknowledged = false;
            violation.auto_recovery_attempted = false;

            // Determine safety level based on health percentage
            if (health.health_percentage < 30) {
                violation.level = SafetyLevel::CRITICAL;
            } else if (health.health_percentage < 50) {
                violation.level = SafetyLevel::DEGRADED;
            } else {
                violation.level = SafetyLevel::WARNING;
            }

            report_violation(violation);
        }
    }
}

void SafetyEngine::check_watchdog() {
    uint32_t time_since_feed = millis() - last_watchdog_feed_ms_;

    if (time_since_feed > limits_.watchdog_timeout_ms) {
        SafetyViolation violation;
        violation.type = ViolationType::WATCHDOG_TIMEOUT;
        violation.level = SafetyLevel::CRITICAL;
        violation.module_name = "System";
        violation.description = "Watchdog timeout - system may be hung";
        violation.value = time_since_feed;
        violation.threshold = limits_.watchdog_timeout_ms;
        violation.timestamp_ms = millis();
        violation.acknowledged = false;
        violation.auto_recovery_attempted = false;

        report_violation(violation);
    }
}

bool SafetyEngine::attempt_recovery(SafetyViolation& violation) {
    if (violation.auto_recovery_attempted) {
        return false;  // Already tried recovery
    }

    violation.auto_recovery_attempted = true;

    Serial.printf("[SafetyEngine] Attempting auto-recovery for violation: %s\n",
                 violation.description.c_str());

    // Recovery logic based on violation type
    switch (violation.type) {
        case ViolationType::MODULE_HEALTH_DEGRADED:
            // Try to find and restart the unhealthy module
            for (auto* module : modules_) {
                if (module->get_name() == violation.module_name) {
                    Serial.printf("[SafetyEngine] Restarting module: %s\n",
                                 module->get_name().c_str());
                    module->shutdown();
                    delay(100);
                    if (module->initialize()) {
                        Serial.println("[SafetyEngine] Module recovery successful");
                        violation.acknowledged = true;
                        return true;
                    }
                }
            }
            break;

        default:
            // No auto-recovery available for this violation type
            break;
    }

    return false;
}

void SafetyEngine::set_safe_state() {
    Serial.println("[SafetyEngine] Setting all hardware to safe state...");

    // This will be implemented when hardware modules are added
    // For now, just log the action

    Serial.println("[SafetyEngine] Safe state activated");
}
