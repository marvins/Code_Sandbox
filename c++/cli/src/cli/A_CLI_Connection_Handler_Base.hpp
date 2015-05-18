/**
 * @file    A_CLI_Connection_Handler_Base.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CLI_CONNECTION_HANDLER_BASE_HPP__
#define __CLI_A_CLI_CONNECTION_HANDLER_BASE_HPP__

// C++ Standard Libraries
#include <memory>
#include <string>

namespace CLI{

/**
 * @class A_CLI_Connection_Handler_Base
*/
class A_CLI_Connection_Handler_Base{

    public:
        
        /// Pointer type
        typedef std::shared_ptr<A_CLI_Connection_Handler_Base> ptr_t;


        /**
         * @brief Constructor
         */
        A_CLI_Connection_Handler_Base();


        /**
         * @brief Destructor
        */
        virtual ~A_CLI_Connection_Handler_Base();

        
        /**
         * @brief Start the handler
         */
        virtual void Start_Handler();


        /**
         * @brief Stop the handler
         */
        virtual void Signal_Shutdown();


    protected:
        
        /**
         * @brief Run the handler
         */
        virtual void Run_Handler() = 0;


        /// Running Flag
        bool m_is_running;

    private:
        
        /// Class Name
        std::string m_class_name;


}; // End of A_CLI_Connection_Handler_Base Class

} // End of CLI Namespace

#endif
