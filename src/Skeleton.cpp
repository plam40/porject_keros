/**
 * @file Skeleton.cpp
 * @purpose Implementation of core skeleton system
 * @dependencies Skeleton.h, ModuleInterface.h, EventBus.h, SafetyEngine.h, ConfigManager.h
 * @version 1.0.0
 * @performance_notes Full system update cycle: 0.5-2ms typical
 */

#include "Skeleton.h"
#include <algorithm>

bool Skeleton::initialize(bool load_config) {
    if (state_ != SystemState::UNINITIALIZED) {
        log_error("System already initialized");
        return false;
    }

    log("Initializing HVAC Controller Skeleton...");
    state_ = SystemState::INITIALIZING;
    system_start_time_ms_ = millis();

    // Initialize Serial if not already done
    if (!Serial) {
        Serial.begin(115200);
        delay(100);
    }

    log("Initializing ConfigManager...");
    if (!ConfigManager::get_instance().initialize()) {
        log_error("Failed to initialize ConfigManager");
        state_ = SystemState::ERROR;
        return false;
    }

    if (load_config) {
        log("Loading configuration...");
        if (!ConfigManager::get_instance().load()) {
            log("WARNING: Failed to load config, using defaults");
        }
    }

    log("Initializing SafetyEngine...");
    if (!SafetyEngine::get_instance().initialize()) {
        log_error("Failed to initialize SafetyEngine");
        state_ = SystemState::ERROR;
        return false;
    }

    log("Initializing modules...");
    if (!initialize_modules()) {
        log_error("Failed to initialize modules");
        state_ = SystemState::ERROR;
        return false;
    }

    log("Starting safety monitoring...");
    if (!SafetyEngine::get_instance().start_monitoring()) {
        log_error("Failed to start safety monitoring");
        state_ = SystemState::ERROR;
        return false;
    }

    // Publish system startup event
    EventData event;
    event.source_module = "Skeleton";
    EventBus::get_instance().publish(EventType::SYSTEM_STARTUP, &event);

    state_ = SystemState::RUNNING;
    log("Skeleton initialized successfully");
    print_status();

    return true;
}

void Skeleton::update() {
    if (state_ != SystemState::RUNNING && state_ != SystemState::DEGRADED) {
        return;
    }

    uint32_t loop_start_us = micros();

    // Feed watchdog
    SafetyEngine::get_instance().feed_watchdog();

    // Update module health monitoring
    update_module_health();

    // Update statistics
    update_statistics();

    // Check if we need to degrade or recover
    if (SafetyEngine::get_instance().get_current_safety_level() >= SafetyLevel::DEGRADED) {
        if (state_ == SystemState::RUNNING) {
            state_ = SystemState::DEGRADED;
            log("System entering DEGRADED mode");
        }
    } else {
        if (state_ == SystemState::DEGRADED) {
            state_ = SystemState::RUNNING;
            log("System recovered to RUNNING mode");
        }
    }

    // Calculate loop time
    uint32_t loop_time_us = micros() - loop_start_us;
    last_loop_time_us_ = loop_time_us;
    if (loop_time_us > max_loop_time_us_) {
        max_loop_time_us_ = loop_time_us;
    }
    total_loop_time_us_ += loop_time_us;
    loop_count_++;
}

void Skeleton::shutdown(bool emergency) {
    if (emergency) {
        log_error("EMERGENCY SHUTDOWN INITIATED");
        SafetyEngine::get_instance().emergency_shutdown("User requested emergency shutdown");
        state_ = SystemState::EMERGENCY_STOP;
        return;
    }

    log("Initiating graceful shutdown...");
    state_ = SystemState::SHUTTING_DOWN;

    // Publish shutdown event
    EventData event;
    event.source_module = "Skeleton";
    EventBus::get_instance().publish(EventType::SYSTEM_SHUTDOWN, &event);

    // Stop safety monitoring
    log("Stopping safety monitoring...");
    SafetyEngine::get_instance().stop_monitoring();

    // Shutdown all modules
    log("Shutting down modules...");
    shutdown_modules();

    // Save configuration
    log("Saving configuration...");
    ConfigManager::get_instance().save(true);

    state_ = SystemState::UNINITIALIZED;
    log("Shutdown complete");
}

