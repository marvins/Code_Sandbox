/**
 * @file    A_CLI_Command_Parser.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Command_Parser.hpp"

namespace CLI{
namespace CMD{

/*****************************/
/*       Constructor         */
/*****************************/
A_CLI_Command_Parser::A_CLI_Command_Parser( const std::vector<A_CLI_Command>& command_list )
  : m_class_name("A_CLI_Command_Parser"),
    m_command_list(command_list)
{
}


/***************************************/
/*          Evaluate Command           */
/***************************************/
A_CLI_Command_Result  A_CLI_Command_Parser::Evaulate_Command( const std::string& test_str )
{
    // Create output result
    std::string command_name;

    // Split the string
    


    // return the result
    A_CLI_Command_Result result;

    return result;
}


} // End of CMD Namespace
} // End of CLI Namespace

