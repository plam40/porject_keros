/**
 * @file Skeleton.h
 * @purpose Core skeleton system that manages modules, events, safety, and configuration
 * @dependencies ModuleInterface.h, EventBus.h, SafetyEngine.h, ConfigManager.h
 * @version 1.0.0
 * @performance_notes Full system initialization: ~2-5 seconds, main loop: ~1ms
 *
 * The Skeleton is the core of the HVAC controller. It:
 * - Manages module lifecycle (registration, initialization, shutdown)
 * - Coordinates event bus communication
 * - Integrates safety monitoring
 * - Manages configuration
 * - Provides unified API for the entire system
 *
 * This follows the Skeleton + Module architecture pattern defined in CODING_STANDARDS.md
 */

#ifndef SKELETON_H
#define SKELETON_H

#include <Arduino.h>
#include "ModuleInterface.h"
#include "EventBus.h"
#include "SafetyEngine.h"
#include "ConfigManager.h"
#include <vector>
#include <map>

/**
 * System state
 */
enum class SystemState {
    UNINITIALIZED,      // System created but not initialized
    INITIALIZING,       // System is initializing
    RUNNING,            // System is running normally
    DEGRADED,           // System running with reduced functionality
    SHUTTING_DOWN,      // System is shutting down
    ERROR,              // System in error state
    EMERGENCY_STOP      // Emergency stop activated
};

/**
 * System statistics
 */
struct SystemStats {
    uint32_t uptime_ms;
    uint32_t loop_count;
    uint32_t average_loop_time_us;
    uint32_t max_loop_time_us;
    uint32_t free_heap_bytes;
    uint32_t min_free_heap_bytes;
    uint8_t cpu_usage_percent;
    uint8_t active_modules;
    uint8_t healthy_modules;
    SystemState state;
};

/**
 * @class Skeleton
 * @brief Core system that manages all HVAC controller modules and subsystems
 *
 * The Skeleton provides a unified interface to the entire HVAC controller system.
 * It manages the lifecycle of all modules, coordinates communication through the
 * event bus, monitors safety through the safety engine, and persists configuration
 * through the config manager.
 *
 * Initialization Sequence:
 * 1. Initialize SPIFFS filesystem
 * 2. Load configuration from ConfigManager
 * 3. Initialize EventBus
 * 4. Initialize SafetyEngine
 * 5. Register all modules
 * 6. Initialize each module in dependency order
 * 7. Start safety monitoring
 * 8. Publish SYSTEM_STARTUP event
 *
 * Main Loop Responsibilities:
 * 1. Update all modules
 * 2. Check safety status
 * 3. Process events
 * 4. Update statistics
 * 5. Feed watchdog
 *
 * Shutdown Sequence:
 * 1. Publish SYSTEM_SHUTDOWN event
 * 2. Stop safety monitoring
 * 3. Shutdown modules in reverse dependency order
 * 4. Save configuration
 * 5. Set hardware to safe state
 *
 * Usage Example:
 * Skeleton& system = Skeleton::get_instance();
 * system.register_module(&heat_pump_module);
 * system.register_module(&pump_module);
 * system.initialize();
 *
 * void loop() {
 *     system.update();
 * }
 */
class Skeleton {
public:
    /**
     * Get singleton instance
     * @return Reference to global Skeleton instance
     */
    static Skeleton& get_instance() {
        static Skeleton instance;
        return instance;
    }

    /**
     * Initialize the skeleton system
     * @param load_config If true, load configuration from filesystem
     * @return true if initialization successful
     */
    bool initialize(bool load_config = true);

    /**
     * Update system (call from main loop)
     * @note Should be called every loop iteration
     * @note Typical execution time: 0.5-2ms
     */
    void update();

    /**
     * Shutdown the system gracefully
     * @param emergency If true, perform emergency shutdown
     */
    void shutdown(bool emergency = false);

    /**
     * Register a module with the skeleton
     * @param module Pointer to module (skeleton does not take ownership)
     * @param priority Initialization priority (0=first, 255=last)
     * @return true if registered successfully
     * @note Must be called before initialize()
     */
    bool register_module(ModuleInterface* module, uint8_t priority = 100);

    /**
     * Unregister a module
     * @param module Pointer to module
     * @return true if unregistered successfully
     */
    bool unregister_module(ModuleInterface* module);

    /**
     * Get module by name
     * @param name Module name
     * @return Pointer to module or nullptr if not found
     */
    ModuleInterface* get_module(const String& name) const;

    /**
     * Get all registered modules
     * @return Vector of module pointers
     */
    std::vector<ModuleInterface*> get_all_modules() const;

    /**
     * Get current system state
     * @return Current system state
     */
    SystemState get_state() const { return state_; }

    /**
     * Get system statistics
     * @return System stats structure
     */
    SystemStats get_stats() const;

    /**
     * Check if system is healthy
     * @return true if system and all modules are healthy
     */
    bool is_healthy() const;

    /**
     * Get reference to event bus
     * @return EventBus reference
     */
    EventBus& get_event_bus() { return EventBus::get_instance(); }

    /**
     * Get reference to safety engine
     * @return SafetyEngine reference
     */
    SafetyEngine& get_safety_engine() { return SafetyEngine::get_instance(); }

    /**
     * Get reference to config manager
     * @return ConfigManager reference
     */
    ConfigManager& get_config_manager() { return ConfigManager::get_instance(); }

    /**
     * Enable/disable verbose logging
     * @param enable true to enable debug logging
     */
    void set_verbose_logging(bool enable) { verbose_logging_ = enable; }

    /**
     * Print system status to Serial
     */
    void print_status() const;

    /**
     * Perform system self-test
     * @return true if all self-tests pass
     */
    bool self_test();

    // Prevent copying
    Skeleton(const Skeleton&) = delete;
    Skeleton& operator=(const Skeleton&) = delete;

private:
    Skeleton() : state_(SystemState::UNINITIALIZED), verbose_logging_(false),
                 loop_count_(0), last_loop_time_us_(0), max_loop_time_us_(0),
                 total_loop_time_us_(0), min_free_heap_(0xFFFFFFFF),
                 system_start_time_ms_(0), cpu_sample_index_(0),
                 last_cpu_sample_ms_(0) {
        for (int i = 0; i < 10; i++) cpu_usage_samples_[i] = 0;
    }

    /**
     * Initialize all registered modules in priority order
     * @return true if all modules initialized successfully
     */
    bool initialize_modules();

    /**
     * Shutdown all modules in reverse priority order
     */
    void shutdown_modules();

    /**
     * Update module health monitoring
     */
    void update_module_health();

    /**
     * Update system statistics
     */
    void update_statistics();

    /**
     * Log message (if verbose logging enabled)
     */
    void log(const String& message) const;

    /**
     * Log error message (always logged)
     */
    void log_error(const String& message) const;

    /**
     * Module entry with priority
     */
    struct ModuleEntry {
        ModuleInterface* module;
        uint8_t priority;
    };

    // Registered modules (sorted by priority)
    std::vector<ModuleEntry> modules_;

    // System state
    SystemState state_;

    // Logging
    bool verbose_logging_;

    // Statistics
    uint32_t loop_count_;
    uint32_t last_loop_time_us_;
    uint32_t max_loop_time_us_;
    uint32_t total_loop_time_us_;
    uint32_t min_free_heap_;
    uint32_t system_start_time_ms_;

    // CPU usage tracking (rolling average over last second)
    uint32_t cpu_usage_samples_[10];
    uint8_t cpu_sample_index_;
    uint32_t last_cpu_sample_ms_;
};

#endif // SKELETON_H