bool Skeleton::register_module(ModuleInterface* module, uint8_t priority) {
    if (!module) {
        log_error("Cannot register null module");
        return false;
    }

    if (state_ != SystemState::UNINITIALIZED) {
        log_error("Cannot register modules after initialization");
        return false;
    }

    // Check if already registered
    for (const auto& entry : modules_) {
        if (entry.module == module) {
            log_error("Module already registered: " + module->get_name());
            return false;
        }
    }

    // Add module entry
    ModuleEntry entry;
    entry.module = module;
    entry.priority = priority;
    modules_.push_back(entry);

    // Sort modules by priority (lower number = higher priority, initialized first)
    std::sort(modules_.begin(), modules_.end(),
             [](const ModuleEntry& a, const ModuleEntry& b) {
                 return a.priority < b.priority;
             });

    log("Registered module: " + module->get_name() + " (priority " + String(priority) + ")");

    return true;
}

bool Skeleton::unregister_module(ModuleInterface* module) {
    for (auto it = modules_.begin(); it != modules_.end(); ++it) {
        if (it->module == module) {
            log("Unregistered module: " + module->get_name());
            modules_.erase(it);
            return true;
        }
    }
    return false;
}

ModuleInterface* Skeleton::get_module(const String& name) const {
    for (const auto& entry : modules_) {
        if (entry.module->get_name() == name) {
            return entry.module;
        }
    }
    return nullptr;
}

std::vector<ModuleInterface*> Skeleton::get_all_modules() const {
    std::vector<ModuleInterface*> result;
    for (const auto& entry : modules_) {
        result.push_back(entry.module);
    }
    return result;
}

SystemStats Skeleton::get_stats() const {
    SystemStats stats;
    stats.uptime_ms = millis() - system_start_time_ms_;
    stats.loop_count = loop_count_;
    stats.average_loop_time_us = loop_count_ > 0 ? total_loop_time_us_ / loop_count_ : 0;
    stats.max_loop_time_us = max_loop_time_us_;
    stats.free_heap_bytes = ESP.getFreeHeap();
    stats.min_free_heap_bytes = min_free_heap_;
    stats.cpu_usage_percent = 0;  // TODO: Calculate actual CPU usage
    stats.active_modules = 0;
    stats.healthy_modules = 0;
    stats.state = state_;

    // Count active and healthy modules
    for (const auto& entry : modules_) {
        if (entry.module->get_state() == ModuleState::RUNNING) {
            stats.active_modules++;
            if (entry.module->health_check().is_healthy) {
                stats.healthy_modules++;
            }
        }
    }

    return stats;
}

bool Skeleton::is_healthy() const {
    // Check system state
    if (state_ != SystemState::RUNNING && state_ != SystemState::DEGRADED) {
        return false;
    }

    // Check safety status
    if (!SafetyEngine::get_instance().is_safe_to_operate()) {
        return false;
    }

    // Check all modules
    for (const auto& entry : modules_) {
        HealthStatus health = entry.module->health_check();
        if (!health.is_healthy) {
            return false;
        }
    }

    return true;
}

void Skeleton::print_status() const {
    Serial.println("\n========================================");
    Serial.println("   HVAC Controller System Status");
    Serial.println("========================================");

    SystemStats stats = get_stats();

    Serial.printf("State: %d\n", static_cast<int>(stats.state));
    Serial.printf("Uptime: %u ms\n", stats.uptime_ms);
    Serial.printf("Loop Count: %u\n", stats.loop_count);
    Serial.printf("Avg Loop Time: %u μs\n", stats.average_loop_time_us);
    Serial.printf("Max Loop Time: %u μs\n", stats.max_loop_time_us);
    Serial.printf("Free Heap: %u bytes\n", stats.free_heap_bytes);
    Serial.printf("Min Free Heap: %u bytes\n", stats.min_free_heap_bytes);
    Serial.printf("Modules: %d active, %d healthy (of %d total)\n",
                 stats.active_modules, stats.healthy_modules, modules_.size());

    Serial.println("\nRegistered Modules:");
    for (const auto& entry : modules_) {
        ModuleMetadata meta = entry.module->get_metadata();
        HealthStatus health = entry.module->health_check();
        Serial.printf("  [%d] %-20s v%-6s %s (%d%% health)\n",
                     entry.priority,
                     meta.name.c_str(),
                     meta.version.c_str(),
                     health.is_healthy ? "OK" : "DEGRADED",
                     health.health_percentage);
    }

    Serial.println("========================================\n");
}

