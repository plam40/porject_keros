/**
 * @file EventBus.cpp
 * @purpose Implementation of event-driven communication system
 * @dependencies EventBus.h
 * @version 1.0.0
 * @performance_notes Optimized for minimal latency with direct callback invocation
 */

#include "EventBus.h"

uint32_t EventBus::subscribe(EventType type, EventCallback callback) {
    // Check if we've hit the subscriber limit for this event type
    if (subscriptions_[type].size() >= MAX_SUBSCRIBERS_PER_EVENT) {
        Serial.printf("[EventBus] ERROR: Maximum subscribers (%d) reached for event type\n",
                     MAX_SUBSCRIBERS_PER_EVENT);
        return 0;
    }

    // Create new subscription
    Subscription sub;
    sub.id = next_subscription_id_++;
    sub.callback = callback;

    // Add to subscriptions list
    subscriptions_[type].push_back(sub);

    Serial.printf("[EventBus] Subscribed ID %u to event type %d (%d total subscribers)\n",
                 sub.id, static_cast<int>(type), subscriptions_[type].size());

    return sub.id;
}

bool EventBus::unsubscribe(EventType type, uint32_t subscription_id) {
    auto it = subscriptions_.find(type);
    if (it == subscriptions_.end()) {
        return false;
    }

    // Find and remove the subscription
    auto& subs = it->second;
    for (auto sub_it = subs.begin(); sub_it != subs.end(); ++sub_it) {
        if (sub_it->id == subscription_id) {
            subs.erase(sub_it);
            Serial.printf("[EventBus] Unsubscribed ID %u from event type %d\n",
                         subscription_id, static_cast<int>(type));
            return true;
        }
    }

    return false;
}

void EventBus::publish(EventType type, const EventData* data) {
    // Track event count
    event_counts_[type]++;

    // Get subscribers for this event type
    auto it = subscriptions_.find(type);
    if (it == subscriptions_.end() || it->second.empty()) {
        // No subscribers for this event
        return;
    }

    // Call all subscribers
    const auto& subs = it->second;
    for (const auto& sub : subs) {
        try {
            sub.callback(data);
        } catch (...) {
            // Catch any exceptions to prevent one subscriber from breaking others
            Serial.printf("[EventBus] ERROR: Exception in event callback for type %d\n",
                         static_cast<int>(type));
        }
    }
}

size_t EventBus::get_subscriber_count(EventType type) const {
    auto it = subscriptions_.find(type);
    if (it == subscriptions_.end()) {
        return 0;
    }
    return it->second.size();
}

void EventBus::clear_all_subscriptions() {
    subscriptions_.clear();
    event_counts_.clear();
    next_subscription_id_ = 1;
    Serial.println("[EventBus] All subscriptions cleared");
}
