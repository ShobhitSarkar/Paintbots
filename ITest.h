#ifndef ITEST_H
#define ITEST_H

#include <cstring>  

#include <stdexcept>  
#include <algorithm> 

namespace Paintbots {

    /**
     * @class ITest 
     * Interface for test classes throughout the entire project 
     * All the test classes are going to be inherited from this interface 
     * and implemented using the doTests() method
     */
    class ITest {
        public: 
            /**
             * Virtual destructor for clean ups in derived classes
             */
            virtual ~ITest() = default; 

            /**
             * Runs all the tests in the derived classes
             * @return true if all tests pass, false otherwise 
             */
            virtual bool doTests() = 0; 
    }; 
}

#endif