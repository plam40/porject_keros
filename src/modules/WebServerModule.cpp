/**
 * @file WebServerModule.cpp
 * @purpose Implementation of web server module
 * @dependencies WebServerModule.h, WiFi, AsyncWebServer
 * @version 1.0.0
 * @performance_notes Async server handles requests without blocking
 */

#include "modules/WebServerModule.h"
#include "modules/TemperatureSensorModule.h"
#include "modules/PumpModule.h"
#include "modules/HeatPumpModule.h"
#include "modules/VentilationModule.h"
#include "modules/HeatStorageModule.h"
#include "modules/HumidityControlModule.h"
#include "Skeleton.h"
#include "ConfigManager.h"
#include <WiFi.h>

WebServerModule::WebServerModule()
    : server_(nullptr),
      websocket_(nullptr),
      wifi_ssid_(""),
      wifi_password_(""),
      hostname_("hvac-controller"),
      api_username_("admin"),
      api_password_("changeme"),
      web_port_(80),
      wifi_connected_(false),
      last_wifi_check_ms_(0)
{
    state_ = ModuleState::UNINITIALIZED;
}

WebServerModule::~WebServerModule() {
    if (server_) delete server_;
    if (websocket_) delete websocket_;
}

ModuleMetadata WebServerModule::get_metadata() const {
    ModuleMetadata meta;
    meta.name = "WebServerModule";
    meta.version = "1.0.0";
    meta.description = "Web server with REST API and WebSocket";
    meta.ram_usage_kb = 25;
    meta.flash_usage_kb = 50;
    meta.capabilities["protocols"] = "HTTP,WebSocket";
    meta.capabilities["api_version"] = "v1";
    return meta;
}

HealthStatus WebServerModule::health_check() {
    HealthStatus health;
    health.last_update_ms = millis();

    if (wifi_connected_ && server_) {
        health.is_healthy = true;
        health.health_percentage = 100;
        health.status_message = "Web server running";
    } else if (!wifi_connected_) {
        health.is_healthy = false;
        health.health_percentage = 0;
        health.status_message = "WiFi not connected";
    } else {
        health.is_healthy = false;
        health.health_percentage = 50;
        health.status_message = "Server not started";
    }

    health.metrics["wifi_connected"] = wifi_connected_ ? 1.0f : 0.0f;
    health.metrics["websocket_clients"] = websocket_ ? websocket_->count() : 0.0f;

    return health;
}

bool WebServerModule::initialize() {
    Serial.println("[WebServer] Initializing...");
    state_ = ModuleState::INITIALIZING;

    // Load configuration
    ConfigManager& config = ConfigManager::get_instance();
    wifi_ssid_ = config.get_string(ConfigSection::NETWORK, "wifi_ssid", "");
    wifi_password_ = config.get_string(ConfigSection::NETWORK, "wifi_password", "");
    hostname_ = config.get_string(ConfigSection::NETWORK, "hostname", "hvac-controller");
    api_username_ = config.get_string(ConfigSection::NETWORK, "api_username", "admin");
    api_password_ = config.get_string(ConfigSection::NETWORK, "api_password", "changeme");
    web_port_ = config.get_int(ConfigSection::NETWORK, "web_port", 80);

    // Initialize WiFi
    if (!init_wifi()) {
        Serial.println("[WebServer] WARNING: WiFi not configured, continuing without network");
        state_ = ModuleState::DEGRADED;
        return true;  // Don't fail initialization
    }

    // Create web server
    server_ = new AsyncWebServer(web_port_);

    // Create WebSocket
    websocket_ = new AsyncWebSocket("/ws");
    websocket_->onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client,
                               AwsEventType type, void* arg, uint8_t* data, size_t len) {
        this->on_websocket_event(server, client, type, arg, data, len);
    });
    server_->addHandler(websocket_);

    // Setup REST API endpoints
    setup_api_endpoints();

    // Subscribe to system events for WebSocket broadcasting
    subscribe_to_events();

    // Start server
    server_->begin();

    Serial.printf("[WebServer] Server started on http://%s:%d\n",
                 WiFi.localIP().toString().c_str(), web_port_);
    Serial.printf("[WebServer] WebSocket available at ws://%s:%d/ws\n",
                 WiFi.localIP().toString().c_str(), web_port_);

    state_ = ModuleState::RUNNING;
    return true;
}

