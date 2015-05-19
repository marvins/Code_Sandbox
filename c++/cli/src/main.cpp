/**
 * @file    main.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */

// C++ Standard Libraries
#include <iostream>
#include <string>
#include <unistd.h>


// CLI Libraries
#include "cli/A_CLI_Manager_Factory.hpp"

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


    // Initialize the CLI Manager
    manager->Connect();

    // Check the type of run and wait if necessary
    if( manager->Get_CLI_Connection_Type() == CLI::CLIConnectionType::LOCAL ){
        manager->Wait_Shutdown();
    }

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

