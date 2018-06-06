// MPI 
#include <mpi.h>

// C++ Standard Libraries
#include <iostream>


using namespace std;

int main( int argc, char* argv[] ) {

    // Initialize MPI
    MPI_Init( &argc, &argv );

    // Get the rank and size
    int rank, nprocs;
    MPI_Comm_size( MPI_COMM_WORLD, &nprocs );
    MPI_Comm_rank( MPI_COMM_WORLD, &rank );

    // Get the processor name
    char hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_len;
    MPI_Get_processor_name( hostname, &hostname_len);

    // Print Message
    std::cout << "Initialized Rank: " << rank << ", Number Processors: " << nprocs << ", Hostname: " << hostname << std::endl;

    // Close MPI
    MPI_Finalize();
    
    return 0;
}
