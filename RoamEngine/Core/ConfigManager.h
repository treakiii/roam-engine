// ConfigManager.h - The keeper of settings
// This loads and saves all our precious configuration

#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <string>
#include <unordered_map>
#include <variant>

// Config value types - because settings can be different things
using ConfigValue = std::variant<int, float, bool, std::string>;

// The ConfigManager class - our settings guru
class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    // Load config from file - read the sacred text
    bool LoadConfig(const std::string& filename);

    // Save config to file - preserve our wisdom
    bool SaveConfig(const std::string& filename = "");

    // Set config values - change the world
    void SetInt(const std::string& key, int value);
    void SetFloat(const std::string& key, float value);
    void SetBool(const std::string& key, bool value);
    void SetString(const std::string& key, const std::string& value);

    // Get config values - consult the oracle
    int GetInt(const std::string& key, int defaultValue = 0) const;
    float GetFloat(const std::string& key, float defaultValue = 0.0f) const;
    bool GetBool(const std::string& key, bool defaultValue = false) const;
    std::string GetString(const std::string& key, const std::string& defaultValue = "") const;

    // Check if key exists - does the knowledge exist?
    bool HasKey(const std::string& key) const;

    // Remove key - erase from memory
    void RemoveKey(const std::string& key);

    // Clear all config - start fresh
    void Clear();

private:
    // Parse config line - decode the ancient runes
    bool ParseLine(const std::string& line);

    // Serialize value to string - turn knowledge into text
    std::string ValueToString(const ConfigValue& value) const;

    // Deserialize string to value - turn text into knowledge
    ConfigValue StringToValue(const std::string& str) const;

    // Our config data - the sacred map
    std::unordered_map<std::string, ConfigValue> configData;

    // Current config file - where we store our wisdom
    std::string currentFile;
};

#endif // CONFIGMANAGER_H
