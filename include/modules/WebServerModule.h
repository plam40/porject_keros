/**
 * @file WebServerModule.h
 * @purpose Web server with REST API and WebSocket for remote monitoring/control
 * @dependencies ModuleInterface.h, AsyncWebServer, AsyncWebSocket, ArduinoJson
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes HTTP request: 5-20ms, WebSocket broadcast: 2-10ms
 * @module_type MODULE
 *
 * This module provides:
 * - REST API for all HVAC modules (GET/POST endpoints)
 * - WebSocket for real-time monitoring and updates
 * - JSON responses for easy integration
 * - Basic authentication
 * - CORS support for web dashboards
 * - OTA update support
 *
 * REST API Endpoints:
 * - GET  /api/system/status         - System overview
 * - GET  /api/temperature/all       - All temperature readings
 * - GET  /api/pump/:name/status     - Pump status
 * - POST /api/pump/:name/speed      - Set pump speed
 * - GET  /api/heatpump/status       - Heat pump status
 * - POST /api/heatpump/mode         - Set heat pump mode
 * - GET  /api/humidity/:zone/status - Humidity zone status
 * - POST /api/humidity/:zone/target - Set target humidity
 * - GET  /api/ventilation/status    - Ventilation status
 * - POST /api/ventilation/mode      - Set ventilation mode
 * - GET  /api/storage/:tank/status  - Heat storage status
 * - GET  /api/config                - Get configuration
 * - POST /api/config                - Update configuration
 *
 * WebSocket Events:
 * - temperature_update    - Temperature changed
 * - pump_state_change     - Pump state changed
 * - heatpump_state_change - Heat pump state changed
 * - humidity_update       - Humidity changed
 * - safety_violation      - Safety violation occurred
 * - system_alert          - System alert
 */

#ifndef WEB_SERVER_MODULE_H
#define WEB_SERVER_MODULE_H

#include "ModuleInterface.h"
#include "EventBus.h"
#include <ESPAsyncWebServer.h>
#include <AsyncWebSocket.h>
#include <ArduinoJson.h>

/**
 * @class WebServerModule
 * @brief Web server with REST API and WebSocket support
 *
 * Provides web-based remote monitoring and control of the HVAC system.
 * Includes both REST API for request-response operations and WebSocket
 * for real-time push updates.
 *
 * Configuration Example:
 * network:
 *   wifi_ssid: "YourNetwork"
 *   wifi_password: "YourPassword"
 *   hostname: "hvac-controller"
 *   web_port: 80
 *   api_username: "admin"
 *   api_password: "changeme"
 *
 * Usage Example:
 * WebServerModule web_server;
 * system.register_module(&web_server, 200);
 *
 * // REST API examples:
 * GET  http://hvac-controller/api/system/status
 * POST http://hvac-controller/api/pump/primary/speed
 *      Body: {"speed": 75}
 *
 * // WebSocket connection:
 * ws://hvac-controller/ws
 */
class WebServerModule : public ModuleInterface {
public:
    WebServerModule();
    ~WebServerModule() override;

    // ModuleInterface implementation
    ModuleMetadata get_metadata() const override;
    HealthStatus health_check() override;
    bool initialize() override;
    void shutdown() override;
    String get_api_version() const override { return "1.0"; }

    /**
     * Update web server (process WebSocket events, etc.)
     */
    void update();

    /**
     * Broadcast message to all WebSocket clients
     */
    void broadcast_message(const String& event, const String& data);

    /**
     * Set WiFi credentials
     */
    void set_wifi_credentials(const String& ssid, const String& password);

    /**
     * Set API authentication
     */
    void set_api_auth(const String& username, const String& password);

    /**
     * Get number of connected WebSocket clients
     */
    size_t get_websocket_clients() const;

    /**
     * Check if WiFi is connected
     */
    bool is_wifi_connected() const;

private:
    /**
     * Initialize WiFi connection
     */
    bool init_wifi();

    /**
     * Setup REST API endpoints
     */
    void setup_api_endpoints();

    /**
     * Setup WebSocket handlers
     */
    void setup_websocket();

    /**
     * Subscribe to system events
     */
    void subscribe_to_events();

    // REST API handlers
    void handle_system_status(AsyncWebServerRequest* request);
    void handle_temperature_all(AsyncWebServerRequest* request);
    void handle_pump_status(AsyncWebServerRequest* request);
    void handle_pump_set_speed(AsyncWebServerRequest* request);
    void handle_heatpump_status(AsyncWebServerRequest* request);
    void handle_heatpump_set_mode(AsyncWebServerRequest* request);
    void handle_humidity_status(AsyncWebServerRequest* request);
    void handle_humidity_set_target(AsyncWebServerRequest* request);
    void handle_ventilation_status(AsyncWebServerRequest* request);
    void handle_ventilation_set_mode(AsyncWebServerRequest* request);
    void handle_storage_status(AsyncWebServerRequest* request);
    void handle_config_get(AsyncWebServerRequest* request);
    void handle_config_set(AsyncWebServerRequest* request);
    void handle_not_found(AsyncWebServerRequest* request);

    // WebSocket handlers
    void on_websocket_event(AsyncWebSocket* server, AsyncWebSocketClient* client,
                           AwsEventType type, void* arg, uint8_t* data, size_t len);

    // Authentication
    bool check_auth(AsyncWebServerRequest* request);
    void send_auth_required(AsyncWebServerRequest* request);

    // Helper functions
    void send_json_response(AsyncWebServerRequest* request, int code, const JsonDocument& doc);
    void send_error(AsyncWebServerRequest* request, int code, const String& message);

    // Web server and WebSocket
    AsyncWebServer* server_;
    AsyncWebSocket* websocket_;

    // WiFi credentials
    String wifi_ssid_;
    String wifi_password_;
    String hostname_;

    // Authentication
    String api_username_;
    String api_password_;

    // Server port
    uint16_t web_port_;

    // Connection tracking
    bool wifi_connected_;
    uint32_t last_wifi_check_ms_;

    // Event subscriptions (event type + subscription ID for proper unsubscription)
    struct EventSubscription {
        EventType type;
        uint32_t id;
    };
    std::vector<EventSubscription> event_subscriptions_;
};

#endif // WEB_SERVER_MODULE_H
