/**
 * @file MQTTModule.h
 * @purpose MQTT integration for smart home platforms (Home Assistant, OpenHAB, etc.)
 * @dependencies ModuleInterface.h, PubSubClient, EventBus.h
 * @version 1.0.0
 * @performance_notes MQTT publish: 5-15ms, Subscribe callback: <1ms
 *
 * This module provides MQTT integration with:
 * - Auto-discovery for Home Assistant
 * - Publishing of all sensor data
 * - Control via MQTT commands
 * - Status/availability reporting
 * - Automatic reconnection
 *
 * MQTT Topics Structure:
 * homeassistant/sensor/hvac/{sensor_id}/state
 * homeassistant/sensor/hvac/{sensor_id}/config (auto-discovery)
 * hvac/temperature/{sensor_name}
 * hvac/pump/{pump_name}/state
 * hvac/pump/{pump_name}/set
 * hvac/heatpump/state
 * hvac/heatpump/mode/set
 * hvac/humidity/{zone}/state
 * hvac/humidity/{zone}/target/set
 * hvac/status (availability: online/offline)
 */

#ifndef MQTT_MODULE_H
#define MQTT_MODULE_H

#include "ModuleInterface.h"
#include "EventBus.h"
#include <PubSubClient.h>
#include <WiFiClient.h>

/**
 * @class MQTTModule
 * @brief MQTT client for smart home integration
 *
 * Provides MQTT connectivity for integration with Home Assistant and other
 * smart home platforms. Publishes all HVAC data and subscribes to control topics.
 *
 * Configuration Example:
 * network:
 *   mqtt_broker: "192.168.1.100"
 *   mqtt_port: 1883
 *   mqtt_username: "homeassistant"
 *   mqtt_password: "password"
 *   mqtt_client_id: "hvac-controller"
 *   mqtt_discovery_prefix: "homeassistant"
 *
 * Usage Example:
 * MQTTModule mqtt;
 * system.register_module(&mqtt, 210);
 *
 * Home Assistant will auto-discover all sensors and controls.
 */
class MQTTModule : public ModuleInterface {
public:
    MQTTModule();
    ~MQTTModule() override = default;

    // ModuleInterface implementation
    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override { return "1.0"; }

    /**
     * Update MQTT connection and process messages
     */
    void update();

    /**
     * Set MQTT broker
     */
    void set_broker(const String& host, uint16_t port);

    /**
     * Set MQTT authentication
     */
    void set_auth(const String& username, const String& password);

    /**
     * Check if connected to MQTT broker
     */
    bool is_connected() const;

    /**
     * Publish a message
     */
    bool publish(const String& topic, const String& payload, bool retained = false);

private:
    /**
     * Connect to MQTT broker
     */
    bool connect_mqtt();

    /**
     * Subscribe to control topics
     */
    void subscribe_to_topics();

    /**
     * Subscribe to system events
     */
    void subscribe_to_events();

    /**
     * Publish Home Assistant discovery messages
     */
    void publish_ha_discovery();

    /**
     * Publish discovery for a sensor
     */
    void publish_sensor_discovery(const String& name, const String& device_class,
                                  const String& unit, const String& state_topic);

    /**
     * MQTT callback for incoming messages
     */
    static void mqtt_callback(char* topic, byte* payload, unsigned int length);

    /**
     * Handle incoming MQTT command
     */
    void handle_mqtt_command(const String& topic, const String& payload);

    /**
     * Publish all current states
     */
    void publish_states();

    /**
     * Publish temperature states
     */
    void publish_temperature_states();

    /**
     * Publish pump states
     */
    void publish_pump_states();

    /**
     * Publish heat pump state
     */
    void publish_heatpump_state();

    /**
     * Publish humidity states
     */
    void publish_humidity_states();

    // MQTT client
    WiFiClient wifi_client_;
    PubSubClient* mqtt_client_;

    // Connection parameters
    String broker_host_;
    uint16_t broker_port_;
    String username_;
    String password_;
    String client_id_;
    String discovery_prefix_;

    // State tracking
    bool connected_;
    uint32_t last_reconnect_attempt_ms_;
    uint32_t last_state_publish_ms_;
    uint32_t state_publish_interval_ms_;

    // Event subscriptions
    std::vector<uint32_t> event_subscriptions_;

    // Static instance for callback
    static MQTTModule* instance_;
};

#endif // MQTT_MODULE_H
