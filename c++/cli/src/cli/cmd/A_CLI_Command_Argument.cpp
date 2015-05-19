/**
 * @file    A_CLI_Command_Argument.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_CLI_Command_Argument.hpp"

namespace CLI{
namespace CMD{


/**********************************/
/*          Constructor           */
/**********************************/
A_CLI_Command_Argument::A_CLI_Command_Argument( const std::string&             arg_name,
                                                const CLICommandArgumentType&  arg_type,
                                                const std::string&             arg_description )
   : m_name(arg_name),
     m_type(arg_type),
     m_description(arg_description)
{
}


} // End of CMD Namespace
} // End of CLI Namespace

