#include "ColumnsConfig.h"
#include <fstream>
#include <iostream>

ColumnsConfig& ColumnsConfig::getInstance() {
    static ColumnsConfig instance;
    return instance;
}

bool ColumnsConfig::setConfigFile(const std::string& filepath) {
    return loadConfig(filepath);
}

bool ColumnsConfig::loadConfig(const std::string& filepath) {
    try {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            std::cerr << "Warning: Could not open config file: " << filepath << std::endl;
            return false;
        }
        config = nlohmann::json::parse(file);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Warning: Error loading config file: " << e.what() << std::endl;
        return false;
    }
}