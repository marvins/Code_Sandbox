/**
 * @file    A_CLI_Configuration_File_Parser.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Configuration_File_Parser.hpp"

// CLI Libraries
#include "../thirdparty/pugixml.hpp"

// C++ Standard Libraries
#include <iostream>
#include <sstream>


namespace CLI{


/************************************/
/*          Constructor             */
/************************************/
A_CLI_Configuration_File_Parser::A_CLI_Configuration_File_Parser( const std::string& config_path )
  : m_class_name("A_CLI_Configuration_File_Parser"),
    m_config_pathname(config_path),
    m_current_configuration(A_CLI_Manager_Configuration(CLIConnectionType::LOCAL)),
    m_is_valid(false)
{
    // Parse the file
    Parse_Configuration_File();
}


/********************************************/
/*      Parse the Configuration File        */
/********************************************/
void A_CLI_Configuration_File_Parser::Parse_Configuration_File()
{

    /// List of queries
    const std::string CONNECTION_TYPE_QUERY = "connection-type";


    // Create XML Document
    pugi::xml_document xmldoc;
    pugi::xml_parse_result result = xmldoc.load_file( m_config_pathname.c_str() );

    if( result == false ){
        std::stringstream sin;
        sin << "error: " << __FILE__ << ", Line: " << __LINE__ << ". CLI Configuration File parsed with errors. Details: " << result.description();
        std::cerr << sin.str() << std::endl;
        return;
    }

    
    // Get the root node
    pugi::xml_node root_node = xmldoc.child("cli-manager-configuration");

    // Check the node
    if( root_node == pugi::xml_node() ){ return; }

    

    // Check the connection type
    CLIConnectionType cli_conn_type = StringToCLIConnectionType( root_node.child(CONNECTION_TYPE_QUERY.c_str()).attribute("value").as_string());
    if( cli_conn_type == CLIConnectionType::UNKNOWN ){
        std::stringstream sin;
        sin << "error: " << __FILE__ << ", Line: " << __LINE__ << ". CLI Connection Type value is invalid.";
        std::cerr << sin.str() << std::endl;
        return;
    }
    m_current_configuration.Set_CLI_Connection_Type( cli_conn_type );

    // Set valid
    m_is_valid = true;
}



} // End of CLI Namespace

