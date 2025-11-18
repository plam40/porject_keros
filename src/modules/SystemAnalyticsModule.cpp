/**
 * @file SystemAnalyticsModule.cpp
 * @purpose Implementation of system analytics and predictive maintenance
 * @version 1.0.0
 */

#include "modules/SystemAnalyticsModule.h"
#include "modules/HeatPumpModule.h"
#include "modules/PumpModule.h"
#include "modules/VentilationModule.h"
#include "Skeleton.h"

SystemAnalyticsModule::SystemAnalyticsModule()
    : total_energy_kwh_(0.0),
      total_runtime_ms_(0)
{
    state_ = ModuleState::UNINITIALIZED;
}

ModuleMetadata SystemAnalyticsModule::get_metadata() const {
    ModuleMetadata meta;
    meta.name = "SystemAnalyticsModule";
    meta.version = "1.0.0";
    meta.description = "Analytics and predictive maintenance";
    meta.ram_usage_kb = 8;
    meta.flash_usage_kb = 20;
    meta.capabilities["analytics"] = "yes";
    meta.capabilities["predictive_maintenance"] = "yes";
    return meta;
}

HealthStatus SystemAnalyticsModule::health_check() {
    HealthStatus health;
    health.last_update_ms = millis();
    health.is_healthy = true;
    health.health_percentage = 100;
    health.status_message = "Analytics running";
    health.metrics["cop_samples"] = cop_history_.size();
    health.metrics["maintenance_alerts"] = alerts_.size();
    return health;
}

bool SystemAnalyticsModule::initialize() {
    Serial.println("[Analytics] Initializing...");
    state_ = ModuleState::INITIALIZING;

    // Subscribe to COP updates
    EventBus::get_instance().subscribe(EventType::COP_UPDATED,
        [this](const EventData* data) {
            auto cop_data = static_cast<const ValueEventData*>(data);
            cop_history_.push_back(cop_data->value);
            if (cop_history_.size() > 1000) {
                cop_history_.erase(cop_history_.begin());
            }
        });

    state_ = ModuleState::RUNNING;
    Serial.println("[Analytics] Initialized");
    return true;
}

void SystemAnalyticsModule::shutdown() {
    Serial.println("[Analytics] Shutting down...");
    state_ = ModuleState::SHUTDOWN;
}

void SystemAnalyticsModule::update() {
    if (state_ != ModuleState::RUNNING) return;

    track_cop();
    track_energy();
    check_maintenance();
}

PerformanceMetrics SystemAnalyticsModule::get_metrics() const {
    PerformanceMetrics metrics;

    // Calculate average COP
    if (!cop_history_.empty()) {
        float sum = 0.0;
        for (float cop : cop_history_) {
            sum += cop;
        }
        metrics.average_cop = sum / cop_history_.size();
    } else {
        metrics.average_cop = 0.0;
    }

    metrics.total_energy_kwh = total_energy_kwh_;
    metrics.runtime_hours = total_runtime_ms_ / 3600000.0f;

    // Get compressor starts from heat pump
    Skeleton& system = Skeleton::get_instance();
    HeatPumpModule* hp = static_cast<HeatPumpModule*>(system.get_module("HeatPumpModule"));
    if (hp) {
        metrics.compressor_starts = hp->get_status().compressor_starts;
    }

    // Calculate efficiency score (simplified)
    metrics.efficiency_score = metrics.average_cop * 20.0f;  // Scale to 0-100
    if (metrics.efficiency_score > 100) metrics.efficiency_score = 100;

    return metrics;
}

std::vector<MaintenanceAlert> SystemAnalyticsModule::get_maintenance_alerts() const {
    std::vector<MaintenanceAlert> active;
    for (const auto& alert : alerts_) {
        if (!alert.acknowledged) {
            active.push_back(alert);
        }
    }
    return active;
}

void SystemAnalyticsModule::acknowledge_alert(uint32_t timestamp_ms) {
    for (auto& alert : alerts_) {
        if (alert.timestamp_ms == timestamp_ms) {
            alert.acknowledged = true;
            Serial.printf("[Analytics] Acknowledged maintenance alert: %s\n",
                         alert.description.c_str());
        }
    }
}

String SystemAnalyticsModule::get_cop_recommendations() const {
    PerformanceMetrics metrics = get_metrics();

    if (metrics.average_cop < 2.0) {
        return "COP is low. Check outdoor temp compensation and heat pump settings.";
    } else if (metrics.average_cop < 2.5) {
        return "COP is below optimal. Consider cleaning coils or checking refrigerant.";
    } else if (metrics.average_cop > 4.0) {
        return "COP is excellent. System is running optimally.";
    } else {
        return "COP is good. No immediate action needed.";
    }
}

void SystemAnalyticsModule::track_cop() {
    // COP is tracked via event subscription
}

void SystemAnalyticsModule::track_energy() {
    // Simplified energy tracking
    // In real implementation, would calculate based on power consumption
    total_runtime_ms_ += 100;  // Approximate
}

void SystemAnalyticsModule::check_maintenance() {
    Skeleton& system = Skeleton::get_instance();

    // Check pump runtime
    PumpModule* pump_mod = static_cast<PumpModule*>(system.get_module("PumpModule"));
    if (pump_mod) {
        auto statuses = pump_mod->get_all_pump_status();
        for (const auto& pair : statuses) {
            if (pair.second.total_runtime_hours > PUMP_SERVICE_HOURS) {
                create_alert(MaintenanceType::PUMP_SERVICE,
                           pair.first,
                           "Pump service recommended - " + String(pair.second.total_runtime_hours) + " hours",
                           70);
            }
        }
    }

    // Check compressor starts
    HeatPumpModule* hp = static_cast<HeatPumpModule*>(system.get_module("HeatPumpModule"));
    if (hp) {
        HeatPumpStatus status = hp->get_status();
        if (status.compressor_starts > COMPRESSOR_SERVICE_STARTS) {
            create_alert(MaintenanceType::COMPRESSOR_SERVICE,
                       "heat_pump",
                       "Compressor service recommended - " + String(status.compressor_starts) + " starts",
                       80);
        }
    }

    // Check filter runtime
    VentilationModule* vent = static_cast<VentilationModule*>(system.get_module("VentilationModule"));
    if (vent) {
        VentilationStatus status = vent->get_status();
        if (status.runtime_hours > FILTER_REPLACEMENT_HOURS) {
            create_alert(MaintenanceType::FILTER_REPLACEMENT,
                       "ventilation",
                       "Filter replacement recommended - " + String(status.runtime_hours) + " hours",
                       60);
        }
    }
}

void SystemAnalyticsModule::create_alert(MaintenanceType type, const String& component,
                                        const String& description, uint8_t urgency) {
    // Check if alert already exists
    for (const auto& alert : alerts_) {
        if (alert.type == type && alert.component == component && !alert.acknowledged) {
            return;  // Alert already exists
        }
    }

    MaintenanceAlert alert;
    alert.type = type;
    alert.component = component;
    alert.description = description;
    alert.urgency = urgency;
    alert.timestamp_ms = millis();
    alert.acknowledged = false;

    alerts_.push_back(alert);

    Serial.printf("[Analytics] New maintenance alert: %s\n", description.c_str());

    // Publish event
    SafetyEventData event;
    event.source_module = "SystemAnalyticsModule";
    event.violation_type = "MAINTENANCE_REQUIRED";
    event.affected_component = component;
    event.severity = urgency > 75 ? 2 : 1;
    event.remediation_action = description;
    EventBus::get_instance().publish(EventType::SYSTEM_ERROR, &event);
}
