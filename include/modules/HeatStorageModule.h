/**
 * @file HeatStorageModule.h
 * @purpose Heat storage tank management with stratification monitoring
 * @dependencies ModuleInterface.h, TemperatureSensorModule.h
 * @version 1.0.0
 * @performance_notes Tank status update: <1ms
 *
 * Manages heat storage tanks with temperature stratification monitoring.
 * Tracks charge/discharge states and total stored energy.
 */

#ifndef HEAT_STORAGE_MODULE_H
#define HEAT_STORAGE_MODULE_H

#include "ModuleInterface.h"
#include "EventBus.h"
#include <vector>

enum class TankState {
    IDLE,
    CHARGING,
    DISCHARGING,
    FULL,
    EMPTY
};

struct TankConfig {
    String name;
    float volume_liters;
    float min_temp_c;
    float max_temp_c;
    std::vector<String> temp_sensors;  // Top to bottom
    bool enabled;
};

struct TankStatus {
    String name;
    TankState state;
    float top_temp_c;
    float bottom_temp_c;
    float avg_temp_c;
    float stratification_index;  // 0-100%, higher is better
    float charge_percent;         // 0-100%
    float stored_energy_kwh;
    bool has_error;
    String error_message;
};

class HeatStorageModule : public ModuleInterface {
public:
    HeatStorageModule();
    ~HeatStorageModule() override = default;

    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override { return "1.0"; }

    void update();
    bool add_tank(const TankConfig& config);
    TankStatus get_tank_status(const String& name) const;
    std::vector<TankStatus> get_all_tanks_status() const;

private:
    struct Tank {
        TankConfig config;
        TankState state;
        std::vector<float> temperatures;
        float stratification_index;
        float charge_percent;
        bool has_error;
        String error_message;
    };

    void update_tank(Tank& tank);
    float calculate_stratification(const std::vector<float>& temps);
    float calculate_charge_percent(const Tank& tank);
    float calculate_stored_energy(const Tank& tank);

    std::map<String, Tank> tanks_;
};

#endif // HEAT_STORAGE_MODULE_H