bool Skeleton::self_test() {
    log("Running system self-test...");

    bool all_passed = true;

    // Test 1: ConfigManager
    log("Test 1: ConfigManager...");
    if (!ConfigManager::get_instance().get_string(ConfigSection::SYSTEM, "device_name", "").isEmpty()) {
        log("  PASS: ConfigManager operational");
    } else {
        log_error("  FAIL: ConfigManager test failed");
        all_passed = false;
    }

    // Test 2: EventBus
    log("Test 2: EventBus...");
    bool event_received = false;
    auto sub_id = EventBus::get_instance().subscribe(EventType::SYSTEM_STARTUP,
        [&event_received](const EventData* data) {
            event_received = true;
        });
    EventData test_event;
    EventBus::get_instance().publish(EventType::SYSTEM_STARTUP, &test_event);
    EventBus::get_instance().unsubscribe(EventType::SYSTEM_STARTUP, sub_id);
    if (event_received) {
        log("  PASS: EventBus operational");
    } else {
        log_error("  FAIL: EventBus test failed");
        all_passed = false;
    }

    // Test 3: SafetyEngine
    log("Test 3: SafetyEngine...");
    if (SafetyEngine::get_instance().is_safe_to_operate()) {
        log("  PASS: SafetyEngine operational");
    } else {
        log_error("  FAIL: SafetyEngine test failed");
        all_passed = false;
    }

    // Test 4: Module health checks
    log("Test 4: Module health checks...");
    bool all_healthy = true;
    for (const auto& entry : modules_) {
        HealthStatus health = entry.module->health_check();
        if (!health.is_healthy) {
            log_error("  Module " + entry.module->get_name() + " unhealthy");
            all_healthy = false;
        }
    }
    if (all_healthy) {
        log("  PASS: All modules healthy");
    } else {
        log_error("  FAIL: Some modules unhealthy");
        all_passed = false;
    }

    if (all_passed) {
        log("Self-test: ALL TESTS PASSED");
    } else {
        log_error("Self-test: SOME TESTS FAILED");
    }

    return all_passed;
}

bool Skeleton::initialize_modules() {
    bool all_initialized = true;

    for (const auto& entry : modules_) {
        log("Initializing module: " + entry.module->get_name());

        if (!entry.module->initialize()) {
            log_error("Failed to initialize module: " + entry.module->get_name());
            all_initialized = false;
            continue;
        }

        // Register with safety engine
        SafetyEngine::get_instance().register_module(entry.module);

        log("Module initialized: " + entry.module->get_name());
    }

    return all_initialized;
}

void Skeleton::shutdown_modules() {
    // Shutdown in reverse order
    for (auto it = modules_.rbegin(); it != modules_.rend(); ++it) {
        log("Shutting down module: " + it->module->get_name());
        it->module->shutdown();
    }
}

void Skeleton::update_module_health() {
    // Module health is checked by SafetyEngine's monitoring task
    // This is just a placeholder for any additional health checks
}

void Skeleton::update_statistics() {
    // Update min free heap
    uint32_t free_heap = ESP.getFreeHeap();
    if (free_heap < min_free_heap_) {
        min_free_heap_ = free_heap;
    }
}

void Skeleton::log(const String& message) const {
    if (verbose_logging_) {
        Serial.printf("[Skeleton] %s\n", message.c_str());
    }
}

void Skeleton::log_error(const String& message) const {
    Serial.printf("[Skeleton] ERROR: %s\n", message.c_str());
}
