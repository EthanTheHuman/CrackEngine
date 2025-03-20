#pragma once
#include <string>
#include "../../../includes/json.hpp"

class ColumnsConfig {
public:
    static ColumnsConfig& getInstance();
    
    // Set the config file path and load it
    bool setConfigFile(const std::string& filepath);
    
    // Getters for game constants with default values
    int getFrameDropRate() const { return getValue("game_constants", "frame_drop_rate", 30); }
    int getFrameDropRateDrop() const { return getValue("game_constants", "frame_drop_rate_drop", 5); }
    int getLockDelay() const { return getValue("game_constants", "lock_delay", 30); }
    int getAutoRepeatDelay() const { return getValue("game_constants", "auto_repeat_delay", 15); }
    int getAutoRepeatRate() const { return getValue("game_constants", "auto_repeat_rate", 4); }
    int getSpawnDelay() const { return getValue("game_constants", "spawn_delay", 15); }
    int getBlocksToChain() const { return getValue("game_constants", "blocks_to_chain", 3); }
    
    // Getters for chaining rules with default values
    bool getVerticalChaining() const { return getValue("chaining_rules", "vertical_chaining", true); }
    bool getHorizontalChaining() const { return getValue("chaining_rules", "horizontal_chaining", true); }
    bool getDiagonalChaining() const { return getValue("chaining_rules", "diagonal_chaining", true); }
    
    // Getters for board dimensions with default values
    int getBoardWidth() const { return getValue("board_dimensions", "width", 6); }
    int getBoardHeight() const { return getValue("board_dimensions", "height", 13); }

private:
    ColumnsConfig() {
        // Initialize with default values
        config = {
            {"game_constants", {
                {"frame_drop_rate", 30},
                {"frame_drop_rate_drop", 5},
                {"lock_delay", 30},
                {"auto_repeat_delay", 15},
                {"auto_repeat_rate", 4},
                {"spawn_delay", 15},
                {"blocks_to_chain", 3}
            }},
            {"chaining_rules", {
                {"vertical_chaining", true},
                {"horizontal_chaining", true},
                {"diagonal_chaining", true}
            }},
            {"board_dimensions", {
                {"width", 6},
                {"height", 13}
            }}
        };
    }
    ~ColumnsConfig() = default;
    ColumnsConfig(const ColumnsConfig&) = delete;
    ColumnsConfig& operator=(const ColumnsConfig&) = delete;

    // Helper function to safely get values with defaults
    template<typename T>
    T getValue(const std::string& section, const std::string& key, T defaultValue) const {
        try {
            if (config.contains(section) && config[section].contains(key)) {
                return config[section][key].get<T>();
            }
        } catch (const std::exception&) {
            // If any error occurs, return the default value
        }
        return defaultValue;
    }

    // Load configuration from file
    bool loadConfig(const std::string& filepath);

    nlohmann::json config;
}; 