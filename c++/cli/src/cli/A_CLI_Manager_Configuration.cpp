/**
 * @file    A_CLI_Manager_Configuration.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Manager_Configuration.hpp"

// CLI Libraries
#include "A_CLI_Connection_Handler_Local.hpp"
#include "A_CLI_Connection_Handler_Socket.hpp"

namespace CLI{


/***************************/
/*      Constructor        */
/***************************/
A_CLI_Manager_Configuration::A_CLI_Manager_Configuration( CLIConnectionType const& cli_conn_type )
  : m_class_name("A_CLI_Manager_Configuration"),
    m_cli_conn_type(cli_conn_type)
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
        context->tty_in  = stdin;
        context->tty_out = stdout;
    }
    else{
        return nullptr;
    }


    // return the new context
    return context;
}



} // End of CLI Namespace
