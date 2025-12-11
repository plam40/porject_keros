/**
 * @file SafetyEngine.h
 * @purpose Safety monitoring, fault detection, and emergency shutdown system
 * @dependencies ModuleInterface.h, EventBus.h
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes Safety checks run every 100ms, complete in <10ms typically
 * @module_type SKELETON
 *
 * The SafetyEngine continuously monitors all modules, hardware, and system parameters
 * to ensure safe operation. It can trigger emergency shutdowns, send alerts, and
 * initiate recovery procedures when faults are detected.
 *
 * Safety Levels:
 * - LEVEL_0: Normal operation
 * - LEVEL_1: Minor fault, log warning
 * - LEVEL_2: Moderate fault, reduce functionality
 * - LEVEL_3: Severe fault, initiate graceful shutdown
 * - LEVEL_4: Critical fault, immediate emergency shutdown
 */

#ifndef SAFETY_ENGINE_H
#define SAFETY_ENGINE_H

#include <Arduino.h>
#include "ModuleInterface.h"
#include "EventBus.h"
#include <vector>
#include <map>

/**
 * Safety violation types
 */
enum class ViolationType {
    // Temperature violations
    OVERTEMPERATURE,
    UNDERTEMPERATURE,
    TEMPERATURE_RATE_EXCEEDED,
    TEMPERATURE_SENSOR_FAILURE,

    // Pressure violations
    OVERPRESSURE,
    UNDERPRESSURE,
    PRESSURE_SENSOR_FAILURE,

    // Electrical violations
    OVERCURRENT,
    OVERVOLTAGE,
    UNDERVOLTAGE,
    GROUND_FAULT,

    // Module health violations
    MODULE_UNRESPONSIVE,
    MODULE_HEALTH_DEGRADED,
    MODULE_INIT_FAILED,
    MODULE_CRASHED,

    // System violations
    WATCHDOG_TIMEOUT,
    MEMORY_EXHAUSTION,
    TASK_OVERRUN,
    STACK_OVERFLOW,

    // Hardware violations
    HARDWARE_FAULT,
    SENSOR_DISCONNECTED,
    ACTUATOR_STUCK,

    // Configuration violations
    INVALID_CONFIGURATION,
    CONFIGURATION_MISMATCH
};

/**
 * Safety levels
 */
enum class SafetyLevel {
    NORMAL = 0,         // Normal operation
    WARNING = 1,        // Minor fault, continue with logging
    DEGRADED = 2,       // Moderate fault, reduce functionality
    CRITICAL = 3,       // Severe fault, initiate graceful shutdown
    EMERGENCY = 4       // Critical fault, immediate shutdown
};

/**
 * Safety violation record
 */
struct SafetyViolation {
    ViolationType type;
    SafetyLevel level;
    String module_name;
    String description;
    float value;                // Violation value (e.g., temperature)
    float threshold;            // Threshold that was exceeded
    uint32_t timestamp_ms;
    bool acknowledged;          // Has operator acknowledged this violation?
    bool auto_recovery_attempted;
};

/**
 * Safety limits configuration
 */
struct SafetyLimits {
    // Temperature limits (°C)
    float max_supply_temp = 65.0;
    float max_return_temp = 50.0;
    float max_tank_temp = 80.0;
    float min_outdoor_temp = -30.0;
    float max_temp_rate_per_min = 10.0;

    // Pressure limits (bar)
    float max_system_pressure = 3.0;
    float min_system_pressure = 0.5;

    // Current limits (A)
    float max_compressor_current = 15.0;
    float max_pump_current = 5.0;
    float max_total_current = 25.0;

    // Voltage limits (V)
    float max_voltage = 253.0;
    float min_voltage = 207.0;

    // Module health
    uint8_t min_module_health_percent = 50;
    uint32_t max_module_unresponsive_ms = 5000;

    // Watchdog timeout (ms)
    uint32_t watchdog_timeout_ms = 10000;
};

/**
 * @class SafetyEngine
 * @brief Monitors system safety and initiates emergency procedures
 *
 * The SafetyEngine runs in a high-priority FreeRTOS task and continuously
 * monitors all aspects of the HVAC system for safety violations. When a
 * violation is detected, it takes appropriate action based on the severity.
 *
 * Actions by Safety Level:
 * - WARNING: Log violation, send notification
 * - DEGRADED: Reduce system capacity, disable non-essential features
 * - CRITICAL: Initiate graceful shutdown with 30s timeout
 * - EMERGENCY: Immediate hardware shutdown via safety relays
 *
 * Hardware Safety Features:
 * - Independent watchdog timer (hardware timer)
 * - Safety relay for emergency power cutoff
 * - Redundant temperature sensors for critical measurements
 * - Pressure relief valves (hardware level)
 *
 * Usage Example:
 * SafetyEngine& safety = SafetyEngine::get_instance();
 * safety.set_limits(custom_limits);
 * safety.register_module(&my_module);
 * safety.start_monitoring();
 */