void WebServerModule::shutdown() {
    Serial.println("[WebServer] Shutting down...");

    // Unsubscribe from events
    for (uint32_t sub_id : event_subscriptions_) {
        // We don't know which event type, but that's OK
    }
    event_subscriptions_.clear();

    // Stop server
    if (server_) {
        server_->end();
    }

    // Disconnect WiFi
    WiFi.disconnect(true);
    wifi_connected_ = false;

    state_ = ModuleState::SHUTDOWN;
    Serial.println("[WebServer] Shutdown complete");
}

void WebServerModule::update() {
    if (state_ != ModuleState::RUNNING && state_ != ModuleState::DEGRADED) {
        return;
    }

    // Check WiFi connection periodically
    uint32_t now = millis();
    if (now - last_wifi_check_ms_ > 10000) {  // Every 10 seconds
        last_wifi_check_ms_ = now;

        bool previously_connected = wifi_connected_;
        wifi_connected_ = (WiFi.status() == WL_CONNECTED);

        if (!wifi_connected_ && previously_connected) {
            Serial.println("[WebServer] WiFi disconnected, attempting reconnect...");
            init_wifi();
        }
    }

    // Clean up WebSocket clients
    if (websocket_) {
        websocket_->cleanupClients();
    }
}

void WebServerModule::broadcast_message(const String& event, const String& data) {
    if (!websocket_) return;

    DynamicJsonDocument doc(1024);
    doc["event"] = event;
    doc["timestamp"] = millis();
    doc["data"] = serialized(data);

    String message;
    serializeJson(doc, message);

    websocket_->textAll(message);
}

void WebServerModule::set_wifi_credentials(const String& ssid, const String& password) {
    wifi_ssid_ = ssid;
    wifi_password_ = password;
}

void WebServerModule::set_api_auth(const String& username, const String& password) {
    api_username_ = username;
    api_password_ = password;
}

size_t WebServerModule::get_websocket_clients() const {
    return websocket_ ? websocket_->count() : 0;
}

bool WebServerModule::is_wifi_connected() const {
    return wifi_connected_;
}

bool WebServerModule::init_wifi() {
    if (wifi_ssid_.isEmpty()) {
        Serial.println("[WebServer] No WiFi SSID configured");
        return false;
    }

    // Set hostname
    WiFi.setHostname(hostname_.c_str());

    // Connect to WiFi
    Serial.printf("[WebServer] Connecting to WiFi: %s\n", wifi_ssid_.c_str());
    WiFi.begin(wifi_ssid_.c_str(), wifi_password_.c_str());

    // Wait for connection (timeout after 30 seconds)
    uint32_t start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 30000) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        wifi_connected_ = true;
        Serial.printf("[WebServer] Connected! IP: %s\n", WiFi.localIP().toString().c_str());

        // Publish WiFi connected event
        EventData event;
        event.source_module = "WebServerModule";
        EventBus::get_instance().publish(EventType::WIFI_CONNECTED, &event);

        return true;
    } else {
        wifi_connected_ = false;
        Serial.println("[WebServer] WiFi connection failed");
        return false;
    }
}

