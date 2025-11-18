/**
 * @file ConfigManager.h
 * @purpose Configuration management with versioning, backup, and validation
 * @dependencies ArduinoJson, SPIFFS/LittleFS, EventBus.h
 * @version 1.0.0
 * @performance_notes Config load: ~50-100ms, Save: ~100-200ms
 *
 * The ConfigManager provides centralized configuration management with:
 * - Automatic backup before changes
 * - Configuration versioning
 * - Migration scripts for version upgrades
 * - Rollback capability
 * - Default factory settings
 *
 * Configuration is stored in JSON format on SPIFFS/LittleFS filesystem.
 * Each config change triggers a backup of the previous version.
 */

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <SPIFFS.h>
#include "EventBus.h"
#include <map>

/**
 * Configuration sections
 */
enum class ConfigSection {
    SYSTEM,             // System-wide settings
    NETWORK,            // WiFi, MQTT, network settings
    HEAT_PUMP,          // Heat pump configuration
    PUMPS,              // Circulation pumps configuration
    VENTILATION,        // Ventilation system configuration
    HEAT_STORAGE,       // Heat storage tanks configuration
    SENSORS,            // Sensor configuration
    SAFETY,             // Safety limits and parameters
    UI,                 // User interface settings
    CLOUD,              // Cloud integration settings
    ADVANCED            // Advanced/expert settings
};

/**
 * Configuration validation result
 */
struct ConfigValidation {
    bool is_valid;
    std::vector<String> errors;
    std::vector<String> warnings;
};

/**
 * @class ConfigManager
 * @brief Manages system configuration with versioning and backup
 *
 * The ConfigManager handles all configuration persistence, validation,
 * and migration. It ensures configuration is always in a valid state
 * and provides rollback capability if changes cause issues.
 *
 * Configuration File Structure:
 * /config/
 *   ├── system.json           (current configuration)
 *   ├── system.json.backup    (previous version)
 *   ├── system.json.factory   (factory defaults)
 *   └── version.txt           (configuration version)
 *
 * Configuration Lifecycle:
 * 1. Load config from file (or create from defaults)
 * 2. Validate configuration
 * 3. If invalid, attempt migration
 * 4. If migration fails, load backup
 * 5. If backup fails, load factory defaults
 *
 * Usage Example:
 * ConfigManager& config = ConfigManager::get_instance();
 * config.initialize();
 *
 * // Get a value
 * float max_temp = config.get_float(ConfigSection::HEAT_PUMP, "max_temp", 65.0);
 *
 * // Set a value (automatically backs up old config)
 * config.set_float(ConfigSection::HEAT_PUMP, "max_temp", 70.0);
 * config.save(); // Persist to filesystem
 */
class ConfigManager {
public:
    /**
     * Get singleton instance
     * @return Reference to global ConfigManager instance
     */
    static ConfigManager& get_instance() {
        static ConfigManager instance;
        return instance;
    }

    /**
     * Initialize configuration manager
     * @param format_if_failed If true, format filesystem if mount fails
     * @return true if initialization successful
     */
    bool initialize(bool format_if_failed = false);

    /**
     * Load configuration from filesystem
     * @return true if loaded successfully
     * @note Falls back to backup, then factory defaults if load fails
     */
    bool load();

    /**
     * Save configuration to filesystem
     * @param create_backup If true, backup current config before saving
     * @return true if saved successfully
     */
    bool save(bool create_backup = true);

    /**
     * Restore configuration from backup
     * @return true if restored successfully
     */
    bool restore_from_backup();

    /**
     * Reset to factory defaults
     * @return true if reset successful
     */
    bool reset_to_factory_defaults();

    /**
     * Validate current configuration
     * @return Validation result with errors/warnings
     */
    ConfigValidation validate() const;

    /**
     * Get string value from configuration
     * @param section Configuration section
     * @param key Configuration key
     * @param default_value Default value if key not found
     * @return Configuration value or default
     */
    String get_string(ConfigSection section, const String& key, const String& default_value = "") const;

    /**
     * Get integer value from configuration
     */
    int get_int(ConfigSection section, const String& key, int default_value = 0) const;

    /**
     * Get float value from configuration
     */
    float get_float(ConfigSection section, const String& key, float default_value = 0.0f) const;

    /**
     * Get boolean value from configuration
     */
    bool get_bool(ConfigSection section, const String& key, bool default_value = false) const;

    /**
     * Set string value in configuration
     * @note Configuration must be saved to persist changes
     */
    void set_string(ConfigSection section, const String& key, const String& value);

    /**
     * Set integer value in configuration
     */
    void set_int(ConfigSection section, const String& key, int value);

    /**
     * Set float value in configuration
     */
    void set_float(ConfigSection section, const String& key, float value);

    /**
     * Set boolean value in configuration
     */
    void set_bool(ConfigSection section, const String& key, bool value);

    /**
     * Check if key exists in configuration
     */
    bool has_key(ConfigSection section, const String& key) const;

    /**
     * Remove a key from configuration
     */
    void remove_key(ConfigSection section, const String& key);

    /**
     * Get entire section as JSON object
     * @param section Section to retrieve
     * @return JSON object with section data
     */
    JsonObject get_section(ConfigSection section) const;

    /**
     * Get configuration version
     * @return Current configuration version (e.g., "1.0.0")
     */
    String get_version() const { return version_; }

    /**
     * Export configuration to JSON string
     * @param pretty If true, format with indentation
     * @return JSON string
     */
    String export_to_json(bool pretty = true) const;

    /**
     * Import configuration from JSON string
     * @param json JSON string to import
     * @return true if import successful
     */
    bool import_from_json(const String& json);

    /**
     * List all backup files
     * @return Vector of backup file paths
     */
    std::vector<String> list_backups() const;

    /**
     * Get configuration statistics
     */
    struct Stats {
        size_t total_keys;
        size_t file_size_bytes;
        uint32_t last_save_ms;
        uint32_t last_load_ms;
        uint32_t save_count;
    };
    Stats get_stats() const { return stats_; }

    // Prevent copying
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

private:
    ConfigManager() : initialized_(false), version_("1.0.0") {}

    /**
     * Load factory default configuration
     */
    void load_factory_defaults();

    /**
     * Get section name as string
     */
    static String section_to_string(ConfigSection section);

    /**
     * Get or create section in JSON document
     */
    JsonObject get_or_create_section(ConfigSection section);

    /**
     * Migrate configuration from old version to current
     * @param from_version Source version
     * @return true if migration successful
     */
    bool migrate_config(const String& from_version);

    // Configuration document (in-memory)
    DynamicJsonDocument config_doc_;

    // Configuration version
    String version_;

    // Initialization state
    bool initialized_;

    // Statistics
    Stats stats_;

    // File paths
    static const char* CONFIG_FILE;
    static const char* BACKUP_FILE;
    static const char* FACTORY_FILE;
    static const char* VERSION_FILE;

    // JSON document size (bytes)
    static const size_t JSON_DOC_SIZE = 8192;
};

#endif // CONFIG_MANAGER_H
