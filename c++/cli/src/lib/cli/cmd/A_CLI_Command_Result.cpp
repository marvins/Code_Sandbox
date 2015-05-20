/**
 * @file    A_CLI_Command_Result.cpp
 * @author  Marvin Smith
 * @date    5/19/2015
*/
#include "A_CLI_Command_Result.hpp"

// C++ Standard Libraries
#include <sstream>
#include <string>

namespace CLI{
namespace CMD{


/*************************************/
/*            Constructor            */
/*************************************/
A_CLI_Command_Result::A_CLI_Command_Result()
  : m_parse_status( CLICommandParseStatus::UNKNOWN )
{
}


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
    for( size_t arg=0; arg < arguments.size(); arg++ )
    {
        // Check the types
        if( command.Check_Argument( arg, arguments[arg] ) == false ){
            return A_CLI_Command_Result( CLICommandParseStatus::INVALID_ARGUMENTS,
                                         command,
                                         arguments );
        }
    }


    // Check the excess arguments
    if( arguments.size() > command.Get_Argument_List().size() ){
        return A_CLI_Command_Result( CLICommandParseStatus::EXCESSIVE_ARGUMENTS,
                                     command,
                                     arguments );
    }


    // Return success
    return A_CLI_Command_Result( CLICommandParseStatus::VALID,
                                 command,
                                 arguments );


}


/************************************/
/*      Print to Debug String       */
/************************************/
std::string A_CLI_Command_Result::To_Debug_String( const int& offset )const
{
    // create the gap
    std::string gap = "";
    for( int i=0; i<offset; i++ ){
        gap += ' ';
    }

    // Print the output
    std::stringstream sin;
    sin << gap << "A_CLI_Command_Result:\n";
    sin << gap << "    Command Name: " << m_command.Get_Name() << "\n";
    sin << gap << "    Command Desc: " << m_command.Get_Description() << "\n";
    sin << gap << "    Command Args: "; 
    for( size_t i=0; i<m_command.Get_Argument_List().size(); i++ ){
        sin << gap << "       Argument " << i << " : Name : " << m_command.Get_Argument_List()[i].Get_Name() << "\n";
        sin << gap << "                              Type : " << CMD::CLICommandArgumentTypeToString(m_command.Get_Argument_List()[i].Get_Type()) << "\n";
        sin << gap << "                              Desc : " << m_command.Get_Argument_List()[i].Get_Description() << "\n";
        sin << gap << "                              Value: ";
        if( m_argument_values.size() > i ){ sin << m_argument_values[i]; }
        sin << "\n";
    }

    return sin.str();
}


} // End of CMD Namespace
} // End of CLI Namespace

