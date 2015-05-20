/**
 * @file    A_CLI_Manager_Configuration.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Manager_Configuration.hpp"

// CLI Libraries
#include "A_CLI_Connection_Handler_Local.hpp"
#include "A_CLI_Connection_Handler_Socket.hpp"


// C++ Standard Libraries
#include <iostream>
#include <unistd.h>


namespace CLI{


/***************************/
/*      Constructor        */
/***************************/
A_CLI_Manager_Configuration::A_CLI_Manager_Configuration( CLIConnectionType const& cli_conn_type )
  : m_class_name("A_CLI_Manager_Configuration"),
    m_cli_conn_type(cli_conn_type),
    m_command_parser(nullptr),
    m_socket_window_rows(20),
    m_socket_window_cols(80)
{
}

/***************************************************/
/*          Get the Connection Handler             */
/***************************************************/
A_CLI_Connection_Handler_Base::ptr_t  A_CLI_Manager_Configuration::Get_Connection_Handler()const{

    // Make sure the configuration is not null
    if( m_connection_handler_configuration == nullptr ){
        return nullptr;
    }

    // If we are Local, create the handelr
    if( m_cli_conn_type == CLIConnectionType::LOCAL )
    {
        return std::make_shared<A_CLI_Connection_Handler_Local>( m_connection_handler_configuration );
    }

    // If we are socket, create the handler
    else if( m_cli_conn_type == CLIConnectionType::SOCKET )
    {
        return std::make_shared<A_CLI_Connection_Handler_Socket>( m_connection_handler_configuration );
    }

    // otherwise, return null
    return nullptr;

}


/************************************/
/*       Create NCurses Context     */
/************************************/
NCURSES::An_NCurses_Context::ptr_t  A_CLI_Manager_Configuration::Create_NCurses_Context()const
{
    
    // Create new object
    NCURSES::An_NCurses_Context::ptr_t context = std::make_shared<NCURSES::An_NCurses_Context>();


    // Attach communication file pointers
    if( m_cli_conn_type == CLIConnectionType::LOCAL ){

        // find the terminal name
        std::string terminal_name = getenv("TERM");

        // Create the context elements
        context->tty_fd  = NULL;
        context->tty_in  = stdin;
        context->tty_out = stdout;
        context->tty_terminal_name = terminal_name;
    }

    // Attach communication file pointers
    else if( m_cli_conn_type == CLIConnectionType::SOCKET ){
        
        // find the terminal name
        std::string terminal_name = getenv("TERM");

        // Create the context elements
        context->tty_fd  = NULL;
        context->tty_in  = stdin;
        context->tty_out = stdout;
        context->tty_terminal_name = terminal_name;


    }

    // Otherwise, there is a problem
    else{
        return nullptr;
    }


    // return the new context
    return context;
}


/************************************************************/
/*          Check if the configuration is valid             */
/************************************************************/
bool A_CLI_Manager_Configuration::Is_Valid()const
{
    // Check if the configuration is null
    if( m_connection_handler_configuration == nullptr ){
        return false;
    }


    // If no failures, yet, then success
    return true;
}


} // End of CLI Namespace
