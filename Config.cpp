#include "Config.h"
#include "ConfigError.h"
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    Config* Config::instance = nullptr; // initialize static instance 

    /**
     * Default constructor with present values 
     */
    Config::Config() : 
        hitDuration(20),
        paintBlobLimit(30),
        rockLowerBound(10),
        rockUpperBound(20),
        fogLowerBound(5),
        fogUpperBound(10),
        longRangeLimit(30) {}

    /**
     * Gets default singleton instance 
     */
    Config& Config::getInstance() {
        resetInstance(); 
        if (instance == nullptr) {
            instance = new Config();
        }
        return *instance;
    }

    /**
     * Gets singleton instance with config file 
     * @param configFile: configuration file path 
     * @throws std:runtime_error on file/parsing error
     */
    Config& Config::getInstance(const std::string& configFile) {
        resetInstance();
        instance = new Config(configFile);
        return *instance;
    }

    /**
     * Resets singleton instance safely 
     */
    void Config::resetInstance() {
        delete instance;
        instance = nullptr;
    }

    /**
     * File-based constructor. Parses file to extract key-value pairs 
     * @param configFile: path to config file 
     * @throws std::runtime_error: for invalid files/values 
     */
    Config::Config(const std::string& configFile) : Config() {
        std::ifstream file(configFile);
        
        // Check if file exists and handle permissions
        if (!file.is_open()) {
            if (errno == ENOENT) {
                throw ConfigFileError(configFile);
            } else if (errno == EACCES) {
                throw ConfigPermissionError(configFile);
            } else {
                throw ConfigError("Error opening file: " + std::string(strerror(errno)));
            }
        }

        std::string line;
        int lineNumber = 0;
        while (std::getline(file, line)) {
            lineNumber++;
            
            
            if (line.empty() || line[0] == '#' || std::all_of(line.begin(), line.end(), isspace)) {
                continue;
            }

            size_t equalPos = line.find('=');
            if (equalPos == std::string::npos) {
                throw ConfigFormatError("Missing '=' on line " + std::to_string(lineNumber) + ": " + line);
            }
            if (line.find('=', equalPos + 1) != std::string::npos) {
                throw ConfigFormatError("Multiple '=' found on line " + std::to_string(lineNumber));
            }

            std::string key = line.substr(0, equalPos);
            std::string value = line.substr(equalPos + 1);

            // Remove whitespace
            key.erase(remove_if(key.begin(), key.end(), isspace), key.end());
            value.erase(remove_if(value.begin(), value.end(), isspace), value.end());

            // Validate key format
            if (key.empty()) {
                throw ConfigFormatError("Empty key on line " + std::to_string(lineNumber));
            }
            if (key.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789") != std::string::npos) {
                throw ConfigFormatError("Invalid characters in key on line " + std::to_string(lineNumber) + ": " + key);
            }

            // Validate value
            if (value.empty()) {
                throw ConfigFormatError("Empty value for key '" + key + "' on line " + std::to_string(lineNumber));
            }

            std::transform(key.begin(), key.end(), key.begin(), ::toupper);

            try {
                int intValue = std::stoi(value);
                if (intValue <= 0) {
                    throw ConfigValueError("Negative or zero value for key '" + key + "': " + value);
                }
                
                if (key == "HIT_DURATION") hitDuration = intValue;
                else if (key == "PAINTBLOB_LIMIT") paintBlobLimit = intValue;
                else if (key == "ROCK_LOWER_BOUND") rockLowerBound = intValue;
                else if (key == "ROCK_UPPER_BOUND") rockUpperBound = intValue;
                else if (key == "FOG_LOWER_BOUND") fogLowerBound = intValue;
                else if (key == "FOG_UPPER_BOUND") fogUpperBound = intValue;
                else if (key == "LONG_RANGE_LIMIT") longRangeLimit = intValue;
                else throw ConfigFormatError("Unknown configuration key: " + key);
                
            } catch (const std::invalid_argument&) {
                throw ConfigValueError("Non-numeric value for key '" + key + "': " + value);
            } catch (const std::out_of_range&) {
                throw ConfigValueError("Value out of range for key '" + key + "': " + value);
            }
        }

        // Validate bounds
        if (rockLowerBound > rockUpperBound) {
            throw ConfigBoundsError("Rock lower bound (" + std::to_string(rockLowerBound) + 
                                ") cannot be greater than upper bound (" + std::to_string(rockUpperBound) + ")");
        }
        if (fogLowerBound > fogUpperBound) {
            throw ConfigBoundsError("Fog lower bound (" + std::to_string(fogLowerBound) + 
                                ") cannot be greater than upper bound (" + std::to_string(fogUpperBound) + ")");
        }
    }   

    /**
     *  * Returns hit duration config value
    */
    int Config::getHitDuration() const {return hitDuration;}

    /**
     * returns paintblob limit config value
     */
    int Config::getPaintBlobLimit() const {return paintBlobLimit;}

    /**
     * return lower bound config value for rocks 
     */
    int Config::getRockLowerBound() const {return rockLowerBound;}

    /**
     * return upper bound config value for rocks
     */
    int Config::getRockUpperBound() const{return rockUpperBound;}

    /**
     * return lower bound config value for fog
     */
    int Config::getFogLowerBound() const {return fogLowerBound;}

    /**
     * return upper bound config value for fog
     */
    int Config::getFogUpperBound() const{return fogUpperBound;}

    /**
     * return the long range scan limit 
     */
    int Config::getLongRangeLimit() const {return longRangeLimit;}
}