class SafetyEngine {
public:
    /**
     * Get singleton instance
     * @return Reference to global SafetyEngine instance
     */
    static SafetyEngine& get_instance() {
        static SafetyEngine instance;
        return instance;
    }

    /**
     * Initialize safety engine
     * @return true if initialization successful
     */
    bool initialize();

    /**
     * Start safety monitoring task
     * @return true if task started successfully
     */
    bool start_monitoring();

    /**
     * Stop safety monitoring task
     */
    void stop_monitoring();

    /**
     * Register a module for health monitoring
     * @param module Pointer to module
     * @return true if registered successfully
     */
    bool register_module(ModuleInterface* module);

    /**
     * Unregister a module
     * @param module Pointer to module
     */
    void unregister_module(ModuleInterface* module);

    /**
     * Set safety limits
     * @param limits Safety limits configuration
     */
    void set_limits(const SafetyLimits& limits) { limits_ = limits; }

    /**
     * Get current safety limits
     * @return Current safety limits
     */
    const SafetyLimits& get_limits() const { return limits_; }

    /**
     * Report a safety violation
     * @param violation Violation details
     * @note Can be called by any module to report safety issues
     */
    void report_violation(const SafetyViolation& violation);

    /**
     * Get all active violations
     * @return Vector of unacknowledged violations
     */
    std::vector<SafetyViolation> get_active_violations() const;

    /**
     * Acknowledge a violation
     * @param timestamp_ms Timestamp of violation to acknowledge
     * @return true if acknowledged successfully
     */
    bool acknowledge_violation(uint32_t timestamp_ms);

    /**
     * Get current system safety level
     * @return Highest safety level among all violations
     */
    SafetyLevel get_current_safety_level() const;

    /**
     * Check if system is safe to operate
     * @return true if no critical or emergency violations exist
     */
    bool is_safe_to_operate() const;

    /**
     * Initiate emergency shutdown
     * @param reason Reason for shutdown
     * @note Immediately stops all actuators and sets safe state
     */
    void emergency_shutdown(const String& reason);

    /**
     * Reset safety engine after shutdown
     * @return true if reset successful and safe to restart
     */
    bool reset();

    /**
     * Feed the watchdog timer
     * @note Must be called at least every watchdog_timeout_ms
     */
    void feed_watchdog();

    /**
     * Enable/disable auto-recovery
     * @param enable true to enable auto-recovery from warnings
     */
    void set_auto_recovery(bool enable) { auto_recovery_enabled_ = enable; }

    // Prevent copying
    SafetyEngine(const SafetyEngine&) = delete;
    SafetyEngine& operator=(const SafetyEngine&) = delete;

private:
    SafetyEngine() : monitoring_active_(false), auto_recovery_enabled_(true),
                     in_emergency_shutdown_(false) {}

    /**
     * Safety monitoring task (runs in FreeRTOS task)
     */
    void monitoring_task();

    /**
     * Check all modules health
     */
    void check_modules_health();

    /**
     * Check hardware watchdog
     */
    void check_watchdog();

    /**
     * Attempt automatic recovery from violation
     * @param violation Violation to recover from
     * @return true if recovery successful
     */
    bool attempt_recovery(SafetyViolation& violation);

    /**
     * Set all hardware to safe state
     */
    void set_safe_state();

    // Registered modules
    std::vector<ModuleInterface*> modules_;

    // Safety limits
    SafetyLimits limits_;

    // Violation history
    std::vector<SafetyViolation> violations_;

    // Monitoring state
    bool monitoring_active_;
    bool auto_recovery_enabled_;
    bool in_emergency_shutdown_;

    // Watchdog state
    uint32_t last_watchdog_feed_ms_;

    // FreeRTOS task handle
    TaskHandle_t monitoring_task_handle_;

    // Emergency shutdown GPIO pin
    static const int EMERGENCY_SHUTDOWN_PIN = 25;
};

#endif // SAFETY_ENGINE_H
