
// MPI 
#include <mpi.h>

// C++ Standard Libraries
#include <iostream>
#include <vector>

using namespace std;

// Global Definitions
const int WINDOW_SIZE_INTS=1000000;
const int NUMBER_COMMUNICATORS=5;
const int NUMBER_ITERATIONS=10;
const int NUMBER_WINDOWS=10;


// Source Dataset
// 
// source_dataset[iteration-number][communicator][window]
std::vector<std::vector<std::vector<int*> > >  source_dataset;


// MPI Communicators
std::vector<MPI_Comm> communicators;

// MPI Groups
std::vector<MPI_Group> groups;


/**
 * Initialize the dataset
*/
void Initialize_Source_Datasets();


/**
 * Main Function
 */
int main( int argc, char* argv[] ) {


    // Initialize MPI
    MPI_Init( &argc, &argv );


    // Get the rank and size
    int comm_world_rank;
    int comm_world_size;
    MPI_Comm_rank( MPI_COMM_WORLD, &comm_world_rank );
    MPI_Comm_size( MPI_COMM_WORLD, &comm_world_size );


    // Get the processor name
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int processor_name_len;
    MPI_Get_processor_name( processor_name, &processor_name_len);


    // Log the entry point
    for( int i=0; i<comm_world_size; i++ ){
        MPI_Barrier(MPI_COMM_WORLD);
        if( i == comm_world_rank ){
            std::cout << "Initialized Rank: " << comm_world_rank << ", Number Processors: " << comm_world_size << ", Processor Name: " << processor_name << std::endl;
        }
    }

    // Initialize the source datasets
    Initialize_Source_Datasets();

    /*

    // MPI Address Displacements
    MPI_Aint sdisp_remote;
    MPI_Aint sdisp_local;


    // Create the MPI Window
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
    */

    // Wait at the barrier
    MPI_Barrier( MPI_COMM_WORLD );


    // Destroy the MPI Window
    //MPI_Win_free( &window );    


    // Close MPI
    MPI_Finalize();

    return 0;
}


/**
 * Initialize Source Datasets
*/
void Initialize_Source_Datasets(){
    
    // Setup the dataset
    source_dataset.resize(NUMBER_ITERATIONS);

    //  For each iteration
    for( int iteration=0; iteration<NUMBER_ITERATIONS; iteration++ ){

        // For each communicator
        source_dataset[iteration].resize(NUMBER_COMMUNICATORS);
        for( int comm_id=0; comm_id<NUMBER_COMMUNICATORS; comm_id++ ){
        
            // For each window
            source_dataset[iteration][comm_id].resize(NUMBER_WINDOWS);
            for( int window_id=0; window_id<NUMBER_WINDOWS; window_id++ ){
                
                // Allocate each window
                source_dataset[iteration][comm_id][window_id] = new int[WINDOW_SIZE_INTS];

                // assign the data
                for( int data_idx=0; data_idx<WINDOW_SIZE_INTS; data_idx++ ){
                    source_dataset[iteration][comm_id][window_id][data_idx] = data_idx;
                }
            }
        }
    }

}


