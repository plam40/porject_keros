/**
 * @file MQTTModule.cpp
 * @purpose Implementation of MQTT integration module
 * @dependencies MQTTModule.h, PubSubClient
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes MQTT publish: 5-15ms, Subscribe callback: <1ms
 * @module_type MODULE
 */

#include "modules/MQTTModule.h"
#include "modules/TemperatureSensorModule.h"
#include "modules/PumpModule.h"
#include "modules/HeatPumpModule.h"
#include "modules/HumidityControlModule.h"
#include "Skeleton.h"
#include "ConfigManager.h"
#include <ArduinoJson.h>

MQTTModule* MQTTModule::instance_ = nullptr;

MQTTModule::MQTTModule()
    : mqtt_client_(nullptr),
      broker_host_(""),
      broker_port_(1883),
      username_(""),
      password_(""),
      client_id_("hvac-controller"),
      discovery_prefix_("homeassistant"),
      connected_(false),
      last_reconnect_attempt_ms_(0),
      last_state_publish_ms_(0),
      state_publish_interval_ms_(60000)  // Publish every 60 seconds
{
    state_ = ModuleState::UNINITIALIZED;
    instance_ = this;
}

ModuleMetadata MQTTModule::get_metadata() const {
    ModuleMetadata meta;
    meta.name = "MQTTModule";
    meta.version = "1.0.0";
    meta.description = "MQTT client for smart home integration";
    meta.ram_usage_kb = 15;
    meta.flash_usage_kb = 35;
    meta.capabilities["protocol"] = "MQTT 3.1.1";
    meta.capabilities["discovery"] = "Home Assistant";
    return meta;
}

HealthStatus MQTTModule::health_check() {
    HealthStatus health;
    health.last_update_ms = millis();

    if (connected_ && mqtt_client_ && mqtt_client_->connected()) {
        health.is_healthy = true;
        health.health_percentage = 100;
        health.status_message = "Connected to MQTT broker";
    } else {
        health.is_healthy = false;
        health.health_percentage = 0;
        health.status_message = "Not connected to MQTT broker";
    }

    health.metrics["connected"] = connected_ ? 1.0f : 0.0f;

    return health;
}

bool MQTTModule::initialize() {
    Serial.println("[MQTT] Initializing...");
    state_ = ModuleState::INITIALIZING;

    // Load configuration
    ConfigManager& config = ConfigManager::get_instance();
    broker_host_ = config.get_string(ConfigSection::NETWORK, "mqtt_broker", "");
    broker_port_ = config.get_int(ConfigSection::NETWORK, "mqtt_port", 1883);
    username_ = config.get_string(ConfigSection::NETWORK, "mqtt_username", "");
    password_ = config.get_string(ConfigSection::NETWORK, "mqtt_password", "");
    client_id_ = config.get_string(ConfigSection::NETWORK, "mqtt_client_id", "hvac-controller");

    if (broker_host_.isEmpty()) {
        Serial.println("[MQTT] WARNING: No MQTT broker configured");
        state_ = ModuleState::DEGRADED;
        return true;
    }

    // Create MQTT client
    mqtt_client_ = new PubSubClient(wifi_client_);
    mqtt_client_->setServer(broker_host_.c_str(), broker_port_);
    mqtt_client_->setCallback(mqtt_callback);
    mqtt_client_->setBufferSize(512);

    // Connect to broker
    if (connect_mqtt()) {
        // Subscribe to events
        subscribe_to_events();

        // Publish discovery messages
        publish_ha_discovery();

        // Publish initial states
        publish_states();
    }

    state_ = ModuleState::RUNNING;
    Serial.println("[MQTT] Initialized");
    return true;
}

void MQTTModule::shutdown() {
    Serial.println("[MQTT] Shutting down...");

    // Publish offline status
    if (mqtt_client_ && mqtt_client_->connected()) {
        publish("hvac/status", "offline", true);
        mqtt_client_->disconnect();
    }

    if (mqtt_client_) {
        delete mqtt_client_;
        mqtt_client_ = nullptr;
    }

    connected_ = false;
    state_ = ModuleState::SHUTDOWN;
    Serial.println("[MQTT] Shutdown complete");
}

void MQTTModule::update() {
    if (state_ != ModuleState::RUNNING) {
        return;
    }

    // Reconnect if needed
    if (mqtt_client_ && !mqtt_client_->connected()) {
        uint32_t now = millis();
        if (now - last_reconnect_attempt_ms_ > 5000) {
            last_reconnect_attempt_ms_ = now;
            Serial.println("[MQTT] Attempting reconnection...");
            connect_mqtt();
        }
    }

    // Process MQTT messages
    if (mqtt_client_ && mqtt_client_->connected()) {
        mqtt_client_->loop();

        // Periodic state publishing
        uint32_t now = millis();
        if (now - last_state_publish_ms_ > state_publish_interval_ms_) {
            last_state_publish_ms_ = now;
            publish_states();
        }
    }
}

void MQTTModule::set_broker(const String& host, uint16_t port) {
    broker_host_ = host;
    broker_port_ = port;
}

void MQTTModule::set_auth(const String& username, const String& password) {
    username_ = username;
    password_ = password;
}

bool MQTTModule::is_connected() const {
    return connected_ && mqtt_client_ && mqtt_client_->connected();
}

bool MQTTModule::publish(const String& topic, const String& payload, bool retained) {
    if (!mqtt_client_ || !mqtt_client_->connected()) {
        return false;
    }

    return mqtt_client_->publish(topic.c_str(), payload.c_str(), retained);
}

