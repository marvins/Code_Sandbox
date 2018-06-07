/**
 * @file    fault-tolerance-demo-01.cpp
 * @author  Marvin Smith
 * @date    6/6/2018
 */

// C++ Libraries
#include <csignal>
#include <iostream>
#include <string>
#include <thread>

// MPI Libraries
#include <mpi.h>

// Common
#include "../common/Print_Utilities.hpp"

// Project Libraries
#include "Connection_Listener.hpp"


bool okay_to_run = true;


void Signal_Handler( int sig )
{
    std::cout << "Received Signal: " << sig << std::endl;
    return;
}


int main( int argc, char* argv[] )
{
    std::signal( SIGTERM, Signal_Handler );

    // Initialize MPI
    MPI_Init( &argc, &argv );

    // Get the rank and size
    int rank, nprocs;
    MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );


    // Get the proc info
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_len;
    MPI_Get_processor_name( hostname, &hostname_len);

    // Print some useful info
    Print_System_Info( rank, nprocs, 
                       std::string(hostname));
    
    // Sync before starting main app
    MPI_Barrier(MPI_COMM_WORLD);

    // Create Connection-Listener
    std::cout << "Constructing Listener" << std::endl;
    auto listener = std::make_shared<Connection_Listener>();

    std::cout << "Starting Listener" << std::endl;
    listener->Start();

    while( okay_to_run ){
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }

    listener->Stop();

    // Close up
    MPI_Finalize();

    return 0;

}