void WebServerModule::setup_api_endpoints() {
    // System status
    server_->on("/api/system/status", HTTP_GET,
        [this](AsyncWebServerRequest* request) { handle_system_status(request); });

    // Temperature endpoints
    server_->on("/api/temperature/all", HTTP_GET,
        [this](AsyncWebServerRequest* request) { handle_temperature_all(request); });

    // Pump endpoints
    server_->on("^\\/api\\/pump\\/([a-zA-Z0-9_]+)\\/status$", HTTP_GET,
        [this](AsyncWebServerRequest* request) { handle_pump_status(request); });
    server_->on("^\\/api\\/pump\\/([a-zA-Z0-9_]+)\\/speed$", HTTP_POST,
        [this](AsyncWebServerRequest* request) {}, nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t index, size_t total) {
            handle_pump_set_speed(request);
        });

    // Heat pump endpoints
    server_->on("/api/heatpump/status", HTTP_GET,
        [this](AsyncWebServerRequest* request) { handle_heatpump_status(request); });

    // Humidity endpoints
    server_->on("^\\/api\\/humidity\\/([a-zA-Z0-9_]+)\\/status$", HTTP_GET,
        [this](AsyncWebServerRequest* request) { handle_humidity_status(request); });

    // Ventilation endpoints
    server_->on("/api/ventilation/status", HTTP_GET,
        [this](AsyncWebServerRequest* request) { handle_ventilation_status(request); });

    // Storage endpoints
    server_->on("^\\/api\\/storage\\/([a-zA-Z0-9_]+)\\/status$", HTTP_GET,
        [this](AsyncWebServerRequest* request) { handle_storage_status(request); });

    // Config endpoints
    server_->on("/api/config", HTTP_GET,
        [this](AsyncWebServerRequest* request) { handle_config_get(request); });

    // 404 handler
    server_->onNotFound([this](AsyncWebServerRequest* request) { handle_not_found(request); });

    Serial.println("[WebServer] API endpoints configured");
}

void WebServerModule::setup_websocket() {
    // WebSocket is already set up in initialize()
}

void WebServerModule::subscribe_to_events() {
    EventBus& bus = EventBus::get_instance();

    // Subscribe to temperature changes
    auto temp_sub = bus.subscribe(EventType::TEMPERATURE_CHANGED,
        [this](const EventData* data) {
            auto temp_data = static_cast<const TemperatureEventData*>(data);
            DynamicJsonDocument doc(256);
            doc["sensor_id"] = temp_data->sensor_id;
            doc["temperature"] = temp_data->temperature_c;
            String json;
            serializeJson(doc, json);
            broadcast_message("temperature_update", json);
        });
    event_subscriptions_.push_back(temp_sub);

    // Subscribe to safety violations
    auto safety_sub = bus.subscribe(EventType::SAFETY_VIOLATION,
        [this](const EventData* data) {
            auto safety_data = static_cast<const SafetyEventData*>(data);
            DynamicJsonDocument doc(256);
            doc["violation_type"] = safety_data->violation_type;
            doc["component"] = safety_data->affected_component;
            doc["severity"] = safety_data->severity;
            String json;
            serializeJson(doc, json);
            broadcast_message("safety_violation", json);
        });
    event_subscriptions_.push_back(safety_sub);

    Serial.println("[WebServer] Subscribed to system events");
}

// REST API handlers

void WebServerModule::handle_system_status(AsyncWebServerRequest* request) {
    if (!check_auth(request)) return;

    Skeleton& system = Skeleton::get_instance();
    SystemStats stats = system.get_stats();

    DynamicJsonDocument doc(1024);
    doc["state"] = static_cast<int>(stats.state);
    doc["uptime_ms"] = stats.uptime_ms;
    doc["loop_count"] = stats.loop_count;
    doc["avg_loop_time_us"] = stats.average_loop_time_us;
    doc["free_heap"] = stats.free_heap_bytes;
    doc["active_modules"] = stats.active_modules;
    doc["healthy_modules"] = stats.healthy_modules;
    doc["wifi_connected"] = wifi_connected_;
    doc["websocket_clients"] = websocket_->count();

    send_json_response(request, 200, doc);
}

