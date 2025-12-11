/**
 * @file VentilationModule.h
 * @purpose Ventilation control for HRV/ERV systems
 * @dependencies ModuleInterface.h, HAL.h
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes Fan speed update: <1ms
 * @module_type MODULE
 *
 * Controls ventilation fans with variable speed control.
 * Supports fresh air intake and exhaust with optional heat recovery.
 */

#ifndef VENTILATION_MODULE_H
#define VENTILATION_MODULE_H

#include "ModuleInterface.h"
#include "HAL.h"
#include "EventBus.h"

enum class VentilationMode {
    OFF,
    AUTO,
    LOW,
    MEDIUM,
    HIGH,
    BOOST
};

struct VentilationConfig {
    uint8_t supply_fan_pin;
    uint8_t supply_fan_channel;
    uint8_t exhaust_fan_pin;
    uint8_t exhaust_fan_channel;
    uint8_t min_speed_percent;
    uint8_t max_speed_percent;
    bool enabled;
};

struct VentilationStatus {
    VentilationMode mode;
    uint8_t supply_speed_percent;
    uint8_t exhaust_speed_percent;
    uint32_t runtime_hours;
    bool has_error;
    String error_message;
};

class VentilationModule : public ModuleInterface {
public:
    VentilationModule();
    ~VentilationModule() override = default;

    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override { return "1.0"; }

    void update();
    bool set_mode(VentilationMode mode);
    bool set_fan_speed(uint8_t supply_percent, uint8_t exhaust_percent);
    VentilationStatus get_status() const;
    void set_config(const VentilationConfig& config) { config_ = config; }

private:
    VentilationConfig config_;
    VentilationMode mode_;
    uint8_t supply_speed_;
    uint8_t exhaust_speed_;
    uint32_t total_runtime_ms_;
    uint32_t last_update_ms_;
    bool has_error_;
    String error_message_;
};

#endif // VENTILATION_MODULE_H
