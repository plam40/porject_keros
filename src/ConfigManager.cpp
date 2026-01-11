/**
 * @file ConfigManager.cpp
 * @purpose Implementation of configuration management system
 * @dependencies ConfigManager.h, ArduinoJson, SPIFFS
 * @version 1.0.0
 * @performance_notes Uses SPIFFS for persistent storage, ~100ms for save operations
 */

#include "ConfigManager.h"

// Static member initialization
const char* ConfigManager::CONFIG_FILE = "/config/system.json";
const char* ConfigManager::BACKUP_FILE = "/config/system.json.backup";
const char* ConfigManager::FACTORY_FILE = "/config/system.json.factory";
const char* ConfigManager::VERSION_FILE = "/config/version.txt";

bool ConfigManager::initialize(bool format_if_failed) {
    if (initialized_) {
        return true;
    }

    // Initialize SPIFFS
    if (!SPIFFS.begin(true)) {
        Serial.println("[ConfigManager] ERROR: Failed to mount SPIFFS");
        if (format_if_failed) {
            Serial.println("[ConfigManager] Formatting SPIFFS...");
            if (!SPIFFS.format()) {
                Serial.println("[ConfigManager] ERROR: Failed to format SPIFFS");
                return false;
            }
            if (!SPIFFS.begin(true)) {
                Serial.println("[ConfigManager] ERROR: Failed to mount SPIFFS after format");
                return false;
            }
        } else {
            return false;
        }
    }

    // Create config directory if it doesn't exist
    if (!SPIFFS.exists("/config")) {
        Serial.println("[ConfigManager] Creating /config directory");
        // SPIFFS doesn't have directories, but we use path prefixes
    }

    // Initialize JSON document
    config_doc_ = DynamicJsonDocument(JSON_DOC_SIZE);

    // Load configuration
    if (!load()) {
        Serial.println("[ConfigManager] WARNING: Failed to load config, using factory defaults");
        load_factory_defaults();
    }

    initialized_ = true;
    stats_.last_load_ms = millis();

    Serial.printf("[ConfigManager] Initialized (version %s, %d keys, %d bytes free)\n",
                 version_.c_str(), stats_.total_keys,
                 SPIFFS.totalBytes() - SPIFFS.usedBytes());

    return true;
}

bool ConfigManager::load() {
    File file = SPIFFS.open(CONFIG_FILE, "r");
    if (!file) {
        Serial.println("[ConfigManager] Config file not found");
        return false;
    }

    DeserializationError error = deserializeJson(config_doc_, file);
    file.close();

    if (error) {
        Serial.printf("[ConfigManager] ERROR: Failed to parse config file: %s\n",
                     error.c_str());
        return false;
    }

    // Validate configuration
    ConfigValidation validation = validate();
    if (!validation.is_valid) {
        Serial.println("[ConfigManager] ERROR: Configuration validation failed:");
        for (const auto& err : validation.errors) {
            Serial.printf("  - %s\n", err.c_str());
        }
        return false;
    }

    stats_.last_load_ms = millis();
    Serial.println("[ConfigManager] Configuration loaded successfully");

    return true;
}

bool ConfigManager::save(bool create_backup) {
    if (!initialized_) {
        Serial.println("[ConfigManager] ERROR: Cannot save, not initialized");
        return false;
    }

    // Create backup of current config if requested
    if (create_backup && SPIFFS.exists(CONFIG_FILE)) {
        if (SPIFFS.exists(BACKUP_FILE)) {
            SPIFFS.remove(BACKUP_FILE);
        }
        File src = SPIFFS.open(CONFIG_FILE, "r");
        File dst = SPIFFS.open(BACKUP_FILE, "w");
        if (src && dst) {
            while (src.available()) {
                dst.write(src.read());
            }
            src.close();
            dst.close();
            Serial.println("[ConfigManager] Backup created");
        }
    }

    // Save configuration
    File file = SPIFFS.open(CONFIG_FILE, "w");
    if (!file) {
        Serial.println("[ConfigManager] ERROR: Failed to open config file for writing");
        return false;
    }

    if (serializeJson(config_doc_, file) == 0) {
        Serial.println("[ConfigManager] ERROR: Failed to write config file");
        file.close();
        return false;
    }

    file.close();
    stats_.last_save_ms = millis();
    stats_.save_count++;

    Serial.printf("[ConfigManager] Configuration saved (%d bytes)\n",
                 measureJson(config_doc_));

    // Publish config changed event
    EventData event;
    event.source_module = "ConfigManager";
    EventBus::get_instance().publish(EventType::CONFIG_CHANGED, &event);

    return true;
}

