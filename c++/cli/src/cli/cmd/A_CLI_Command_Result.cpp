/**
 * @file    A_CLI_Command_Result.cpp
 * @author  Marvin Smith
 * @date    5/19/2015
*/
#include "A_CLI_Command_Result.hpp"

namespace CLI{
namespace CMD{


/*************************************/
/*            Constructor            */
/*************************************/
A_CLI_Command_Result::A_CLI_Command_Result( CLICommandParseStatus const&  parse_status,
                                            A_CLI_Command const&          command)
  : m_parse_status(parse_status),
    m_command(command)
{
}


/*************************************/
/*            Constructor            */
/*************************************/
A_CLI_Command_Result::A_CLI_Command_Result( CLICommandParseStatus const&     parse_status,
                                            A_CLI_Command const&             command,
                                            std::vector<std::string> const&  argument_values )
  : m_parse_status(parse_status),
    m_command(command),
    m_argument_values(argument_values)
{
}



/**********************************************/
/*          Process Command Arguments         */
/**********************************************/
A_CLI_Command_Result  A_CLI_Command_Result::Process_Arguments( const A_CLI_Command& command,    
                                                               const std::vector<std::string>&  arguments )
{

    //  Iterate over arguments
    return A_CLI_Command_Result( CLICommandParseStatus::INVALID_ARGUMENTS,
                                 command,
                                 arguments );


}


} // End of CMD Namespace
} // End of CLI Namespace

