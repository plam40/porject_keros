/**
 * @file HeatPumpModule.h
 * @purpose Heat pump control with compressor management, COP optimization, and defrost
 * @dependencies ModuleInterface.h, HAL.h, EventBus.h
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes COP calculation: <100μs, Defrost decision: ~1ms
 * @module_type MODULE
 *
 * This module manages heat pump operation including:
 * - Compressor control with protection timers
 * - COP (Coefficient of Performance) monitoring and optimization
 * - Automatic defrost cycle management
 * - Multiple operating modes (heating, cooling, off)
 * - Safety limit enforcement
 * - Outdoor temperature compensation
 *
 * Heat Pump Protection Features:
 * - Minimum on-time: Prevents short cycling (typically 3-5 minutes)
 * - Minimum off-time: Compressor thermal protection (typically 3-5 minutes)
 * - Maximum temperature limits: Prevents overheating
 * - Minimum outdoor temperature: Low ambient protection
 * - Defrost management: Maintains efficiency in cold weather
 * - COP monitoring: Detects performance degradation
 */

#ifndef HEAT_PUMP_MODULE_H
#define HEAT_PUMP_MODULE_H

#include "ModuleInterface.h"
#include "HAL.h"
#include "EventBus.h"
#include <vector>

/**
 * Heat pump operating modes
 */
enum class HeatPumpMode {
    OFF,                // Heat pump is off
    HEATING,            // Heating mode
    COOLING,            // Cooling mode (if supported)
    DEFROST,            // Defrost cycle active
    STANDBY             // Standby mode (ready but not running)
};

/**
 * Heat pump state
 */
enum class HeatPumpState {
    STOPPED,            // Compressor stopped
    STARTING,           // Starting sequence
    RUNNING,            // Compressor running
    STOPPING,           // Stopping sequence
    DEFROSTING,         // Defrost cycle
    ERROR               // Error state
};

/**
 * Defrost strategy
 */
enum class DefrostStrategy {
    TIME_BASED,         // Defrost based on runtime and temperature
    DEMAND_BASED,       // Defrost when COP degrades
    HYBRID              // Combination of time and demand
};

/**
 * Heat pump configuration
 */
struct HeatPumpConfig {
    // GPIO pins
    uint8_t compressor_relay_pin;       // Compressor relay output
    uint8_t reversing_valve_pin;        // Reversing valve (0 = not used)
    uint8_t defrost_relay_pin;          // Defrost relay (0 = not used)

    // Temperature sensor names (from TemperatureSensorModule)
    String supply_temp_sensor;          // Supply temperature sensor
    String return_temp_sensor;          // Return temperature sensor
    String outdoor_temp_sensor;         // Outdoor temperature sensor
    String evaporator_temp_sensor;      // Evaporator temperature (optional)

    // Operating limits
    float max_supply_temp_c;            // Maximum supply temperature
    float min_supply_temp_c;            // Minimum supply temperature
    float max_delta_t_c;                // Maximum temperature differential
    float min_outdoor_temp_c;           // Minimum outdoor temperature for operation

    // Timing protection
    uint32_t min_on_time_ms;            // Minimum compressor on time
    uint32_t min_off_time_ms;           // Minimum compressor off time
    uint32_t startup_delay_ms;          // Startup delay after power on

    // Defrost configuration
    DefrostStrategy defrost_strategy;
    float defrost_enable_temp_c;        // Enable defrost below this outdoor temp
    uint32_t defrost_interval_ms;       // Time-based defrost interval
    uint32_t defrost_duration_ms;       // Defrost cycle duration
    float defrost_cop_threshold;        // COP below which to trigger defrost

    // COP monitoring
    float min_cop;                      // Minimum acceptable COP
    float nominal_cop;                  // Nominal (expected) COP

    bool enabled;                       // Is heat pump enabled?
};

/**
 * Heat pump status
 */
