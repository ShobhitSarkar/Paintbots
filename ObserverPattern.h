// ObserverPattern.h
#ifndef OBSERVER_PATTERN_H 
#define OBSERVER_PATTERN_H 

#include <vector> 
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 


namespace Paintbots {

    class Observable; 

    class Observer {
        public: 
            /**
             * Virtual destructor for subclasses 
             */
            virtual ~Observer() = default; 

            /**
             * Updates for observables 
             */
            virtual void update(Observable* subject) = 0; 
    }; 

    class Observable {
        private: 
            /**
             * Vector for keeping track of observers 
             */
            std::vector<Observer*> observers; 

        public: 
            /**
             * Virtual desctructor for observable 
             */
            virtual ~Observable() = default; 

            /**
             * method to add observer to list 
             */
            void addObserver(Observer* observer) {
                if (observer == nullptr){
                    throw std::invalid_argument("Cannot add null observer");
                }
                observers.push_back(observer); 
            }

            /**
             * Method to remove observer from observer list 
             */
            void removeObserver(Observer* observer) {
                auto it = std::find(observers.begin(), observers.end(), observer); 
                if (it != observers.end()) {
                    observers.erase(it); 
                }
            }

            /**
             * Method to notify observers about state changes 
             */
            void notifyObservers() {
                for (Observer* observer : observers) {
                    observer->update(this); 
                }
            }
            
            /**
             * Method to get observers 
             */
            const std::vector<Observer*>& getObservers() const {
                return observers;
            }
    }; 
}

#endif