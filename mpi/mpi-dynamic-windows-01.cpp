
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
    for( int i=0; i<nprocs; i++ ){
        MPI_Barrier(MPI_COMM_WORLD);
        if( i == rank ){
            std::cout << "Initialized Rank: " << rank << ", Number Processors: " << nprocs << ", Hostname: " << hostname << std::endl;
        }
    }

    // MPI Variables
    MPI_Aint sdisp_remote;
    MPI_Aint sdisp_local;

    // Create MPI Window
    MPI_Win window;
    MPI_Win_create_dynamic( MPI_INFO_NULL, MPI_COMM_WORLD, &window );
    
    // If we are rank 0, then create a message
    if( rank == 0 ){

        // Create Message
        char message[] = "Hello World";
        int message_size = 12;

        // Attach the window
        MPI_Win_attach( window, &message_size, 1);
        MPI_Get_address( &message_size, &sdisp_local);
        for( size_t i=1; i<nprocs; i++ ){
            MPI_Send(&sdisp_local, 1, MPI_AINT, i, 1, MPI_COMM_WORLD);
        }

        // Wait for other ranks to catach up
        MPI_Barrier( MPI_COMM_WORLD );
        
        // Make sure we dont detach until everyone has pulled.
        MPI_Barrier( MPI_COMM_WORLD );
        
        // Detach the window
        MPI_Win_detach( window, &message_size );
        
        // Attach the message data
        MPI_Win_attach( window, message, message_size );
        MPI_Get_address( message, &sdisp_local );
        for( size_t i=1; i<nprocs; i++ ){
            MPI_Send(&sdisp_local, 1, MPI_AINT, i, 1, MPI_COMM_WORLD);
        }


        // Make sure the master rank does not detach
        MPI_Barrier( MPI_COMM_WORLD );

    }
    // Otherwise, we will need the message
    else{

        // Wait for the master rank to attach the data
        MPI_Barrier( MPI_COMM_WORLD );

        // Get the data
        int message_size;

        // Create a window
        MPI_Status reqstat;
        MPI_Recv(&sdisp_remote, 1, MPI_AINT, 0, 1, MPI_COMM_WORLD, &reqstat );
        
        // Lock to prevent writing
        MPI_Win_lock( MPI_LOCK_SHARED, 0, 0, window );
        
        // Call MPI Get
        MPI_Get( &message_size, 1, MPI_INT, 0, sdisp_remote, 1, MPI_INT, window );
        
        // Unlock the window
        MPI_Win_unlock( 0, window );
        

        // Make sure the master rank does not detach
        MPI_Barrier( MPI_COMM_WORLD );


        // Get the message data
        char* message_data = new char[message_size];
        
        // Get the status
        MPI_Recv( &sdisp_remote, 1, MPI_AINT, 0, 1, MPI_COMM_WORLD, &reqstat );

        // Lock to prevent writing
        MPI_Win_lock( MPI_LOCK_SHARED, 0, 0, window );
        
        // Call MPI Get
        MPI_Get( message_data, message_size, MPI_CHAR, 0, sdisp_remote, message_size, MPI_CHAR, window );
        
        // Unlock the window
        MPI_Win_unlock( 0, window );

        // Print Message
        std::cout << "Rank: " << rank << ", Received Message from Master: " << message_data << std::endl;
        
        // Make sure the master rank does not detach
        MPI_Barrier( MPI_COMM_WORLD );

        // delete the message data
        delete [] message_data;

    }

    // Wait at the barrier
    MPI_Barrier( MPI_COMM_WORLD );
    
    // Destroy the MPI Window
    MPI_Win_free( &window );

    // Close MPI
    MPI_Finalize();
    
    return 0;
}
