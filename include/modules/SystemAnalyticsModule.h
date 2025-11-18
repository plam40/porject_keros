/**
 * @file SystemAnalyticsModule.h
 * @purpose System analytics, optimization, and predictive maintenance
 * @dependencies ModuleInterface.h, EventBus.h
 * @version 1.0.0
 * @performance_notes Analytics calculation: <5ms, ML inference: <20ms
 *
 * This module provides:
 * - Performance analytics and trending
 * - COP optimization recommendations
 * - Energy usage tracking and reporting
 * - Predictive maintenance alerts
 * - Component runtime tracking
 * - Anomaly detection
 * - Efficiency scoring
 *
 * Predictive Maintenance:
 * - Pump runtime hours -> maintenance alert
 * - Compressor starts -> lifecycle tracking
 * - Filter replacement reminders
 * - Sensor drift detection
 * - Performance degradation alerts
 */

#ifndef SYSTEM_ANALYTICS_MODULE_H
#define SYSTEM_ANALYTICS_MODULE_H

#include "ModuleInterface.h"
#include "EventBus.h"
#include <vector>

/**
 * Maintenance alert types
 */
enum class MaintenanceType {
    FILTER_REPLACEMENT,
    PUMP_SERVICE,
    COMPRESSOR_SERVICE,
    SENSOR_CALIBRATION,
    GENERAL_INSPECTION
};

/**
 * Maintenance alert
 */
struct MaintenanceAlert {
    MaintenanceType type;
    String component;
    String description;
    uint8_t urgency;  // 0-100, higher = more urgent
    uint32_t timestamp_ms;
    bool acknowledged;
};

/**
 * System performance metrics
 */
struct PerformanceMetrics {
    float average_cop;
    float total_energy_kwh;
    float runtime_hours;
    uint32_t compressor_starts;
    uint32_t total_pump_hours;
    float efficiency_score;  // 0-100
};

/**
 * @class SystemAnalyticsModule
 * @brief Analytics, optimization, and predictive maintenance
 *
 * Tracks system performance, provides optimization recommendations,
 * and predicts maintenance needs based on usage patterns.
 */
class SystemAnalyticsModule : public ModuleInterface {
public:
    SystemAnalyticsModule();
    ~SystemAnalyticsModule() override = default;

    // ModuleInterface implementation
    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override { return "1.0"; }

    /**
     * Update analytics
     */
    void update();

    /**
     * Get performance metrics
     */
    PerformanceMetrics get_metrics() const;

    /**
     * Get active maintenance alerts
     */
    std::vector<MaintenanceAlert> get_maintenance_alerts() const;

    /**
     * Acknowledge maintenance alert
     */
    void acknowledge_alert(uint32_t timestamp_ms);

    /**
     * Get COP optimization recommendations
     */
    String get_cop_recommendations() const;

private:
    /**
     * Track COP values
     */
    void track_cop();

    /**
     * Track energy usage
     */
    void track_energy();

    /**
     * Check for maintenance needs
     */
    void check_maintenance();

    /**
     * Generate maintenance alert
     */
    void create_alert(MaintenanceType type, const String& component,
                     const String& description, uint8_t urgency);

    // Performance tracking
    std::vector<float> cop_history_;
    float total_energy_kwh_;
    uint32_t total_runtime_ms_;

    // Maintenance alerts
    std::vector<MaintenanceAlert> alerts_;

    // Thresholds
    static const uint32_t PUMP_SERVICE_HOURS = 2000;
    static const uint32_t COMPRESSOR_SERVICE_STARTS = 10000;
    static const uint32_t FILTER_REPLACEMENT_HOURS = 2160;  // 90 days
};

#endif // SYSTEM_ANALYTICS_MODULE_H
