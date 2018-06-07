/**
 * @file    Print_Utilities.cpp
 * @author  Marvin Smith
 * @date    6/6/2018
 */
#include "Print_Utilities.hpp"

// MPI Libraries
#include <mpi.h>


// C++ Libraries
#include <iostream>


/********************************************/
/*          Print System Information        */
/********************************************/
void Print_System_Info( int rank, 
                        int nprocs, 
                        std::string hostname )
{
    // Print Message
    for( int i=0; i<nprocs; i++ ){
        MPI_Barrier(MPI_COMM_WORLD);
        if( rank == i ){
            std::cout << "Initialized Rank: " << rank << ", Number Processors: " << nprocs << ", Hostname: " << hostname << std::endl;
        }
    }
}


