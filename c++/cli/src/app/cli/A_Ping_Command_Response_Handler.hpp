/**
 * @file    A_Ping_Command_Response_Handler.hpp
 * @author  Marvin Smith
 * @date    5/20/2015
 */
#ifndef __CLI_A_PING_COMMAND_RESPONSE_HANDLER_HPP__
#define __CLI_A_PING_COMMAND_RESPONSE_HANDLER_HPP__

// CLI Libraries
#include <cli_cpp/cli/A_Command_Response_Handler_Base.hpp>


/**
 * @class A_Ping_Command_Response_Handler
 */
class A_Ping_Command_Response_Handler : public CLI::A_Command_Response_Handler_Base 
{

    public:
        
        /// Pointer Type
        typedef std::shared_ptr<A_Ping_Command_Response_Handler> ptr_t;


        /**
         * @brief Constructor
         */
        A_Ping_Command_Response_Handler();


        /**
         * @brief Check if valid.
         */
        virtual bool Is_Supported( const CLI::CMD::A_CLI_Command_Result& command )const;


        /**
         * @brief Process the Command
         */
        virtual void Process_Command( CLI::CMD::A_CLI_Command_Result::ptr_t command );


}; // End of A_Ping_Command_Response_Handler Class


#endif
