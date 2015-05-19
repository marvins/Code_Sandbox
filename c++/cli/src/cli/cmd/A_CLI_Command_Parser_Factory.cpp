/**
 * @file    A_CLI_Command_Parser_Factory.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_CLI_Command_Parser_Factory.hpp"

// C++ Standard Libraries
#include <iostream>
#include <sstream>

// PUGI Xml
#include "../../thirdparty/pugixml.hpp"

namespace CLI{
namespace CMD{


/*********************************************/
/*          Create a Command Parser          */
/*********************************************/
A_CLI_Command_Parser::ptr_t  A_CLI_Command_Parser_Factory::Initialize( const std::string& config_path )
{
    // Create XML Document
    pugi::xml_document xmldoc;
    pugi::xml_parse_result result = xmldoc.load_file( config_path.c_str() );

    if( result == false ){
        std::stringstream sin;
        sin << "error: " << __FILE__ << ", Line: " << __LINE__ << ". CLI Command Configuration File parsed with errors. Details: " << result.description();
        std::cerr << sin.str() << std::endl;
        return nullptr;
    }

    
    // Get the root node
    pugi::xml_node root_node = xmldoc.child("command-configuration");

    // Check the node
    if( root_node == pugi::xml_node() ){ return nullptr; }


    // Get the Commands Node
    pugi::xml_node commands_node = root_node.child("commands");

    // Create the parser
    A_CLI_Command_Parser::ptr_t parser = std::make_shared<A_CLI_Command_Parser>();

    // Return new parser
    return parser;
}

} // End of CMD Namespace
} // End of CLI Namespace

