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
#include "HAL.h"

// Module includes
#include "modules/TemperatureSensorModule.h"
#include "modules/PumpModule.h"
#include "modules/HeatPumpModule.h"
#include "modules/VentilationModule.h"
#include "modules/HeatStorageModule.h"
#include "modules/HumidityControlModule.h"
#include "modules/WebServerModule.h"
#include "modules/MQTTModule.h"
#include "modules/SystemAnalyticsModule.h"

// Global skeleton instance reference
Skeleton& system = Skeleton::get_instance();

// Global module instances (must be static to persist)
static TemperatureSensorModule temp_module;
static PumpModule pump_module;
static HeatPumpModule heat_pump_module;
static VentilationModule ventilation_module;
static HeatStorageModule heat_storage_module;
static HumidityControlModule humidity_module;
static WebServerModule web_server_module;
static MQTTModule mqtt_module;
static SystemAnalyticsModule analytics_module;

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

    // Initialize HAL first
    HAL::get_instance().initialize();

    // Register modules in priority order (lower number = higher priority)
    // Temperature sensors first - other modules depend on temperature data
    system.register_module(&temp_module, 10);

    // Pumps next - needed for circulation
    system.register_module(&pump_module, 30);

    // Heat storage monitoring
    system.register_module(&heat_storage_module, 50);

    // Humidity control
    system.register_module(&humidity_module, 60);

    // Ventilation control
    system.register_module(&ventilation_module, 70);

    // Heat pump last - depends on other modules
    system.register_module(&heat_pump_module, 100);

    // Web server and networking modules (low priority)
    system.register_module(&web_server_module, 200);
    system.register_module(&mqtt_module, 210);
    system.register_module(&analytics_module, 220);

    // Configure pump module
    PumpConfig primary_pump;
    primary_pump.name = "primary";
    primary_pump.pwm_pin = 26;
    primary_pump.pwm_channel = 0;
    primary_pump.current_sense_pin = 35;
    primary_pump.min_speed_percent = 30;
    primary_pump.max_speed_percent = 100;
    primary_pump.min_on_time_ms = 60000;   // 1 minute
    primary_pump.min_off_time_ms = 30000;  // 30 seconds
    primary_pump.soft_start_time_ms = 3000; // 3 seconds
    primary_pump.max_current_amps = 5.0;
    primary_pump.min_current_amps = 0.5;
    primary_pump.current_calibration = 20.0;  // ADC voltage to Amps
    primary_pump.enabled = true;
    pump_module.add_pump(primary_pump);

    // Configure heat storage tank
    TankConfig main_tank;
    main_tank.name = "main_tank";
    main_tank.volume_liters = 500.0;
    main_tank.min_temp_c = 20.0;
    main_tank.max_temp_c = 80.0;
    main_tank.temp_sensors = {"tank_top", "tank_mid", "tank_bottom"};
    main_tank.enabled = true;
    heat_storage_module.add_tank(main_tank);

    // Configure humidity module
    // Add humidity sensor (SHT31 I2C sensor)
    HumiditySensorConfig hum_sensor;
    hum_sensor.name = "living_room_sensor";
    hum_sensor.type = HumiditySensorType::SHT31;
    hum_sensor.i2c_address = 0x44;  // Default SHT31 address
    hum_sensor.data_pin = 0;
    hum_sensor.humidity_offset = 0.0;
    hum_sensor.temperature_offset_c = 0.0;
    hum_sensor.enabled = true;
    humidity_module.add_sensor(hum_sensor);

    // Add humidity control zone
    HumidityZoneConfig hum_zone;
    hum_zone.zone_name = "living_room";
    hum_zone.sensor_name = "living_room_sensor";
    hum_zone.dehumidifier_pin = 33;  // GPIO 33 for dehumidifier relay
    hum_zone.humidifier_pin = 0;     // No humidifier
    hum_zone.target_humidity_percent = 50.0;
    hum_zone.hysteresis_percent = 5.0;
    hum_zone.min_run_time_ms = 300000;   // 5 minutes
    hum_zone.min_off_time_ms = 600000;   // 10 minutes
    hum_zone.max_humidity_percent = 70.0;
    hum_zone.min_humidity_percent = 30.0;
    hum_zone.mold_risk_humidity = 65.0;
    hum_zone.mold_risk_duration_ms = 3600000;  // 1 hour
    hum_zone.enabled = true;
    humidity_module.add_zone(hum_zone);

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
    // Update the system skeleton (safety, config, health monitoring)
    system.update();

    // Update all modules
    temp_module.update();
    pump_module.update();
    heat_pump_module.update();
    ventilation_module.update();
    heat_storage_module.update();
    humidity_module.update();
    web_server_module.update();
    mqtt_module.update();
    analytics_module.update();

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
