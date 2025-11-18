/**
 * @file main.cpp
 * @purpose Main entry point for HVAC Controller
 * @dependencies Skeleton.h and all module headers
 * @version 1.0.0
 * @performance_notes Main loop runs at ~1kHz, can be adjusted based on requirements
 *
 * This file initializes the HVAC controller system and runs the main control loop.
 * Modules are registered here and the skeleton manages their lifecycle.
 */

#include <Arduino.h>
#include "Skeleton.h"

// Module includes will go here as they are implemented
// #include "modules/TemperatureSensorModule.h"
// #include "modules/PumpModule.h"
// #include "modules/HeatPumpModule.h"
// etc.

// Global skeleton instance reference
Skeleton& system = Skeleton::get_instance();

/**
 * Arduino setup function
 * Called once at startup
 */
void setup() {
    // Initialize Serial for debugging
    Serial.begin(115200);
    delay(1000);  // Wait for serial monitor to connect

    Serial.println("\n\n");
    Serial.println("====================================");
    Serial.println("  HVAC Controller Starting...");
    Serial.println("====================================");
    Serial.printf("Chip Model: %s\n", ESP.getChipModel());
    Serial.printf("Chip Revision: %d\n", ESP.getChipRevision());
    Serial.printf("CPU Frequency: %d MHz\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash Size: %d bytes\n", ESP.getFlashChipSize());
    Serial.printf("Free Heap: %d bytes\n", ESP.getFreeHeap());
    Serial.printf("SDK Version: %s\n", ESP.getSdkVersion());
    Serial.println("====================================\n");

    // Enable verbose logging
    system.set_verbose_logging(true);

    // Register modules here
    // Example:
    // static TemperatureSensorModule temp_module;
    // system.register_module(&temp_module, 10);  // High priority
    //
    // static PumpModule pump_module;
    // system.register_module(&pump_module, 50);  // Medium priority
    //
    // static HeatPumpModule hp_module;
    // system.register_module(&hp_module, 100);  // Lower priority

    // Initialize the system
    Serial.println("Initializing system...\n");
    if (!system.initialize()) {
        Serial.println("\n!!! SYSTEM INITIALIZATION FAILED !!!\n");
        Serial.println("System halted. Please check error messages above.");
        while (true) {
            delay(1000);
        }
    }

    // Run self-test
    Serial.println("\nRunning self-test...\n");
    if (!system.self_test()) {
        Serial.println("\n!!! SELF-TEST FAILED !!!\n");
        Serial.println("Some tests failed. System will continue but may not operate correctly.");
    }

    Serial.println("\n====================================");
    Serial.println("  System Ready - Entering Main Loop");
    Serial.println("====================================\n");

    // Print initial status
    system.print_status();
}

/**
 * Arduino main loop
 * Called continuously
 */
void loop() {
    // Update the system
    // This calls update on all registered modules and monitors health
    system.update();

    // Optional: Print periodic status updates
    static uint32_t last_status_print = 0;
    if (millis() - last_status_print > 60000) {  // Every 60 seconds
        last_status_print = millis();
        system.print_status();

        // Print safety violations if any
        auto violations = system.get_safety_engine().get_active_violations();
        if (!violations.empty()) {
            Serial.println("\n*** ACTIVE SAFETY VIOLATIONS ***");
            for (const auto& v : violations) {
                Serial.printf("  [%s] Level %d: %s\n",
                             v.module_name.c_str(),
                             static_cast<int>(v.level),
                             v.description.c_str());
            }
            Serial.println();
        }
    }

    // Small delay to prevent watchdog issues
    // The system loop should run at ~1000Hz (every 1ms)
    delay(1);
}
