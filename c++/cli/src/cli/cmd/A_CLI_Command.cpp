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
A_CLI_Command::A_CLI_Command()
{
}


/**********************************/
/*          Constructor           */
/**********************************/
A_CLI_Command::A_CLI_Command( const std::string& command_name,
                              const std::string& command_description )
  : m_command_name(command_name),
    m_command_description(command_description)
{
}


/**********************************/
/*          Constructor           */
/**********************************/
A_CLI_Command::A_CLI_Command( const std::string& command_name,
                              const std::string& command_description,
                              const std::vector<A_CLI_Command_Argument>&  command_arguments )
  : m_command_name(command_name),
    m_command_description(command_description),
    m_command_argument_list(command_arguments)
{
}


/**************************************/
/*        Check if Name Match         */
/**************************************/
bool A_CLI_Command::Is_Name_Match( const std::string& command_name )const
{
    if( m_command_name == command_name ){
        return true;
    }
    return false;
}



} // End of CMD
} // End of CLI

