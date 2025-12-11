/**
 * @file HeatStorageModule.cpp
 * @purpose Implementation of heat storage tank management
 * @dependencies HeatStorageModule.h, TemperatureSensorModule.h
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes Tank status update: <1ms
 * @module_type MODULE
 */

#include "modules/HeatStorageModule.h"
#include "modules/TemperatureSensorModule.h"
#include "Skeleton.h"
#include <math.h>

HeatStorageModule::HeatStorageModule() {
    state_ = ModuleState::UNINITIALIZED;
}

ModuleMetadata HeatStorageModule::get_metadata() const {
    ModuleMetadata meta;
    meta.name = "HeatStorageModule";
    meta.version = "1.0.0";
    meta.description = "Heat storage tank management with stratification monitoring";
    meta.ram_usage_kb = 10;
    meta.flash_usage_kb = 20;
    meta.capabilities["max_tanks"] = "4";
    meta.capabilities["stratification_monitoring"] = "yes";
    return meta;
}

HealthStatus HeatStorageModule::health_check() {
    HealthStatus health;
    health.last_update_ms = millis();
    health.is_healthy = true;
    health.health_percentage = 100;
    health.status_message = "Operating normally";

    int total_tanks = 0;
    int healthy_tanks = 0;

    for (const auto& pair : tanks_) {
        total_tanks++;
        if (!pair.second.has_error) {
            healthy_tanks++;
        }
    }

    if (total_tanks > 0) {
        health.health_percentage = (healthy_tanks * 100) / total_tanks;
        if (healthy_tanks < total_tanks) {
            health.is_healthy = false;
            health.status_message = "Some tanks have errors";
        }
    }

    health.metrics["total_tanks"] = total_tanks;
    health.metrics["healthy_tanks"] = healthy_tanks;

    return health;
}

bool HeatStorageModule::initialize() {
    Serial.println("[HeatStorage] Initializing...");
    state_ = ModuleState::INITIALIZING;

    // Tanks will be added via configuration or API
    state_ = ModuleState::RUNNING;
    Serial.println("[HeatStorage] Initialized");
    return true;
}

void HeatStorageModule::shutdown() {
    Serial.println("[HeatStorage] Shutting down...");
    state_ = ModuleState::SHUTDOWN;
    Serial.println("[HeatStorage] Shutdown complete");
}

void HeatStorageModule::update() {
    if (state_ != ModuleState::RUNNING) {
        return;
    }

    // Update all tanks
    for (auto& pair : tanks_) {
        update_tank(pair.second);
    }
}

bool HeatStorageModule::add_tank(const TankConfig& config) {
    if (tanks_.find(config.name) != tanks_.end()) {
        Serial.printf("[HeatStorage] WARNING: Tank %s already exists\n", config.name.c_str());
        return false;
    }

    Tank tank;
    tank.config = config;
    tank.state = TankState::IDLE;
    tank.stratification_index = 0.0;
    tank.charge_percent = 0.0;
    tank.has_error = false;

    tanks_[config.name] = tank;

    Serial.printf("[HeatStorage] Added tank: %s (%.0fL)\n",
                 config.name.c_str(), config.volume_liters);
    return true;
}

TankStatus HeatStorageModule::get_tank_status(const String& name) const {
    TankStatus status;
    status.name = name;
    status.has_error = true;
    status.error_message = "Tank not found";

    auto it = tanks_.find(name);
    if (it == tanks_.end()) {
        return status;
    }

    const Tank& tank = it->second;
    status.name = tank.config.name;
    status.state = tank.state;
    status.stratification_index = tank.stratification_index;
    status.charge_percent = tank.charge_percent;
    status.has_error = tank.has_error;
    status.error_message = tank.error_message;

    // Get temperatures
    if (!tank.temperatures.empty()) {
        status.top_temp_c = tank.temperatures.front();
        status.bottom_temp_c = tank.temperatures.back();

        float sum = 0.0;
        for (float temp : tank.temperatures) {
            sum += temp;
        }
        status.avg_temp_c = sum / tank.temperatures.size();
    }

    status.stored_energy_kwh = calculate_stored_energy(tank);

    return status;
}

