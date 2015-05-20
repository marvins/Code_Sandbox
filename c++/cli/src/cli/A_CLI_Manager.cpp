/**
 * @file    A_CLI_Manager.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Manager.hpp"


// CLI Libraries
#include "A_CLI_Connection_Handler_Base.hpp"
#include "A_Console_Render_Manager_NCurses.hpp"

// C++ Standard Libraries
#include <iostream>


namespace CLI{


/********************************/
/*          Constructor         */
/********************************/
A_CLI_Manager::A_CLI_Manager( A_CLI_Manager_Configuration const& configuration )
  : m_class_name("A_CLI_Manager"),
    m_configuration(configuration),
    m_ncurses_context(NULL)
{
    // Grab the console render manager
    if( m_configuration.Get_CLI_Connection_Type() == CLIConnectionType::SOCKET ){
        m_console_render_manager = std::make_shared<A_Console_Render_Manager>();
    }
    else if( m_configuration.Get_CLI_Connection_Type() == CLIConnectionType::LOCAL ){
        m_console_render_manager = std::make_shared<A_Console_Render_Manager_NCurses>();
    }

    
    // Create CLI Communication Type
    m_ncurses_context = m_configuration.Create_NCurses_Context();

    
    // Grab the connection handler
    m_connection_handler = m_configuration.Get_Connection_Handler();
    m_connection_handler->Update_Render_Manager( m_console_render_manager );
    m_connection_handler->Update_CLI_Command_Parser( m_configuration.Get_CLI_Command_Parser());

}


/**********************************************/
/*          Connect the CLI Manager           */
/**********************************************/
void A_CLI_Manager::Connect()
{
    
    // Configure the ncurses context
    m_console_render_manager->Update_NCurses_Context( m_ncurses_context );

    // Kick off the console thread

    
    // Kick off the communication thread
    m_connection_handler->Start_Handler();

}



/**********************************************/
/*        Disconnect the CLI Manager          */
/**********************************************/
void A_CLI_Manager::Disconnect()
{
    
    // Stop the thread
    m_connection_handler->Signal_Shutdown();

    // Join the thread
    m_connection_handler->Wait_Shutdown();

}


/************************************************/
/*      Wait for CLI Connection Shutdown        */
/************************************************/
void A_CLI_Manager::Wait_Shutdown()
{
    m_connection_handler->Wait_Shutdown();
}


} // End of CLI Namespace

