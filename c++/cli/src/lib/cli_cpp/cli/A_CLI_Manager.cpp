/**
 * @file    A_CLI_Manager.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Manager.hpp"


// CLI Libraries
#include "A_CLI_Connection_Handler_Base.hpp"
#include "A_Console_Render_Manager_ASCII.hpp"
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
    m_ncurses_context(NULL),
    m_handler_thread_running(false)
{


    // Grab the socket console render manager
    if( m_configuration.Get_CLI_Connection_Type() == CLIConnectionType::SOCKET ){
        m_console_render_manager = std::make_shared<A_Console_Render_Manager_ASCII>(m_configuration.Get_Socket_Window_Rows(),
                                                                                    m_configuration.Get_Socket_Window_Cols());
    }

    // Grab the ncurses console render manager
    else if( m_configuration.Get_CLI_Connection_Type() == CLIConnectionType::LOCAL ){
        m_console_render_manager = std::make_shared<A_Console_Render_Manager_NCurses>();
    }


    // Set the title
    m_console_render_manager->Set_CLI_Title( m_configuration.Get_CLI_Title() );

    // Set the command lists
    m_console_render_manager->Update_Parser_Command_List( m_configuration.Get_CLI_Command_Parser()->Get_Parser_Command_List());
    m_console_render_manager->Update_Command_List( m_configuration.Get_CLI_Command_Parser()->Get_Command_List());

    
    // Create CLI Communication Type
    m_ncurses_context = m_configuration.Create_NCurses_Context();

    // Set the Queue
    m_cli_command_queue = std::make_shared<A_CLI_Command_Queue>( m_configuration.Get_CLI_Command_Queue_Max_Size() );
    
    // Grab the connection handler
    m_connection_handler = m_configuration.Get_Connection_Handler();
    m_connection_handler->Update_Render_Manager( m_console_render_manager );
    m_connection_handler->Update_CLI_Command_Parser( m_configuration.Get_CLI_Command_Parser());
    m_connection_handler->Update_CLI_Command_Queue( m_cli_command_queue );

}


/**********************************************/
/*          Connect the CLI Manager           */
/**********************************************/
void A_CLI_Manager::Connect()
{
    
    // Configure the ncurses context
    m_console_render_manager->Update_NCurses_Context( m_ncurses_context );
    
    // Kick off the handler thread
    m_handler_thread = std::thread( &A_CLI_Manager::Process_Command_Results, 
                                    this );


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

    // Stop the handler thread
    if( m_handler_thread_running == true ){
        m_handler_thread_running = false;
        m_cli_command_queue->Clear();
        m_handler_thread.join();
    }

}


/************************************************/
/*      Wait for CLI Connection Shutdown        */
/************************************************/
void A_CLI_Manager::Wait_Shutdown()
{
    // Wait for the connection handler to stop
    m_connection_handler->Wait_Shutdown();
}


/************************************/
/*      Process Command Results     */
/************************************/
void A_CLI_Manager::Process_Command_Results()
{
    
    // Misc Variables
    CMD::A_CLI_Command_Result::ptr_t command_result;

    // Set flag
    m_handler_thread_running = true;

    while( m_handler_thread_running != false ){

        // Pop the next command
        command_result = m_cli_command_queue->Pop_Command();

        // Check if null
        if( command_result == nullptr ){
            continue;
        }

        // Process Command
        for( size_t i=0; i<m_cli_command_handlers.size(); i++ ){
            if( m_cli_command_handlers[i]->Is_Supported( *command_result ) == true )
            {
                m_cli_command_handlers[i]->Process_Command( command_result );
            }
        }
    }

    // Set flag
    m_handler_thread_running = false;
}


/*******************************************************/
/*          Register Command Response Handler          */
/*******************************************************/
void A_CLI_Manager::Register_Command_Response_Handler( A_Command_Response_Handler_Base::ptr_t handler )
{

    // Add to the list
    m_cli_command_handlers.push_back( handler );


}


} // End of CLI Namespace

