/**
 * @file PumpModule.h
 * @purpose Circulation pump control module with variable speed control
 * @dependencies ModuleInterface.h, HAL.h, EventBus.h
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes PWM update: <1ms, Current monitoring: ~20μs
 * @module_type MODULE
 *
 * This module manages all circulation pumps in the HVAC system. It provides:
 * - Variable speed control via PWM (0-100%)
 * - Current monitoring for fault detection
 * - Minimum runtime protection
 * - Dry run protection
 * - Locked rotor detection
 * - Multiple control modes (manual, auto, off)
 *
 * Pump Protection Features:
 * - Minimum on-time: Prevents short cycling
 * - Minimum off-time: Thermal protection
 * - Current monitoring: Detects locked rotor or dry running
 * - Soft start: Gradual speed ramp to reduce mechanical stress
 */

#ifndef PUMP_MODULE_H
#define PUMP_MODULE_H

#include "ModuleInterface.h"
#include "HAL.h"
#include "EventBus.h"
#include <map>
#include <vector>

/**
 * Pump control modes
 */
enum class PumpMode {
    OFF,            // Pump is off
    MANUAL,         // Manual speed control
    AUTO,           // Automatic speed based on system demand
    SCHEDULED       // Time-based schedule
};

/**
 * Pump state
 */
enum class PumpState {
    STOPPED,        // Pump is stopped
    STARTING,       // Pump is starting (soft start)
    RUNNING,        // Pump is running
    STOPPING,       // Pump is stopping
    ERROR           // Pump has an error
};

/**
 * Pump configuration
 */
struct PumpConfig {
    String name;                    // Pump name (e.g., "primary", "zone1")
    uint8_t pwm_pin;                // PWM output pin
    uint8_t pwm_channel;            // PWM channel (0-15)
    uint8_t current_sense_pin;      // Current sense ADC pin (0 = no monitoring)

    // Speed limits
    uint8_t min_speed_percent;      // Minimum speed (0-100%)
    uint8_t max_speed_percent;      // Maximum speed (0-100%)

    // Timing protection
    uint32_t min_on_time_ms;        // Minimum on time (ms)
    uint32_t min_off_time_ms;       // Minimum off time (ms)
    uint32_t soft_start_time_ms;    // Soft start duration (ms)

    // Current monitoring
    float max_current_amps;         // Maximum allowed current
    float min_current_amps;         // Minimum current (dry run detection)
    float current_calibration;      // ADC to Amps conversion factor

    bool enabled;                   // Is pump enabled?
};

/**
 * Pump status
 */
struct PumpStatus {
    String name;
    PumpMode mode;
    PumpState state;
    uint8_t target_speed_percent;
    uint8_t actual_speed_percent;
    float current_amps;
    uint32_t runtime_seconds;
    uint32_t total_runtime_hours;
    uint32_t start_count;
    bool has_error;
    String error_message;
};

/**
 * @class PumpModule
 * @brief Manages circulation pumps with variable speed control
 *
 * This module provides comprehensive pump control with protection features.
 * Supports multiple pumps with independent control and monitoring.
 *
 * Configuration Example:
 * pumps:
 *   primary:
 *     pwm_pin: 26
 *     pwm_channel: 0
 *     current_sense_pin: 35
 *     min_speed_percent: 30
 *     max_speed_percent: 100
 *     min_on_time_ms: 60000
 *     min_off_time_ms: 30000
 *
 * Usage Example:
 * PumpModule pump_module;
 * system.register_module(&pump_module, 50);
 *
 * // Set pump speed
 * pump_module.set_pump_speed("primary", 75);  // 75% speed
 *
 * // Get status
 * PumpStatus status = pump_module.get_pump_status("primary");
 */
class PumpModule : public ModuleInterface {
public:
    PumpModule();
    ~PumpModule() override = default;

    // ModuleInterface implementation
    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override { return "1.0"; }

    /**
     * Update all pumps (called periodically)
     */
    void update();

    /**
     * Add pump configuration
     * @param config Pump configuration
     * @return true if added successfully
     */
    bool add_pump(const PumpConfig& config);

    /**
     * Set pump mode
     * @param name Pump name
     * @param mode Control mode
     * @return true if successful
     */
    bool set_pump_mode(const String& name, PumpMode mode);

    /**
     * Set pump speed (for MANUAL mode)
     * @param name Pump name
     * @param speed_percent Speed percentage (0-100)
     * @return true if successful
     */
    bool set_pump_speed(const String& name, uint8_t speed_percent);

    /**
     * Start pump
     * @param name Pump name
     * @return true if successful
     */
    bool start_pump(const String& name);

    /**
     * Stop pump
     * @param name Pump name
     * @return true if successful
     */
    bool stop_pump(const String& name);

    /**
     * Get pump status
     * @param name Pump name
     * @return Pump status
     */
    PumpStatus get_pump_status(const String& name) const;

    /**
     * Get all pump statuses
     * @return Map of pump name to status
     */
    std::map<String, PumpStatus> get_all_pump_status() const;

    /**
     * Enable/disable a pump
     * @param name Pump name
     * @param enabled true to enable
     */
    void set_pump_enabled(const String& name, bool enabled);

private:
    /**
     * Pump instance data
     */
    struct Pump {
        PumpConfig config;
        PumpMode mode;
        PumpState state;
        uint8_t target_speed;
        uint8_t current_speed;
        uint32_t state_start_time_ms;
        uint32_t last_start_time_ms;
        uint32_t last_stop_time_ms;
        uint32_t total_runtime_ms;
        uint32_t start_count;
        float current_amps;
        bool has_error;
        String error_message;
    };

    /**
     * Update single pump
     */
    void update_pump(Pump& pump);

    /**
     * Update pump speed with soft start
     */
    void update_pump_speed(Pump& pump);

    /**
     * Monitor pump current
     */
    void monitor_pump_current(Pump& pump);

    /**
     * Check pump protection conditions
     */
    bool check_pump_protection(Pump& pump);

    /**
     * Set hardware PWM output
     */
    void set_pwm_output(Pump& pump, uint8_t speed_percent);

    /**
     * Publish pump state change event
     */
    void publish_pump_event(const Pump& pump, PumpState previous_state);

    // Pump instances
    std::map<String, Pump> pumps_;

    // Timing
    uint32_t last_update_ms_;
};

#endif // PUMP_MODULE_H