bool MQTTModule::connect_mqtt() {
    if (!mqtt_client_) return false;

    Serial.printf("[MQTT] Connecting to %s:%d as %s\n",
                 broker_host_.c_str(), broker_port_, client_id_.c_str());

    // Connect with will message
    const char* will_topic = "hvac/status";
    const char* will_message = "offline";

    bool success;
    if (!username_.isEmpty()) {
        success = mqtt_client_->connect(client_id_.c_str(),
                                       username_.c_str(),
                                       password_.c_str(),
                                       will_topic,
                                       0,
                                       true,
                                       will_message);
    } else {
        success = mqtt_client_->connect(client_id_.c_str(),
                                       will_topic,
                                       0,
                                       true,
                                       will_message);
    }

    if (success) {
        connected_ = true;
        Serial.println("[MQTT] Connected successfully");

        // Publish online status
        publish("hvac/status", "online", true);

        // Subscribe to control topics
        subscribe_to_topics();

        // Publish MQTT connected event
        EventData event;
        event.source_module = "MQTTModule";
        EventBus::get_instance().publish(EventType::MQTT_CONNECTED, &event);

        return true;
    } else {
        connected_ = false;
        Serial.printf("[MQTT] Connection failed, rc=%d\n", mqtt_client_->state());
        return false;
    }
}

void MQTTModule::subscribe_to_topics() {
    if (!mqtt_client_ || !mqtt_client_->connected()) return;

    mqtt_client_->subscribe("hvac/+/+/set");
    mqtt_client_->subscribe("hvac/heatpump/mode/set");
    Serial.println("[MQTT] Subscribed to control topics");
}

void MQTTModule::subscribe_to_events() {
    EventBus& bus = EventBus::get_instance();

    // Temperature changes
    auto temp_sub = bus.subscribe(EventType::TEMPERATURE_CHANGED,
        [this](const EventData* data) {
            auto temp_data = static_cast<const TemperatureEventData*>(data);
            String topic = "hvac/temperature/" + temp_data->sensor_id;
            String payload = String(temp_data->temperature_c, 2);
            publish(topic, payload);
        });
    event_subscriptions_.push_back(temp_sub);

    Serial.println("[MQTT] Subscribed to system events");
}

void MQTTModule::publish_ha_discovery() {
    Serial.println("[MQTT] Publishing Home Assistant discovery messages...");

    // This would publish all discovery messages for sensors
    // Simplified for now
    publish("homeassistant/sensor/hvac/config", "{\"name\":\"HVAC System\",\"state_topic\":\"hvac/status\"}", true);
}

void MQTTModule::mqtt_callback(char* topic, byte* payload, unsigned int length) {
    if (!instance_) return;

    String topic_str = String(topic);
    String payload_str;
    for (unsigned int i = 0; i < length; i++) {
        payload_str += (char)payload[i];
    }

    instance_->handle_mqtt_command(topic_str, payload_str);
}

void MQTTModule::handle_mqtt_command(const String& topic, const String& payload) {
    Serial.printf("[MQTT] Command received: %s = %s\n", topic.c_str(), payload.c_str());

    // Parse topic and handle command
    // Example: hvac/pump/primary/set -> set pump speed
    // This would be implemented based on topic structure
}

void MQTTModule::publish_states() {
    publish_temperature_states();
    publish_pump_states();
    publish_heatpump_state();
    publish_humidity_states();
}

void MQTTModule::publish_temperature_states() {
    Skeleton& system = Skeleton::get_instance();
    TemperatureSensorModule* temp_mod =
        static_cast<TemperatureSensorModule*>(system.get_module("TemperatureSensorModule"));

    if (temp_mod) {
        auto temps = temp_mod->get_all_temperatures();
        for (const auto& pair : temps) {
            String topic = "hvac/temperature/" + pair.first;
            publish(topic, String(pair.second, 2));
        }
    }
}

void MQTTModule::publish_pump_states() {
    Skeleton& system = Skeleton::get_instance();
    PumpModule* pump_mod = static_cast<PumpModule*>(system.get_module("PumpModule"));

    if (pump_mod) {
        auto statuses = pump_mod->get_all_pump_status();
        for (const auto& pair : statuses) {
            DynamicJsonDocument doc(256);
            doc["speed"] = pair.second.actual_speed_percent;
            doc["state"] = static_cast<int>(pair.second.state);
            String json;
            serializeJson(doc, json);
            String topic = "hvac/pump/" + pair.first + "/state";
            publish(topic, json);
        }
    }
}

void MQTTModule::publish_heatpump_state() {
    Skeleton& system = Skeleton::get_instance();
    HeatPumpModule* hp_mod = static_cast<HeatPumpModule*>(system.get_module("HeatPumpModule"));

    if (hp_mod) {
        HeatPumpStatus status = hp_mod->get_status();
        DynamicJsonDocument doc(512);
        doc["mode"] = static_cast<int>(status.mode);
        doc["state"] = static_cast<int>(status.state);
        doc["cop"] = status.current_cop;
        doc["supply_temp"] = status.supply_temp_c;
        String json;
        serializeJson(doc, json);
        publish("hvac/heatpump/state", json);
    }
}

void MQTTModule::publish_humidity_states() {
    Skeleton& system = Skeleton::get_instance();
    HumidityControlModule* hum_mod =
        static_cast<HumidityControlModule*>(system.get_module("HumidityControlModule"));

    if (hum_mod) {
        auto statuses = hum_mod->get_all_zones_status();
        for (const auto& status : statuses) {
            DynamicJsonDocument doc(256);
            doc["humidity"] = status.current_humidity_percent;
            doc["target"] = status.target_humidity_percent;
            doc["mold_risk"] = status.mold_risk;
            String json;
            serializeJson(doc, json);
            String topic = "hvac/humidity/" + status.zone_name + "/state";
            publish(topic, json);
        }
    }
}
