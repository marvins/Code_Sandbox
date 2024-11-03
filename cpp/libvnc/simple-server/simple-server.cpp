/**
 * @file    simple-server.cpp
 * @author  Marvin Smith
 * @date    2/15/2018
 */


// C++ Dependencies
#include <iostream>
#include <memory>
#include <string>


// Project Dependencies
#include "Options.hpp"
#include "Server_Context.hpp"


int main( int argc, char* argv[] )
{
    // Parse the system configuration
    std::cout << "Loading Configuration Information" << std::endl;
    Options options(argc, argv);

    // Create the Server Context
    auto context = std::make_shared<Server_Context>(options);

    // Initialize
    context->Initialize();

    // Run the Server
    context->Run_Server();


    std::cout << "Exiting Application" << std::endl;
    return 0;
}

