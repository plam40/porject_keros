/**
 * @file EventBus.h
 * @purpose Event-driven communication system for inter-module messaging
 * @dependencies ModuleInterface.h
 * @version 1.0.0
 * @last_modified 2025-12-11
 * @author Keros Development Team
 * @performance_notes Event dispatch is O(n) where n = number of subscribers to that event type
 *                     Typical dispatch time: 50-200μs for 10 subscribers
 * @module_type SKELETON
 *
 * The EventBus provides decoupled communication between modules. Modules can publish
 * events without knowing which modules will consume them, enabling loose coupling
 * and easier testing/mocking.
 *
 * Key Design Decisions:
 * - Uses callbacks instead of message queues for lower latency
 * - Events are processed synchronously to maintain deterministic timing
 * - Maximum 50 subscribers per event type to prevent performance degradation
 */

#ifndef EVENT_BUS_H
#define EVENT_BUS_H

#include <Arduino.h>
#include <functional>
#include <map>
#include <vector>

/**
 * Event types in the HVAC system
 * Add new event types as needed for new modules
 */
enum class EventType {
    // System events
    SYSTEM_STARTUP,
    SYSTEM_SHUTDOWN,
    SYSTEM_ERROR,
    CONFIG_CHANGED,

    // Temperature events
    TEMPERATURE_CHANGED,
    TEMPERATURE_THRESHOLD_EXCEEDED,
    TEMPERATURE_SENSOR_ERROR,

    // Heat pump events
    COMPRESSOR_STATE_CHANGED,
    DEFROST_STARTED,
    DEFROST_COMPLETED,
    COP_UPDATED,

    // Pump events
    PUMP_STATE_CHANGED,
    PUMP_SPEED_CHANGED,
    PUMP_ERROR,

    // Ventilation events
    FAN_STATE_CHANGED,
    AIR_QUALITY_CHANGED,
    FILTER_MAINTENANCE_REQUIRED,

    // Humidity events
    HUMIDITY_CHANGED,
    HUMIDITY_THRESHOLD_EXCEEDED,
    MOLD_RISK_DETECTED,

    // Heat storage events
    TANK_TEMPERATURE_CHANGED,
    TANK_CHARGE_COMPLETE,
    TANK_DISCHARGE_COMPLETE,
    STRATIFICATION_DEGRADED,

    // Safety events
    SAFETY_VIOLATION,
    WATCHDOG_TIMEOUT,
    EMERGENCY_SHUTDOWN,

    // User interface events
    USER_COMMAND_RECEIVED,
    WEB_CLIENT_CONNECTED,
    WEB_CLIENT_DISCONNECTED,

    // Network events
    WIFI_CONNECTED,
    WIFI_DISCONNECTED,
    MQTT_CONNECTED,
    MQTT_DISCONNECTED
};

/**
 * Base event data structure
 * All event payloads should extend this structure
 */
struct EventData {
    uint32_t timestamp_ms;          // Event timestamp
    String source_module;           // Module that published the event
    uint8_t priority;               // Event priority (0=lowest, 255=highest)

    EventData() : timestamp_ms(millis()), priority(100) {}
    virtual ~EventData() = default;
};

/**
 * Temperature event data
 */
struct TemperatureEventData : public EventData {
    String sensor_id;
    float temperature_c;
    float previous_temperature_c;
};

/**
 * State change event data
 */
struct StateChangeEventData : public EventData {
    String component_id;
    String previous_state;
    String new_state;
    String reason;
};

/**
 * Safety event data
 */
struct SafetyEventData : public EventData {
    String violation_type;
    String affected_component;
    uint8_t severity;               // 0=info, 1=warning, 2=error, 3=critical
    String remediation_action;
};

/**
 * Numeric value event data
 */
struct ValueEventData : public EventData {
    String parameter_name;
    float value;
    float previous_value;
    String units;
};

// Event callback type
using EventCallback = std::function<void(const EventData*)>;

/**
 * @class EventBus
 * @brief Singleton event bus for publish-subscribe messaging
 *
 * The EventBus enables loose coupling between modules through event-driven
 * communication. Modules can publish events and subscribe to events from
 * other modules without direct dependencies.
 *
 * Thread Safety: This implementation is NOT thread-safe. All event publishing
 * and subscribing must happen from the same thread (main loop thread).
 *
 * Performance Characteristics:
 * - subscribe(): O(1)
 * - unsubscribe(): O(n) where n = subscribers to that event type
 * - publish(): O(n) where n = subscribers to that event type
 *
 * Usage Example:
 * // Subscribe to temperature events
 * EventBus::get_instance().subscribe(EventType::TEMPERATURE_CHANGED,
 *     [](const EventData* data) {
 *         auto temp_data = static_cast<const TemperatureEventData*>(data);
 *         Serial.printf("Temp: %.2f°C\n", temp_data->temperature_c);
 *     });
 *
 * // Publish temperature event
 * TemperatureEventData event;
 * event.sensor_id = "tank_top";
 * event.temperature_c = 45.5;
 * EventBus::get_instance().publish(EventType::TEMPERATURE_CHANGED, &event);
 */
class EventBus {
public:
    /**
     * Get singleton instance
     * @return Reference to the global EventBus instance
     */
    static EventBus& get_instance() {
        static EventBus instance;
        return instance;
    }

    /**
     * Subscribe to an event type
     * @param type Event type to subscribe to
     * @param callback Function to call when event is published
     * @return Subscription ID (use for unsubscribe)
     * @note Maximum 50 subscribers per event type
     */
    uint32_t subscribe(EventType type, EventCallback callback);

    /**
     * Unsubscribe from an event type
     * @param type Event type to unsubscribe from
     * @param subscription_id ID returned from subscribe()
     * @return true if unsubscribed successfully
     */
    bool unsubscribe(EventType type, uint32_t subscription_id);

    /**
     * Publish an event
     * @param type Event type
     * @param data Event data (lifetime must exceed this call)
     * @note Calls all subscribers synchronously
     * @note If a subscriber throws, other subscribers still get called
     */
    void publish(EventType type, const EventData* data);

    /**
     * Get number of subscribers for an event type
     * @param type Event type
     * @return Number of subscribers
     */
    size_t get_subscriber_count(EventType type) const;

    /**
     * Clear all subscriptions (used for testing/reset)
     */
    void clear_all_subscriptions();

    /**
     * Get event statistics
     * @return Map of event type to publish count
     */
    std::map<EventType, uint32_t> get_statistics() const { return event_counts_; }

    // Prevent copying
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

private:
    EventBus() : next_subscription_id_(1) {}

    struct Subscription {
        uint32_t id;
        EventCallback callback;
    };

    // Map of event type to list of subscriptions
    std::map<EventType, std::vector<Subscription>> subscriptions_;

    // Next subscription ID to assign
    uint32_t next_subscription_id_;

    // Event statistics
    std::map<EventType, uint32_t> event_counts_;

    // Maximum subscribers per event type
    static const size_t MAX_SUBSCRIBERS_PER_EVENT = 50;
};

#endif // EVENT_BUS_H