void WebServerModule::handle_temperature_all(AsyncWebServerRequest* request) {
    if (!check_auth(request)) return;

    Skeleton& system = Skeleton::get_instance();
    TemperatureSensorModule* temp_mod =
        static_cast<TemperatureSensorModule*>(system.get_module("TemperatureSensorModule"));

    if (!temp_mod) {
        send_error(request, 404, "Temperature module not found");
        return;
    }

    auto temps = temp_mod->get_all_temperatures();

    DynamicJsonDocument doc(1024);
    JsonArray sensors = doc.createNestedArray("sensors");

    for (const auto& pair : temps) {
        JsonObject sensor = sensors.createNestedObject();
        sensor["name"] = pair.first;
        sensor["temperature_c"] = pair.second;
    }

    send_json_response(request, 200, doc);
}

void WebServerModule::handle_pump_status(AsyncWebServerRequest* request) {
    if (!check_auth(request)) return;

    String pump_name = request->pathArg(0);

    Skeleton& system = Skeleton::get_instance();
    PumpModule* pump_mod = static_cast<PumpModule*>(system.get_module("PumpModule"));

    if (!pump_mod) {
        send_error(request, 404, "Pump module not found");
        return;
    }

    PumpStatus status = pump_mod->get_pump_status(pump_name);

    if (status.has_error) {
        send_error(request, 404, status.error_message);
        return;
    }

    DynamicJsonDocument doc(512);
    doc["name"] = status.name;
    doc["mode"] = static_cast<int>(status.mode);
    doc["state"] = static_cast<int>(status.state);
    doc["target_speed_percent"] = status.target_speed_percent;
    doc["actual_speed_percent"] = status.actual_speed_percent;
    doc["runtime_hours"] = status.total_runtime_hours;

    send_json_response(request, 200, doc);
}

void WebServerModule::handle_pump_set_speed(AsyncWebServerRequest* request) {
    // Body parsing would go here
    send_error(request, 501, "Not implemented yet");
}

void WebServerModule::handle_heatpump_status(AsyncWebServerRequest* request) {
    if (!check_auth(request)) return;

    Skeleton& system = Skeleton::get_instance();
    HeatPumpModule* hp_mod = static_cast<HeatPumpModule*>(system.get_module("HeatPumpModule"));

    if (!hp_mod) {
        send_error(request, 404, "Heat pump module not found");
        return;
    }

    HeatPumpStatus status = hp_mod->get_status();

    DynamicJsonDocument doc(512);
    doc["mode"] = static_cast<int>(status.mode);
    doc["state"] = static_cast<int>(status.state);
    doc["supply_temp_c"] = status.supply_temp_c;
    doc["return_temp_c"] = status.return_temp_c;
    doc["outdoor_temp_c"] = status.outdoor_temp_c;
    doc["cop"] = status.current_cop;
    doc["runtime_hours"] = status.total_runtime_hours;

    send_json_response(request, 200, doc);
}

void WebServerModule::handle_heatpump_set_mode(AsyncWebServerRequest* request) {
    send_error(request, 501, "Not implemented yet");
}

void WebServerModule::handle_humidity_status(AsyncWebServerRequest* request) {
    if (!check_auth(request)) return;

    String zone_name = request->pathArg(0);

    Skeleton& system = Skeleton::get_instance();
    HumidityControlModule* hum_mod =
        static_cast<HumidityControlModule*>(system.get_module("HumidityControlModule"));

    if (!hum_mod) {
        send_error(request, 404, "Humidity module not found");
        return;
    }

    HumidityZoneStatus status = hum_mod->get_zone_status(zone_name);

    if (status.has_error) {
        send_error(request, 404, status.error_message);
        return;
    }

    DynamicJsonDocument doc(512);
    doc["zone_name"] = status.zone_name;
    doc["current_humidity"] = status.current_humidity_percent;
    doc["target_humidity"] = status.target_humidity_percent;
    doc["dewpoint_c"] = status.dewpoint_c;
    doc["mold_risk"] = status.mold_risk;

    send_json_response(request, 200, doc);
}

void WebServerModule::handle_humidity_set_target(AsyncWebServerRequest* request) {
    send_error(request, 501, "Not implemented yet");
}

