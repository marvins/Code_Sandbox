/**
 * @file    Connection_Listener.cpp
 * @author  Marvin Smith
 * @date    6/6/2018
 */
#include "Connection_Listener.hpp"


// C++ Libraries
#include <iostream>

// Boost Libraries
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/asio/write.hpp>
#include <boost/asio/buffer.hpp>


using namespace boost::asio;
using namespace boost::asio::ip;


/********************************/
/*          Constructor         */
/********************************/
Connection_Listener::Connection_Listener()
  : m_next_id(0)
{
    //m_io_service = std::make_shared<boost::asio::io_service>();
    m_io_context = std::make_shared<boost::asio::io_context>();
    m_endpoint = tcp::endpoint(tcp::v4(), 12349);
    m_acceptor = std::make_shared<tcp::acceptor>(*m_io_context, m_endpoint);
    //m_socket   = std::make_shared<tcp::socket>(*m_io_service);
}


/****************************/
/*          Start           */
/****************************/
void Connection_Listener::Start()
{
    std::cout << "Starting Network Listener" << std::endl;
    m_okay_to_run = true;
    Add_Accept_Handler();
    
    std::cout << "Starting Context" << std::endl;
    m_io_context->run();
    std::cout << "Finished Starting" << std::endl;
}


/****************************/
/*          Stop            */
/****************************/
void Connection_Listener::Stop()
{
    std::cout << "Stopping Listener" << std::endl;
    m_okay_to_run = false;
    
    //m_socket->shutdown(tcp::socket::shutdown_send);
    
    if( m_worker.joinable()){
        m_worker.join();
    }
}


/************************/
/*          Run         */
/************************/
void Connection_Listener::Add_Accept_Handler()
{   std::cout << "Adding accept handler" << std::endl;
    m_acceptor->async_accept( [this](boost::system::error_code ec, tcp::socket socket){
        if (!ec){
            std::cout << "Accepted!" << std::endl;
            
            // Send the connection information
            Send_Connection_Data( socket );
        } else {
            std::cerr << "Problem accepting. " << ec.message() << std::endl;
        }
        
        // RE-add
        if( m_okay_to_run ){
            Add_Accept_Handler();
        }
    });
}

/********************************/
/*          Handle Resolve      */
/********************************/
void Connection_Listener::Accept_Handler( const boost::system::error_code& ec )
{
    if( !ec ){
        std::cout << "ACCEPTED!" << std::endl;
    } else {
        std::cerr << "Failed to accept: " << ec.message() << std::endl;
    }
}

void Connection_Listener::Send_Connection_Data( tcp::socket& socket )
{
    char port_data[MPI_MAX_PORT_NAME];
    
    // Get the port data
    MPI_Open_port( MPI_INFO_NULL, port_data );
    std::cout << "Port Data: " << port_data << std::endl;
    
    auto new_session = std::make_shared<Session>();
    new_session->port_info = port_data;
    new_session->status = 0;
    new_session->id = m_next_id++;
    
    // Add to session list
    m_sessions[new_session->id] = new_session;
    
    // Transmit
    async_write( socket, buffer(new_session->port_info),
                 boost::bind( &Connection_Listener::Port_Write_Handler,
                              this, placeholders::error,
                              placeholders::bytes_transferred,
                              new_session->id));
    
}


void Connection_Listener::Port_Write_Handler(const boost::system::error_code &ec, std::size_t bytes_transferred, int id)
{
    std::cout << "Sent " << bytes_transferred << " bytes, ID: " << id << std::endl;
    
    // Get the session
    auto session = m_sessions[id];
    
    // Try to connect
    std::cout << "Trying to accept" << std::endl;
    MPI_Comm_accept(session->port_info.c_str(), MPI_INFO_NULL,
                    0, MPI_COMM_SELF, &session->comm);
    
    
    // close
    std::cout << "Trying to close" << std::endl;
    MPI_Close_port(session->port_info.c_str());
}