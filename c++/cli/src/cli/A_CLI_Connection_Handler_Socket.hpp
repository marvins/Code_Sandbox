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
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

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

        /**
         * @brief Setup the socket.
         */
        void Setup_Socket();


        /**
         * @brief Close the socket.
         */
        void Close_Socket();

        
        /// Class Name
        std::string m_class_name;


        /// Configuration
        A_CLI_Connection_Handler_Socket_Config::ptr_t m_configuration;


        /// Socket File Descriptor
        int m_sock_fd;


        /// Client File Description
        int m_client_fd;

}; // End of A_CLI_Connection_Handler_Socket Class

} // End of CLI Namespace

#endif
