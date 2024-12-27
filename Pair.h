#ifndef PAIR_H
#define PAIR_H

#include <stdexcept>
#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    /**
     * The class is designed to hold and provide access to two pointers of the same type 
     */
    template<typename T>
    class Pair {
        private:
            T* first; // pointer to the first object in the pair 
            T* second; // pointer to the second object in the pair 

        public:

            /**
             * Constructs pair object with 2 pointers 
             */
            Pair(T* firstPtr, T* secondPtr) : first(firstPtr), second(secondPtr) {}

            /**
             * Retrieves first pointer from the list 
             */
            T* getFirst() const {
                return first;
            }

            /**
             * Retrieves the second pointer from the list 
             */
            T* getSecond() const {
                return second;
            }

            /**
             * Privdes indexed access to the pointers in the pair 
             * 
             * Allows access to pointers via indices (1 for the first pointer and 
             * 2 for the second pointer)
             */
            T* operator[](int index) {
                switch(index) {
                    case 1: return first;
                    case 2: return second;
                    default: 
                        throw std::out_of_range("Pair index must be 1 or 2");
                }
            }

            /**
             * Provides constant indexed access to the pointers in the pair (1 for 
             * the first pointer and 2 for the second pointer)
             */
            const T* operator[](int index) const {
                switch(index) {
                    case 1: return first;
                    case 2: return second;
                    default: 
                        throw std::out_of_range("Pair index must be 1 or 2");
                }
            }
    };
}

#endif
