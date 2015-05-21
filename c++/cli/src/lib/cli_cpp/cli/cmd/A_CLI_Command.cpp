/**
 * @file    A_CLI_Command.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_CLI_Command.hpp"

// C++ Standard Libraries
#include <sstream>

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
                              const std::string& command_description,
                              const bool&        command_expect_response )
  : m_command_name(command_name),
    m_command_description(command_description),
    m_expect_response(command_expect_response)
{
}


/**********************************/
/*          Constructor           */
/**********************************/
A_CLI_Command::A_CLI_Command( const std::string& command_name,
                              const std::string& command_description,
                              const bool&        command_expect_response,
                              const std::vector<A_CLI_Command_Argument>&  command_arguments )
  : m_command_name(command_name),
    m_command_description(command_description),
    m_expect_response(command_expect_response),
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


/************************************/
/*         Check Arguments          */
/************************************/
bool A_CLI_Command::Check_Argument( const int& argument_idx,
                                    const std::string& test_argument )const
{
    // Check the size
    if( argument_idx >= (int)m_command_argument_list.size() ){
        return false;
    }

    // Compare
    return m_command_argument_list[argument_idx].Is_Valid_Type( test_argument );
}


/************************************/
/*      Write as a debug string     */
/************************************/
std::string A_CLI_Command::To_Debug_String()const
{
    // Create stream
    std::stringstream sin;
    sin << "A_CLI_Command:\n";
    sin << "   Name: " << Get_Name() << "\n";
    sin << "   Desc: " << Get_Description() << "\n";
    
    return sin.str();
}


} // End of CMD
} // End of CLI

