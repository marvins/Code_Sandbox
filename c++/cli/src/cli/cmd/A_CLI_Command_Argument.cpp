/**
 * @file    A_CLI_Command_Argument.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_CLI_Command_Argument.hpp"

// Boost Libraries
#include <boost/regex.hpp>

namespace CLI{
namespace CMD{


/**********************************/
/*          Constructor           */
/**********************************/
A_CLI_Command_Argument::A_CLI_Command_Argument()
  : m_name(""),
    m_type(CLICommandArgumentType::UNKNOWN),
    m_description("")
{
}


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


/*****************************************/
/*          Check String Type            */
/*****************************************/
bool A_CLI_Command_Argument::Is_Valid_Type( const std::string& test_str )const
{
    // String Match
    boost::smatch what;

    // Compare Integer Types
    if( m_type == CLICommandArgumentType::INTEGER ){
        return boost::regex_match( test_str, what, boost::regex("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?")); 
    }

    // Compare Float Types
    if( m_type == CLICommandArgumentType::FLOAT ){
        return boost::regex_match( test_str, what, boost::regex("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?")); 
    }

    // Compare String Types
    if( m_type == CLICommandArgumentType::STRING ){
        return true;
    }

    // Otherwise, there is a problem
    return false;
}

} // End of CMD Namespace
} // End of CLI Namespace

