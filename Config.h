#ifndef CONFIG_H 
#define CONFIG_H 

#include <string> 
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots{

    /**
     * @class Config 
     * Singleton class managing game configuration parameters 
     * 
     * Handles reading and storing game setting from config files. In case of absence of 
     * values, it provides default values 
     */
    class Config{

        private: 
            static Config* instance; // singleton instance

            int hitDuration; // duration of paint hit 
            int paintBlobLimit; // max paint blobs per robot
            int rockLowerBound; // min number of rocks
            int rockUpperBound; // max number of rocks
            int fogLowerBound; // min number of fog squares
            int fogUpperBound; // max number of fog squares
            int longRangeLimit; // maximum number of long range scans 

            // Private constructor for singleton 
            Config(); 
            Config(const std::string& configFile); 
            Config(const Config&) = delete; 
            Config& operator = (const Config&) = delete; 

        
        public: 

            /**
             * Gets singleton instance with default config 
             * @return Reference to Config instance
             */
            static Config& getInstance(); 

            /**
             * Gets singleton instance with specified config file
             * @param configFilPath: path to configuration file 
             * @return: reference to Config instance 
             * @throws std::runtime_error on file or parsign errors 
             */
            static Config& getInstance(const std::string& configFile); 

            /**
             * Resets singleton instance 
             */
            static void resetInstance(); 

            /**
             * Returns hit duration config value
             */
            int getHitDuration() const; 

            /**
             * returns paintblob limit config value
             */
            int getPaintBlobLimit() const; 

            /**
             * return lower bound config value for rocks 
             */
            int getRockLowerBound() const; 

            /**
             * return upper bound config value for rocks
             */
            int getRockUpperBound() const; 

            /**
             * return lower bound config value for fog
             */
            int getFogLowerBound() const; 

            /**
             * return upper bound config value for fog
             */
            int getFogUpperBound() const; 

            /**
             * return the long range scan limit 
             */
            int getLongRangeLimit() const; 

    }; 
}

#endif