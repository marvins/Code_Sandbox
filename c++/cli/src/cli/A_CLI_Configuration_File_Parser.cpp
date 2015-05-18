/**
 * @file    A_CLI_Configuration_File_Parser.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Configuration_File_Parser.hpp"


namespace CLI{


/************************************/
/*          Constructor             */
/************************************/
A_CLI_Configuration_File_Parser::A_CLI_Configuration_File_Parser( const std::string& config_path )
  : m_class_name("A_CLI_Configuration_File_Parser"),
    m_config_pathname(config_path)
{
}


/*****************************************************/
/*          Get the CLI Manager Configuration        */
/*****************************************************/
A_CLI_Manager_Configuration  A_CLI_Configuration_File_Parser::Get_CLI_Manager_Configuration()const{
    
    // Create the configuration
    A_CLI_Manager_Configuration config;

    // Return the configuration object.
    return config;
}



} // End of CLI Namespace

