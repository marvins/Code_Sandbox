/**
 * @file    fault-tolerance-demo-01.cpp
 * @author  Marvin Smith
 * @date    6/6/2018
 */

// C++ Libraries
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

// MPI Libraries
#include <mpi.h>

// Common
#include "../common/Print_Utilities.hpp"

// Boost Libraries
#include <boost/asio/buffer.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>

std::array<char, 4096> bytes;
boost::asio::io_service io_service;
boost::asio::ip::tcp::socket tcp_socket{io_service};

void read_handler(const boost::system::error_code &ec,
                  std::size_t bytes_transferred )
{
    if (!ec)
    {
        
        tcp_socket.async_read_some( boost::asio::buffer(bytes),
                                    read_handler );
    }
    else
    {
        std::string bytestr(bytes.data());
        std::cout << "Completed: " << bytestr << std::endl;
    }
}

void connect_handler(const boost::system::error_code &ec )
{
    if (!ec)
    {
        // Wait for some data
        //std::string r =
        //    "GET / HTTP/1.1\r\nHost: theboostcpplibraries.com\r\n\r\n";
        //write(tcp_socket, buffer(r));
        tcp_socket.async_read_some( boost::asio::buffer(bytes),
                                    read_handler);
    }
}

void resolve_handler(const boost::system::error_code &ec,
                     boost::asio::ip::tcp::resolver::iterator it )
{
    if (!ec)
        tcp_socket.async_connect(*it, connect_handler );
}


int main( int argc, char* argv[] )
{
    // Wait some time
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Create Connection to Master
    boost::asio::ip::tcp::resolver::query q{"localhost", "12349"};
    boost::asio::ip::tcp::resolver resolver{io_service};
    
    resolver.async_resolve(q, resolve_handler );
    
    
    io_service.run();
    
    
    // Get master connection info


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

    // Have all but one rank do something useless
    while( true )
    {


    }

    // Close up
    MPI_Finalize();

    return 0;

}

