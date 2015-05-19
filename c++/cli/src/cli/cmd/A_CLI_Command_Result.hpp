/**
 * @file    A_CLI_Command_Result.hpp
 * @author  Marvin Smith
 * @date    5/19/2015
 */
#ifndef __CLI_CMD_A_CLI_COMMAND_RESULT_HPP__
#define __CLI_CMD_A_CLI_COMMAND_RESULT_HPP__

// CLI Libraries
#include "CLICommandParseStatus.hpp"

// C++ Standard Libraries
#include <string>


namespace CLI{
namespace CMD{

/**
 * @class A_CLI_Command_Result
 */
class A_CLI_Command_Result{

    public:
        
        /**
         * @brief Constructor
        */
        A_CLI_Command_Result();

    
    private:
    
        /// Status
        CLICommandParseStatus m_parse_status;

}; // End of A_CLI_Command_Result Class

} // End of CMD Namespace
} // End of CLI Namespace

#endif
