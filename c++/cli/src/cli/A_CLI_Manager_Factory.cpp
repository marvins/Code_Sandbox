/**
 * @file    A_CLI_Manager_Factory.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Manager_Factory.hpp"


// CLI Libraries
#include "A_CLI_Configuration_File_Parser.hpp"
#include "A_CLI_Manager_Configuration.hpp"

// C++ Standard Libraries
#include <memory>

// Boost Libraries
#include <boost/filesystem.hpp>

namespace CLI{


/**********************************************/
/*        Initialize the CLI Manager          */
/**********************************************/
A_CLI_Manager::ptr_t A_CLI_Manager_Factory::Initialize( const std::string& config_pathname )
{
    // Check the path
    if(  boost::filesystem::exists( config_pathname ) == false ){
        return nullptr;
    }

    
    //  Parse the configuration file
    A_CLI_Configuration_File_Parser   parser( config_pathname );

    // Check parser status
    if( parser.Is_Valid() != true ){
        return nullptr;
    }
    
    // Fetch the configuration
    A_CLI_Manager_Configuration manager_config = parser.Get_CLI_Manager_Configuration();

    
    // Create the manager
    return std::make_shared<A_CLI_Manager>( manager_config );

}



} // End of CLI Namespace

