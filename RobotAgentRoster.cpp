#include "RobotAgentRoster.h"
#include <iostream>
#include <cstring>  
#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {
    RobotAgentRoster* RobotAgentRoster::instance = nullptr;

    /**
     * @brief Get the singleton instance of RobotAgentRoster
     */
    RobotAgentRoster& RobotAgentRoster::getInstance() {
        if (instance == nullptr) {
            std::cout << "Creating new RobotAgentRoster instance" << std::endl;
            instance = new RobotAgentRoster();
        }
        return *instance;
    }

    /**
     * @brief Add a robot agent to the roster
     * @param agent Pointer to the IRobotAgent to be added
     */

    void RobotAgentRoster::add(IRobotAgent* agent) {
        if (agent == nullptr) {
            throw std::invalid_argument("Cannot add null robot agent");
        }
        std::cout << "Adding robot: " << agent->getRobotName() << std::endl;
        agents.push_back(agent);
        std::cout << "New roster size: " << agents.size() << std::endl;
    }

    /**
     * @brief Get a robot agent by index
     * @param index Index of the robot agent
     * @return Pointer to the IRobotAgent at the specified index
     * @throws std::out_of_range if index is invalid
     */
    IRobotAgent* RobotAgentRoster::operator[](int index) {
        if (index < 0 || index >= static_cast<int>(agents.size())) {
            throw std::out_of_range("Robot agent index out of range");
        }
        return agents[index];
    }

    /**
     * @brief Get a robot agent by name
     * @param name Name of the robot agent
     * @return Pointer to the IRobotAgent with the specified name
     * @throws std::runtime_error if no agent with the given name is found
     */
    IRobotAgent* RobotAgentRoster::operator[](const std::string& name) {
        for (IRobotAgent* agent : agents) {
            if (agent->getRobotName() == name) {
                return agent;
            }
        }
        throw std::runtime_error("No robot agent found with name: " + name);
    }

    /**
     * @brief Get the number of agents in the roster
     * @return Size of the agents vector
     */
    size_t RobotAgentRoster::size() const {
        return agents.size();
    }

    /**
     * @brief Destructor to clean up dynamically allocated agents
     */
    RobotAgentRoster::~RobotAgentRoster() {
        for (IRobotAgent* agent : agents) {
            delete agent;
        }
        agents.clear();
    }

    /**
     * @brief Reset the singleton instance
     */
    void RobotAgentRoster::resetInstance() {
        delete instance;
        instance = nullptr;
    }
}