/**
 * @file   A_Command_Response_Handler_Base.hpp
 * @author Marvin Smith
 * @date   5/19/2015
*/
#ifndef __CLI_A_COMMAND_RESPONSE_HANDLER_BASE_HPP__
#define __CLI_A_COMMAND_RESPONSE_HANDLER_BASE_HPP__

// C++ Standard Libraries
#include <memory>

// CLI Libraries
#include "cmd/A_CLI_Command_Response.hpp"

namespace CLI{

/**
 * @class A_Command_Response_Handler_Base
*/
class A_Command_Response_Handler_Base{

    public:
        
        /**
         * @brief Constructor
        */
        A_Command_Response_Handler_Base();


        /**
         * @brief Check if valid
         */
        virtual bool Is_Supported( const CMD::A_Response_Command& command )const = 0;


        /**
         * @brief Process COmmand
        */
        virtual void Process_Command( const CMD::A_Response_Command& command ) = 0;

}; // End of A_Command_Response_Handler_Base

} // End of CLI Namespace

#endif