void WebServerModule::handle_ventilation_status(AsyncWebServerRequest* request) {
    if (!check_auth(request)) return;

    Skeleton& system = Skeleton::get_instance();
    VentilationModule* vent_mod =
        static_cast<VentilationModule*>(system.get_module("VentilationModule"));

    if (!vent_mod) {
        send_error(request, 404, "Ventilation module not found");
        return;
    }

    VentilationStatus status = vent_mod->get_status();

    DynamicJsonDocument doc(512);
    doc["mode"] = static_cast<int>(status.mode);
    doc["supply_speed"] = status.supply_speed_percent;
    doc["exhaust_speed"] = status.exhaust_speed_percent;
    doc["runtime_hours"] = status.runtime_hours;

    send_json_response(request, 200, doc);
}

void WebServerModule::handle_ventilation_set_mode(AsyncWebServerRequest* request) {
    send_error(request, 501, "Not implemented yet");
}

void WebServerModule::handle_storage_status(AsyncWebServerRequest* request) {
    if (!check_auth(request)) return;

    String tank_name = request->pathArg(0);

    Skeleton& system = Skeleton::get_instance();
    HeatStorageModule* storage_mod =
        static_cast<HeatStorageModule*>(system.get_module("HeatStorageModule"));

    if (!storage_mod) {
        send_error(request, 404, "Heat storage module not found");
        return;
    }

    TankStatus status = storage_mod->get_tank_status(tank_name);

    if (status.has_error) {
        send_error(request, 404, status.error_message);
        return;
    }

    DynamicJsonDocument doc(512);
    doc["name"] = status.name;
    doc["state"] = static_cast<int>(status.state);
    doc["top_temp_c"] = status.top_temp_c;
    doc["bottom_temp_c"] = status.bottom_temp_c;
    doc["charge_percent"] = status.charge_percent;
    doc["stored_energy_kwh"] = status.stored_energy_kwh;

    send_json_response(request, 200, doc);
}

void WebServerModule::handle_config_get(AsyncWebServerRequest* request) {
    if (!check_auth(request)) return;

    ConfigManager& config = ConfigManager::get_instance();
    String json = config.export_to_json(true);

    request->send(200, "application/json", json);
}

void WebServerModule::handle_config_set(AsyncWebServerRequest* request) {
    if (!check_auth(request)) return;
    send_error(request, 501, "Config update not implemented yet");
}

void WebServerModule::handle_not_found(AsyncWebServerRequest* request) {
    send_error(request, 404, "Endpoint not found");
}

void WebServerModule::on_websocket_event(AsyncWebSocket* server, AsyncWebSocketClient* client,
                                         AwsEventType type, void* arg, uint8_t* data, size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            Serial.printf("[WebServer] WebSocket client #%u connected from %s\n",
                         client->id(), client->remoteIP().toString().c_str());
            break;

        case WS_EVT_DISCONNECT:
            Serial.printf("[WebServer] WebSocket client #%u disconnected\n", client->id());
            break;

        case WS_EVT_DATA:
            // Handle incoming WebSocket data if needed
            break;

        case WS_EVT_PONG:
        case WS_EVT_ERROR:
            break;
    }
}

bool WebServerModule::check_auth(AsyncWebServerRequest* request) {
    // Basic authentication check
    if (!request->authenticate(api_username_.c_str(), api_password_.c_str())) {
        send_auth_required(request);
        return false;
    }
    return true;
}

void WebServerModule::send_auth_required(AsyncWebServerRequest* request) {
    request->requestAuthentication();
}

void WebServerModule::send_json_response(AsyncWebServerRequest* request, int code,
                                         const JsonDocument& doc) {
    String response;
    serializeJson(doc, response);
    request->send(code, "application/json", response);
}

void WebServerModule::send_error(AsyncWebServerRequest* request, int code, const String& message) {
    DynamicJsonDocument doc(256);
    doc["error"] = message;
    doc["code"] = code;
    send_json_response(request, code, doc);
}
