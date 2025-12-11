/**
 * @file ModuleInterface.h
 * @purpose Base interface that all HVAC controller modules must implement
 * @dependencies None (base interface)
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes Virtual methods have minimal overhead (~5-10ns per call on ESP32)
 * @module_type SKELETON
 *
 * This interface ensures all modules follow the skeleton + module architecture pattern
 * defined in CODING_STANDARDS.md. Every module must implement these methods to ensure
 * proper lifecycle management, health monitoring, and fault isolation.
 */

#ifndef MODULE_INTERFACE_H
#define MODULE_INTERFACE_H

#include <Arduino.h>
#include <string>
#include <map>

/**
 * Module metadata structure
 * Contains version, capabilities, and resource requirements
 */
struct ModuleMetadata {
    String name;                    // Module name (e.g., "HeatPumpModule")
    String version;                 // Semantic version (e.g., "1.0.0")
    String description;             // Brief description of module function
    uint32_t ram_usage_kb;          // Estimated RAM usage in KB
    uint32_t flash_usage_kb;        // Estimated Flash usage in KB
    std::map<String, String> capabilities;  // Key-value pairs of module capabilities
};

/**
 * Health check result structure
 * Used by safety engine to monitor module health
 */
struct HealthStatus {
    bool is_healthy;                // Overall health status
    uint8_t health_percentage;      // 0-100 health score
    String status_message;          // Human-readable status
    uint32_t last_update_ms;        // Timestamp of last health check
    std::map<String, float> metrics;  // Additional health metrics
};

/**
 * Module states for lifecycle management
 */
enum class ModuleState {
    UNINITIALIZED,      // Module created but not initialized
    INITIALIZING,       // Module is initializing
    RUNNING,            // Module is running normally
    DEGRADED,           // Module is running with reduced functionality
    ERROR,              // Module encountered an error
    SHUTDOWN            // Module has been shut down
};

/**
 * @class ModuleInterface
 * @brief Base interface for all HVAC controller modules
 *
 * All modules in the HVAC controller must inherit from this interface
 * and implement all pure virtual methods. This ensures consistent behavior,
 * proper lifecycle management, and enables the skeleton core to manage
 * all modules uniformly.
 *
 * Implementation Requirements:
 * - Constructor must be lightweight (< 1ms)
 * - initialize() must complete within 5 seconds
 * - health_check() must complete within 100ms
 * - shutdown() must complete within 2 seconds
 *
 * Example:
 * class MyModule : public ModuleInterface {
 * public:
 *     MyModule() : state_(ModuleState::UNINITIALIZED) {}
 *     ModuleMetadata get_metadata() const override { ... }
 *     HealthStatus health_check() override { ... }
 *     bool initialize() override { ... }
 *     void shutdown() override { ... }
 *     String get_api_version() const override { return "1.0"; }
 * };
 */
class ModuleInterface {
public:
    virtual ~ModuleInterface() = default;

    /**
     * Get module metadata
     * @return Metadata structure with module information
     * @note Must be const and thread-safe
     */
    virtual ModuleMetadata get_metadata() const = 0;

    /**
     * Perform health check on module
     * @return Health status with metrics
     * @note Must complete within 100ms
     * @note Called by safety engine every monitoring cycle
     */
    virtual HealthStatus health_check() = 0;

    /**
     * Initialize the module
     * @return true if initialization successful, false otherwise
     * @note Must complete within 5 seconds
     * @note May be called multiple times if initial attempt fails
     */
    virtual bool initialize() = 0;

    /**
     * Gracefully shutdown the module
     * @note Must complete within 2 seconds
     * @note Must leave hardware in safe state
     * @note Called during system shutdown or module removal
     */
    virtual void shutdown() = 0;

    /**
     * Get API version for compatibility checking
     * @return API version string (e.g., "1.0")
     * @note Used by skeleton to ensure module compatibility
     */
    virtual String get_api_version() const = 0;

    /**
     * Get current module state
     * @return Current state of the module
     */
    virtual ModuleState get_state() const { return state_; }

    /**
     * Get module name (convenience method)
     * @return Module name from metadata
     */
    virtual String get_name() const { return get_metadata().name; }

protected:
    ModuleState state_ = ModuleState::UNINITIALIZED;
};

#endif // MODULE_INTERFACE_H
