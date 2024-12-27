#ifndef CONFIG_ERRORS_H
#define CONFIG_ERRORS_H

#include <stdexcept>
#include <string>
#include <cstring> 
#include <stdexcept>  
#include <algorithm>  

namespace Paintbots {

/**
 * @brief Base class for all configuration errors
 */
class ConfigError : public std::runtime_error {
public:
    explicit ConfigError(const std::string& message) : std::runtime_error(message) {}
};

/**
 * @brief Error thrown when config file cannot be found or opened
 */
class ConfigFileError : public ConfigError {
public:
    explicit ConfigFileError(const std::string& filename) 
        : ConfigError("Unable to open configuration file: " + filename) {}
};

/**
 * @brief Error thrown when config file has permission issues
 */
class ConfigPermissionError : public ConfigError {
public:
    explicit ConfigPermissionError(const std::string& filename)
        : ConfigError("Permission denied for configuration file: " + filename) {}
};

/**
 * @brief Error thrown when config file format is invalid
 */
class ConfigFormatError : public ConfigError {
public:
    explicit ConfigFormatError(const std::string& details)
        : ConfigError("Invalid configuration format: " + details) {}
};

/**
 * @brief Error thrown when config value is invalid
 */
class ConfigValueError : public ConfigError {
public:
    explicit ConfigValueError(const std::string& details)
        : ConfigError("Invalid configuration value: " + details) {}
};

/**
 * @brief Error thrown when config bounds are invalid
 */
class ConfigBoundsError : public ConfigError {
public:
    explicit ConfigBoundsError(const std::string& details)
        : ConfigError("Invalid bounds in configuration: " + details) {}
};

} 

#endif