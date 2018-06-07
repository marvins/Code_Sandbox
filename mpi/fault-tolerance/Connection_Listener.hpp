/**
 * @file    Connection_Listener.hpp
 * @author  Marvin Smith
 * @date    6/6/2018
 */
#ifndef FAULT_DEMO_CONNECTION_LISTENER_HPP
#define FAULT_DEMO_CONNECTION_LISTENER_HPP

// C++ Libraries
#include <map>
#include <memory>
#include <string>
#include <thread>

// Boost Libraries
#include <boost/asio/io_context.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>

// MPI Libraries
#include <mpi.h>


struct Session
{
    public:
        typedef std::shared_ptr<Session> ptr_t;
        
        int id;
        std::string port_info;
        int status;
        MPI_Comm comm;
};

class Connection_Listener
{
    public:

        /**
         * @brief Constructor
         */
        Connection_Listener();
        
        
        void Start();
        
        
        void Stop();

    private:
        
        
        /**
         * @brief Run the listener
         */
        void Add_Accept_Handler();
        
        
        /**
         * @brief Handler for acceptions.
         * @param ec
         * @param it
         */
        void Accept_Handler(const boost::system::error_code&  ec);
        
        
        void Send_Connection_Data( boost::asio::ip::tcp::socket& socket);
        
        
        void Port_Write_Handler( const boost::system::error_code &ec,
                                 std::size_t bytes_transferred, int id );
        
        /// Asio Context
        boost::asio::ip::tcp::endpoint m_endpoint;
        std::shared_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;
        //std::shared_ptr<boost::asio::io_service> m_io_service;
        std::shared_ptr<boost::asio::io_context> m_io_context;
        //std::shared_ptr<boost::asio::ip::tcp::socket> m_socket;
        
        // Back ground worker
        std::thread m_worker;
        bool m_okay_to_run;
        
        // Connections
        std::map<int,Session::ptr_t> m_sessions;
        
        int m_next_id;

}; // End of Connection-Listener Class

#endif
