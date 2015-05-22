/**
 * @file    main.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */

// C++ Standard Libraries
#include <iostream>
#include <signal.h>
#include <string>
#include <unistd.h>

// Demo Libraries
#include "cli/A_Ping_Command_Response_Handler.hpp"

// CLI Libraries
#include <cli_cpp/cli/A_CLI_Manager_Factory.hpp>
#include <cli_cpp/utils/System_Utilities.hpp>

using namespace std;

/**
 * @brief Main Program
 */
int main( int argc, char* argv[] )
{
    try{
    
    // Check args
    if( argc < 2 ){
        std::cerr << "usage: " << argv[0] << " <config-path>" << std::endl;
        return 1;
    }

    // Define the configuration file path
    std::string config_pathname = argv[1];

    // Create the CLI Manager
    CLI::A_CLI_Manager::ptr_t manager = CLI::A_CLI_Manager_Factory::Initialize( config_pathname );

    // Make sure it is not null
    if( manager == nullptr ){
        std::cerr << "Error: Returned null." << std::endl;
        return 1;
    }

    // Register a Ping Command-Response Handler
    A_Ping_Command_Response_Handler::ptr_t  ping_handler = std::make_shared<A_Ping_Command_Response_Handler>();
    manager->Register_Command_Response_Handler( ping_handler );

    // Initialize the CLI Manager
    manager->Connect();

    // Check the type of run and wait if necessary
    //if( manager->Get_CLI_Connection_Type() == CLI::CLIConnectionType::LOCAL ){
        manager->Wait_Shutdown();
    //}

    // Disconnect the CLI Manager
    manager->Disconnect();

    } catch ( exception& e ){
        NCURSES::Abort();  
        std::cerr << "exception caught What: " << e.what() << std::endl;
    } catch (...){
        NCURSES::Abort();  
        std::cerr << "unknown exception detected." << std::endl;
    }


    // Exit Program
    return 0;
}

