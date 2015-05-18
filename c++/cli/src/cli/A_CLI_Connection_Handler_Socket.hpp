/**
 * @file    A_CLI_Connection_Handler_Socket.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#ifndef __CLI_A_CLI_CONNECTION_HANDLER_SOCKET_HPP__
#define __CLI_A_CLI_CONNECTION_HANDLER_SOCKET_HPP__

// CLI Libraries
#include "A_CLI_Connection_Handler_Base.hpp"
#include "A_CLI_Connection_Handler_Base_Config.hpp"
#include "A_CLI_Connection_Handler_Socket_Config.hpp"

// C++ Standard Libraries
#include <memory>

namespace CLI{

/**
 * @class A_CLI_Connection_Handler_Socket
 */
class A_CLI_Connection_Handler_Socket : public A_CLI_Connection_Handler_Base
{

    public:
        
        /// Pointer Type
        typedef std::shared_ptr<A_CLI_Connection_Handler_Socket> ptr_t;

        
        /**
         * @brief Constructor
        */
        A_CLI_Connection_Handler_Socket( A_CLI_Connection_Handler_Base_Config::ptr_t configuration );


        /**
         * @brief Destructor
         */
        ~A_CLI_Connection_Handler_Socket();

    
    protected:
        
        /**
         * @brief Run the message handler
         */
        virtual void Run_Handler();


    private:
        
        /// Class Name
        std::string m_class_name;


        /// Configuration
        A_CLI_Connection_Handler_Socket_Config::ptr_t m_configuration;

}; // End of A_CLI_Connection_Handler_Socket Class

} // End of CLI Namespace

#endif
