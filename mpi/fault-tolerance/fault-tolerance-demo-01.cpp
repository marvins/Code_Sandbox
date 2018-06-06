/**
 * @file    fault-tolerance-demo-01.cpp
 * @author  Marvin Smith
 * @date    6/6/2018
 */

// C++ Libraries
#include <iostream>
#include <string>

// MPI Libraries
#include <mpi.h>


void Print_Info( int rank, int nprocs, std::string hostname )
{
    // Print Message
    for( int i=0; i<nprocs; i++ ){
        MPI_Barrier(MPI_COMM_WORLD);
        if( rank == i ){
            std::cout << "Initialized Rank: " << rank << ", Number Processors: " << nprocs << ", Hostname: " << hostname << std::endl;
        }
    }
    

}


int main( int argc, char* argv[] )
{
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

    Print_Info( rank, nprocs, std::string(hostname));
    
    MPI_Barrier(MPI_COMM_WORLD);

    // Close up
    MPI_Finalize();

    return 0;

}