bool ConfigManager::restore_from_backup() {
    if (!SPIFFS.exists(BACKUP_FILE)) {
        Serial.println("[ConfigManager] ERROR: No backup file found");
        return false;
    }

    File file = SPIFFS.open(BACKUP_FILE, "r");
    if (!file) {
        Serial.println("[ConfigManager] ERROR: Failed to open backup file");
        return false;
    }

    DeserializationError error = deserializeJson(config_doc_, file);
    file.close();

    if (error) {
        Serial.printf("[ConfigManager] ERROR: Failed to parse backup file: %s\n",
                     error.c_str());
        return false;
    }

    Serial.println("[ConfigManager] Configuration restored from backup");
    return save(false);  // Save restored config (don't create backup of backup)
}

bool ConfigManager::reset_to_factory_defaults() {
    load_factory_defaults();
    return save(true);
}

ConfigValidation ConfigManager::validate() const {
    ConfigValidation result;
    result.is_valid = true;

    // Basic validation - check that required sections exist
    const char* required_sections[] = {"system", "network", "safety"};
    for (const char* section : required_sections) {
        if (!config_doc_.containsKey(section)) {
            result.errors.push_back(String("Missing required section: ") + section);
            result.is_valid = false;
        }
    }

    // Validate safety limits
    if (config_doc_.containsKey("safety")) {
        JsonObject safety = config_doc_["safety"];

        // Check temperature limits are reasonable
        if (safety.containsKey("max_supply_temp")) {
            float max_temp = safety["max_supply_temp"];
            if (max_temp < 30.0 || max_temp > 90.0) {
                result.warnings.push_back("max_supply_temp out of typical range (30-90°C)");
            }
        }
    }

    return result;
}

String ConfigManager::get_string(ConfigSection section, const String& key,
                                const String& default_value) const {
    String section_name = section_to_string(section);
    if (!config_doc_.containsKey(section_name) ||
        !config_doc_[section_name].containsKey(key)) {
        return default_value;
    }
    return config_doc_[section_name][key].as<String>();
}

int ConfigManager::get_int(ConfigSection section, const String& key,
                           int default_value) const {
    String section_name = section_to_string(section);
    if (!config_doc_.containsKey(section_name) ||
        !config_doc_[section_name].containsKey(key)) {
        return default_value;
    }
    return config_doc_[section_name][key].as<int>();
}

float ConfigManager::get_float(ConfigSection section, const String& key,
                               float default_value) const {
    String section_name = section_to_string(section);
    if (!config_doc_.containsKey(section_name) ||
        !config_doc_[section_name].containsKey(key)) {
        return default_value;
    }
    return config_doc_[section_name][key].as<float>();
}

bool ConfigManager::get_bool(ConfigSection section, const String& key,
                             bool default_value) const {
    String section_name = section_to_string(section);
    if (!config_doc_.containsKey(section_name) ||
        !config_doc_[section_name].containsKey(key)) {
        return default_value;
    }
    return config_doc_[section_name][key].as<bool>();
}

void ConfigManager::set_string(ConfigSection section, const String& key,
                               const String& value) {
    JsonObject section_obj = get_or_create_section(section);
    section_obj[key] = value;
}

void ConfigManager::set_int(ConfigSection section, const String& key, int value) {
    JsonObject section_obj = get_or_create_section(section);
    section_obj[key] = value;
}

void ConfigManager::set_float(ConfigSection section, const String& key, float value) {
    JsonObject section_obj = get_or_create_section(section);
    section_obj[key] = value;
}

void ConfigManager::set_bool(ConfigSection section, const String& key, bool value) {
    JsonObject section_obj = get_or_create_section(section);
    section_obj[key] = value;
}

bool ConfigManager::has_key(ConfigSection section, const String& key) const {
    String section_name = section_to_string(section);
    return config_doc_.containsKey(section_name) &&
           config_doc_[section_name].containsKey(key);
}

String ConfigManager::export_to_json(bool pretty) const {
    String output;
    if (pretty) {
        serializeJsonPretty(config_doc_, output);
    } else {
        serializeJson(config_doc_, output);
    }
    return output;
}

bool ConfigManager::import_from_json(const String& json) {
    DynamicJsonDocument temp_doc(JSON_DOC_SIZE);
    DeserializationError error = deserializeJson(temp_doc, json);

    if (error) {
        Serial.printf("[ConfigManager] ERROR: Failed to parse JSON: %s\n",
                     error.c_str());
        return false;
    }

    config_doc_ = temp_doc;
    return true;
}

