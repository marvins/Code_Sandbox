/**
 * @file    A_CLI_Command_Parser.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Command_Parser.hpp"

// Boost Libraries
#include <boost/algorithm/string.hpp>

namespace CLI{
namespace CMD{

/*****************************/
/*       Constructor         */
/*****************************/
A_CLI_Command_Parser::A_CLI_Command_Parser( const std::string& regex_split_pattern,
                                            const std::vector<A_CLI_Parser_Command>& parser_command_list,
                                            const std::vector<A_CLI_Command>& command_list )
  : m_class_name("A_CLI_Command_Parser"),
    m_command_list(command_list),
    m_parser_command_list(parser_command_list),
    m_regex_split_pattern(regex_split_pattern)
{
}


/***************************************/
/*          Evaluate Command           */
/***************************************/
A_CLI_Command_Result  A_CLI_Command_Parser::Evaluate_Command( const std::string& test_str )const
{
    // Split the string
    std::vector<std::string> components = Parse_String( test_str );

    // Get the first element
    std::string command_name = components[0];
    
    // Remove the first element
    if( components.size() > 1 ){
        components.erase(components.begin());
    } else{
        components.clear();
    }
    

    // Iterate over parser commands
    for( size_t idx=0; idx < m_parser_command_list.size(); idx++ ){
        if( m_parser_command_list[idx].Is_Match( command_name ) == true )
        {
            return A_CLI_Command_Result( m_parser_command_list[idx].Get_Mode(),
                                         A_CLI_Command( "", "", false),
                                         components );
        }

    }

    
    // Iterate over regular commands
    for( size_t idx=0; idx < m_command_list.size(); idx++ ){
    
        // Check if there is a name match
        if( m_command_list[idx].Is_Name_Match( command_name ) == true ){
            return A_CLI_Command_Result::Process_Arguments( m_command_list[idx], components );
        }
    }


    // return the result
    return A_CLI_Command_Result( CLICommandParseStatus::NO_COMMAND_FOUND,
                                 A_CLI_Command( command_name, "", false),
                                 components);

}


/******************************/
/*        Parse String        */
/******************************/
std::vector<std::string>  A_CLI_Command_Parser::Parse_String( const std::string& test_str )const
{
    // Create output list
    std::vector<std::string> output;

    // Split
    boost::split( output, test_str, boost::is_any_of( m_regex_split_pattern ));

    // return results
    return output;

}


} // End of CMD Namespace
} // End of CLI Namespace

