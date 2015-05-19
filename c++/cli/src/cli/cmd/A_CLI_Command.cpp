/**
 * @file    A_CLI_Command.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_CLI_Command.hpp"

namespace CLI{
namespace CMD{

/**********************************/
/*          Constructor           */
/**********************************/
A_CLI_Command::A_CLI_Command( const std::string& command_name )
  : m_command_name(command_name)
{
}


} // End of CMD
} // End of CLI