std::vector<TankStatus> HeatStorageModule::get_all_tanks_status() const {
    std::vector<TankStatus> statuses;
    for (const auto& pair : tanks_) {
        statuses.push_back(get_tank_status(pair.first));
    }
    return statuses;
}

void HeatStorageModule::update_tank(Tank& tank) {
    if (!tank.config.enabled) {
        return;
    }

    // Get temperature sensor module
    Skeleton& system = Skeleton::get_instance();
    TemperatureSensorModule* temp_module =
        static_cast<TemperatureSensorModule*>(system.get_module("TemperatureSensorModule"));

    if (!temp_module) {
        tank.has_error = true;
        tank.error_message = "Temperature sensor module not found";
        return;
    }

    // Read all temperature sensors for this tank
    tank.temperatures.clear();
    for (const String& sensor_name : tank.config.temp_sensors) {
        float temp = temp_module->get_temperature(sensor_name);
        if (!isnan(temp)) {
            tank.temperatures.push_back(temp);
        }
    }

    if (tank.temperatures.empty()) {
        tank.has_error = true;
        tank.error_message = "No valid temperature readings";
        return;
    }

    // Calculate stratification index
    tank.stratification_index = calculate_stratification(tank.temperatures);

    // Calculate charge percentage
    tank.charge_percent = calculate_charge_percent(tank);

    // Determine tank state
    float top_temp = tank.temperatures.front();
    float bottom_temp = tank.temperatures.back();

    if (tank.charge_percent >= 95.0) {
        tank.state = TankState::FULL;
    } else if (tank.charge_percent <= 10.0) {
        tank.state = TankState::EMPTY;
    } else if (top_temp > bottom_temp + 5.0) {
        tank.state = TankState::CHARGING;
    } else if (top_temp < bottom_temp + 2.0) {
        tank.state = TankState::DISCHARGING;
    } else {
        tank.state = TankState::IDLE;
    }

    tank.has_error = false;
    tank.error_message = "";
}

float HeatStorageModule::calculate_stratification(const std::vector<float>& temps) {
    if (temps.size() < 2) {
        return 0.0;
    }

    // Stratification index based on temperature gradient
    // Higher gradient = better stratification
    float top_temp = temps.front();
    float bottom_temp = temps.back();
    float delta_t = top_temp - bottom_temp;

    // Good stratification: delta_t >= 10°C
    // Poor stratification: delta_t < 5°C
    float index = (delta_t / 15.0) * 100.0;

    // Clamp to 0-100%
    if (index < 0.0) index = 0.0;
    if (index > 100.0) index = 100.0;

    return index;
}

float HeatStorageModule::calculate_charge_percent(const Tank& tank) {
    if (tank.temperatures.empty()) {
        return 0.0;
    }

    // Calculate average temperature
    float sum = 0.0;
    for (float temp : tank.temperatures) {
        sum += temp;
    }
    float avg_temp = sum / tank.temperatures.size();

    // Charge percentage based on average temperature
    float temp_range = tank.config.max_temp_c - tank.config.min_temp_c;
    float charge = ((avg_temp - tank.config.min_temp_c) / temp_range) * 100.0;

    // Clamp to 0-100%
    if (charge < 0.0) charge = 0.0;
    if (charge > 100.0) charge = 100.0;

    return charge;
}

float HeatStorageModule::calculate_stored_energy(const Tank& tank) {
    if (tank.temperatures.empty()) {
        return 0.0;
    }

    // Calculate average temperature
    float sum = 0.0;
    for (float temp : tank.temperatures) {
        sum += temp;
    }
    float avg_temp = sum / tank.temperatures.size();

    // Stored energy = mass * specific_heat * delta_T
    // mass = volume * density (assume 1 kg/L for water)
    // specific_heat = 4.186 kJ/kg·K (water)
    // Reference temperature = min_temp_c

    float mass_kg = tank.config.volume_liters;  // 1 L ≈ 1 kg for water
    float specific_heat_kj = 4.186;
    float delta_t = avg_temp - tank.config.min_temp_c;

    float energy_kj = mass_kg * specific_heat_kj * delta_t;
    float energy_kwh = energy_kj / 3600.0;  // Convert kJ to kWh

    return energy_kwh;
}