void ConfigManager::load_factory_defaults() {
    config_doc_.clear();

    // System section
    JsonObject system = config_doc_.createNestedObject("system");
    system["device_name"] = "HVAC-Controller";
    system["location"] = "Home";
    system["timezone"] = "UTC";

    // Network section
    JsonObject network = config_doc_.createNestedObject("network");
    network["wifi_ssid"] = "";
    network["wifi_password"] = "";
    network["hostname"] = "hvac-controller";
    network["mqtt_broker"] = "";
    network["mqtt_port"] = 1883;

    // Heat pump section
    JsonObject heat_pump = config_doc_.createNestedObject("heat_pump");
    heat_pump["enabled"] = true;
    heat_pump["max_supply_temp"] = 65.0;
    heat_pump["min_outdoor_temp"] = -15.0;
    heat_pump["defrost_enable_temp"] = 5.0;
    heat_pump["min_runtime_seconds"] = 300;
    heat_pump["min_off_time_seconds"] = 300;

    // Pumps section
    JsonObject pumps = config_doc_.createNestedObject("pumps");
    pumps["primary_enabled"] = true;
    pumps["primary_min_speed"] = 30;
    pumps["primary_max_speed"] = 100;

    // Safety section
    JsonObject safety = config_doc_.createNestedObject("safety");
    safety["max_supply_temp"] = 65.0;
    safety["max_return_temp"] = 50.0;
    safety["max_tank_temp"] = 80.0;
    safety["min_outdoor_temp"] = -30.0;
    safety["max_system_pressure"] = 3.0;
    safety["min_system_pressure"] = 0.5;

    Serial.println("[ConfigManager] Factory defaults loaded");
}

String ConfigManager::section_to_string(ConfigSection section) {
    switch (section) {
        case ConfigSection::SYSTEM: return "system";
        case ConfigSection::NETWORK: return "network";
        case ConfigSection::HEAT_PUMP: return "heat_pump";
        case ConfigSection::PUMPS: return "pumps";
        case ConfigSection::VENTILATION: return "ventilation";
        case ConfigSection::HEAT_STORAGE: return "heat_storage";
        case ConfigSection::SENSORS: return "sensors";
        case ConfigSection::SAFETY: return "safety";
        case ConfigSection::UI: return "ui";
        case ConfigSection::CLOUD: return "cloud";
        case ConfigSection::ADVANCED: return "advanced";
        default: return "unknown";
    }
}

JsonObject ConfigManager::get_or_create_section(ConfigSection section) {
    String section_name = section_to_string(section);
    if (!config_doc_.containsKey(section_name)) {
        return config_doc_.createNestedObject(section_name);
    }
    return config_doc_[section_name];
}

std::vector<String> ConfigManager::list_backups() const {
    std::vector<String> backups;
    if (SPIFFS.exists(BACKUP_FILE)) {
        backups.push_back(BACKUP_FILE);
    }
    if (SPIFFS.exists(FACTORY_FILE)) {
        backups.push_back(FACTORY_FILE);
    }
    return backups;
}

void ConfigManager::remove_key(ConfigSection section, const String& key) {
    String section_name = section_to_string(section);
    if (config_doc_.containsKey(section_name) &&
        config_doc_[section_name].containsKey(key)) {
        config_doc_[section_name].remove(key);
        Serial.printf("[ConfigManager] Removed key: %s.%s\n",
                     section_name.c_str(), key.c_str());
    }
}

JsonObject ConfigManager::get_section(ConfigSection section) const {
    String section_name = section_to_string(section);
    if (config_doc_.containsKey(section_name)) {
        return config_doc_[section_name];
    }
    // Return empty object if section doesn't exist
    return JsonObject();
}

bool ConfigManager::migrate_config(const String& from_version) {
    Serial.printf("[ConfigManager] Migrating config from version %s to %s\n",
                 from_version.c_str(), version_.c_str());

    // Version migration logic
    // Currently no migrations needed, but structure is in place for future use
    if (from_version == version_) {
        return true;  // Same version, no migration needed
    }

    // Example migration pattern:
    // if (from_version == "0.9.0") {
    //     // Migrate from 0.9.0 to 1.0.0
    //     // Add new required fields with defaults
    //     if (!has_key(ConfigSection::SYSTEM, "new_field")) {
    //         set_string(ConfigSection::SYSTEM, "new_field", "default_value");
    //     }
    // }

    Serial.println("[ConfigManager] Migration complete");
    return true;
}
