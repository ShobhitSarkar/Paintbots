#ifndef ROBOT_AGENT_ROSTER_H
#define ROBOT_AGENT_ROSTER_H

#include "IRobotAgent.h"
#include <vector>
#include <string>
#include <stdexcept>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

/**
 * Singleton class for managing collection of robot agents 
 */
namespace Paintbots {
    class RobotAgentRoster {
        private:

            /**
             * Pointer to the single instance of the RobotAgentRoster 
             */
            static RobotAgentRoster* instance;

            /**
             * Vector containing pointers to all registered IRobotAgent instances 
             */
            std::vector<IRobotAgent*> agents;
            
            // Private constructor and deleted copy operations
            RobotAgentRoster() = default;
            RobotAgentRoster(const RobotAgentRoster&) = delete;
            RobotAgentRoster& operator=(const RobotAgentRoster&) = delete;

        public:

            /**
             * Retrieves singleton instance of RobotAgentRoster
             */
            static RobotAgentRoster& getInstance();

            /**
             * Adds new robot agent to the roster 
             */
            void add(IRobotAgent* agent);

            /**
             * Retrieves a robot agent by index in the roster
             */
            IRobotAgent* operator[](int index);

            /**
             * Retrieves a robot agent by name in the roster
             */
            IRobotAgent* operator[](const std::string& name);

            /**
             * The number of IRobotAgent instances in the roster
             */
            size_t size() const;

            /**
             * Destructor
             */
            ~RobotAgentRoster();

            /**
             * Resets singleton instance of the roster
             */
            static void resetInstance();
    };
}

#endif