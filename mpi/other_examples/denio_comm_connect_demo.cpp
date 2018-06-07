#include "mpi.h"

#include <chrono>
#include <cstdio>
#include <iostream>
#include <string>
#include <thread>


/* This test checks to make sure that two MPI_Comm_connects to two different MPI ports
 * * match their corresponding MPI_Comm_accepts. The root process opens two MPI ports and
 * * sends the first port to process 1 and the second to process 2. Then the root process
 * * accepts a connection from the second port followed by the first port. 
 * * Processes 1 and 2 both connect back to the root but process 2 first sleeps for three 
 * * seconds to give process 1 time to attempt to connect to the root. The root should wait 
 * * until process 2 connects before accepting the connection from process 1.
 * */
int main( int argc, char *argv[] )
{
    int num_errors = 0;
    int rank, size;
    char port1[MPI_MAX_PORT_NAME];
    char port2[MPI_MAX_PORT_NAME];
    MPI_Status status;
    MPI_Comm comm1, comm2;
    int data = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (size < 3)
    {
        printf("Three processes needed to run this test.\n");fflush(stdout);
        MPI_Finalize();
        return 0;
    }

    // Setup ranks
    if (rank == 0)
    {
        printf("0: opening ports.\n");fflush(stdout);
        MPI_Open_port(MPI_INFO_NULL, port1);
        MPI_Open_port(MPI_INFO_NULL, port2);
        printf("opened port1: <%s>\n", port1);
        printf("opened port2: <%s>\n", port2);fflush(stdout);

        MPI_Send(port1, MPI_MAX_PORT_NAME, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
        MPI_Send(port2, MPI_MAX_PORT_NAME, MPI_CHAR, 2, 0, MPI_COMM_WORLD);

        printf("accepting port2.\n");fflush(stdout);
        MPI_Comm_accept(port2, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm2);
        printf("accepting port1.\n");fflush(stdout);
        MPI_Comm_accept(port1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm1);
        MPI_Close_port(port1);
        MPI_Close_port(port2);
        
        printf("sending 1 to process 1.\n");fflush(stdout);
        data = 1;
        MPI_Send(&data, 1, MPI_INT, 0, 0, comm1);
        printf("sending 2 to process 2.\n");fflush(stdout);
        data = 2;
        MPI_Send(&data, 1, MPI_INT, 0, 0, comm2);

        MPI_Comm_disconnect(&comm1);
        MPI_Comm_disconnect(&comm2);
    }
    else if (rank == 1)
    {
        MPI_Recv(port1, MPI_MAX_PORT_NAME, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        std::cout << "Rank 1: Connecting to port: " << port1 << std::endl;
        MPI_Comm_connect(port1, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm1);
        std::cout << "Rank 1: Connected" << std::endl;
        MPI_Recv(&data, 1, MPI_INT, 0, 0, comm1, &status);
        if (data != 1)
        {
            printf("Received %d from root when expecting 1\n", data);
            fflush(stdout);
            num_errors++;
        }
        MPI_Comm_disconnect(&comm1);
    }
    else if (rank == 2)
    {
        MPI_Recv(port2, MPI_MAX_PORT_NAME, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
        std::cout << "Rank 2: Connecting to port: " << port2 << std::endl;
        /* make sure process 1 has time to do the connect before this process attempts to connect */
        std::this_thread::sleep_for(std::chrono::seconds(1));
        MPI_Comm_connect(port2, MPI_INFO_NULL, 0, MPI_COMM_SELF, &comm2);
        std::cout << "Rank 2: Connected" << std::endl;
        MPI_Recv(&data, 1, MPI_INT, 0, 0, comm2, &status);
        if (data != 2)
        {
            printf("Received %d from root when expecting 2\n", data);
            fflush(stdout);
            num_errors++;
        }
    MPI_Comm_disconnect(&comm2);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
    return 0;
}