struct HeatPumpStatus {
    HeatPumpMode mode;
    HeatPumpState state;
    float supply_temp_c;
    float return_temp_c;
    float outdoor_temp_c;
    float delta_t_c;
    float current_cop;
    uint32_t runtime_seconds;
    uint32_t total_runtime_hours;
    uint32_t compressor_starts;
    uint32_t defrost_cycles;
    uint32_t time_until_next_action_ms;
    bool has_error;
    String error_message;
};

/**
 * @class HeatPumpModule
 * @brief Manages heat pump operation with COP optimization and defrost
 *
 * This module provides comprehensive heat pump control with advanced features
 * like COP monitoring, automatic defrost, and outdoor temperature compensation.
 *
 * Configuration Example:
 * heat_pump:
 *   compressor_relay_pin: 27
 *   supply_temp_sensor: "supply"
 *   return_temp_sensor: "return"
 *   outdoor_temp_sensor: "outdoor"
 *   max_supply_temp_c: 65.0
 *   min_outdoor_temp_c: -15.0
 *   min_on_time_ms: 300000
 *   min_off_time_ms: 300000
 *   defrost_enable_temp_c: 5.0
 *
 * Usage Example:
 * HeatPumpModule heat_pump;
 * system.register_module(&heat_pump, 100);
 *
 * // Start heating
 * heat_pump.set_mode(HeatPumpMode::HEATING);
 * heat_pump.set_target_temperature(55.0);
 */
class HeatPumpModule : public ModuleInterface {
public:
    HeatPumpModule();
    ~HeatPumpModule() override = default;

    // ModuleInterface implementation
    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override { return "1.0"; }

    /**
     * Update heat pump (called periodically)
     */
    void update();

    /**
     * Set heat pump configuration
     */
    void set_config(const HeatPumpConfig& config) { config_ = config; }

    /**
     * Set operating mode
     */
    bool set_mode(HeatPumpMode mode);

    /**
     * Set target supply temperature
     */
    void set_target_temperature(float temp_c) { target_supply_temp_c_ = temp_c; }

    /**
     * Get current status
     */
    HeatPumpStatus get_status() const;

    /**
     * Get current COP
     */
    float get_cop() const { return current_cop_; }

    /**
     * Force defrost cycle
     */
    bool start_defrost();

    /**
     * Enable/disable heat pump
     */
    void set_enabled(bool enabled) { config_.enabled = enabled; }

private:
    /**
     * Update state machine
     */
    void update_state_machine();

    /**
     * Read all temperatures
     */
    void read_temperatures();

    /**
     * Calculate COP
     */
    void calculate_cop();

    /**
     * Check if defrost is needed
     */
    bool is_defrost_needed();

    /**
     * Start defrost cycle
     */
    void enter_defrost_mode();

    /**
     * Exit defrost cycle
     */
    void exit_defrost_mode();

    /**
     * Check protection conditions
     */
    bool check_protection();

    /**
     * Check safety limits
     */
    bool check_safety_limits();

    /**
     * Set compressor relay
     */
    void set_compressor(bool on);

    /**
     * Set reversing valve (heating/cooling)
     */
    void set_reversing_valve(bool heating);

    /**
     * Publish state change event
     */
    void publish_state_event(HeatPumpState previous_state);

    // Configuration
    HeatPumpConfig config_;

    // Current state
    HeatPumpMode mode_;
    HeatPumpState state_;
    float target_supply_temp_c_;

    // Temperature readings
    float supply_temp_c_;
    float return_temp_c_;
    float outdoor_temp_c_;
    float evaporator_temp_c_;

    // COP tracking
    float current_cop_;
    std::vector<float> cop_history_;  // Last 10 COP readings
    static const size_t COP_HISTORY_SIZE = 10;

    // Timing
    uint32_t state_start_time_ms_;
    uint32_t last_start_time_ms_;
    uint32_t last_stop_time_ms_;
    uint32_t last_defrost_time_ms_;
    uint32_t total_runtime_ms_;
    uint32_t compressor_starts_;
    uint32_t defrost_cycles_;

    // Error tracking
    bool has_error_;
    String error_message_;

    // Initialization time
    uint32_t init_time_ms_;
};

#endif // HEAT_PUMP_MODULE_H
