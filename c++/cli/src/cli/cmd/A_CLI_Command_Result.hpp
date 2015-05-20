/**
 * @file    A_CLI_Command_Result.hpp
 * @author  Marvin Smith
 * @date    5/19/2015
 */
#ifndef __CLI_CMD_A_CLI_COMMAND_RESULT_HPP__
#define __CLI_CMD_A_CLI_COMMAND_RESULT_HPP__

// CLI Libraries
#include "A_CLI_Command.hpp"
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


        /**
         * @brief Constructor
         */
        A_CLI_Command_Result( CLICommandParseStatus const&    parse_status,
                              A_CLI_Command const&            command );
       

        /**
         * @brief Constructor
         */
        A_CLI_Command_Result( CLICommandParseStatus const&    parse_status,
                              A_CLI_Command const&            command,
                              std::vector<std::string> const& argument_values );

    
        /**
         * @brief Get the Parsing Status.
         *
         * @return Parse status.
         */
        inline CLICommandParseStatus Get_Parse_Status()const{
            return m_parse_status;
        }

    
        /**
         * @brief Process Command Results
         */
        static A_CLI_Command_Result  Process_Arguments( const A_CLI_Command&             command,
                                                        const std::vector<std::string>&  arguments );


    private:
    
        /// Status
        CLICommandParseStatus m_parse_status;

        /// Command Found
        A_CLI_Command m_command;

        /// Argument Values
        std::vector<std::string> m_argument_values;


}; // End of A_CLI_Command_Result Class

} // End of CMD Namespace
} // End of CLI Namespace

#endif
