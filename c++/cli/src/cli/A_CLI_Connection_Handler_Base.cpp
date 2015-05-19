/**
 * @file    A_CLI_Connection_Handler_Base.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_CLI_Connection_Handler_Base.hpp"

namespace CLI{


/************************/
/*      Constructor     */
/************************/
A_CLI_Connection_Handler_Base::A_CLI_Connection_Handler_Base()
  : m_is_running(false),
    m_console_render_manager(nullptr),
    m_cli_command_parser(nullptr),
    m_class_name("A_CLI_Connection_Handler_Base")
{
}



/*************************/
/*       Destructor      */
/*************************/
A_CLI_Connection_Handler_Base::~A_CLI_Connection_Handler_Base()
{


}


/***************************************/
/*          Start the Handler          */
/***************************************/
void A_CLI_Connection_Handler_Base::Start_Handler()
{

    // Get the current context
    if( m_render_state == nullptr ){
        return;
    }
    
    // Signal that we are running
    m_is_running = true;

    // Start the thread
    m_connection_thread = std::thread( &A_CLI_Connection_Handler_Base::Run_Handler,
                                       this);
}


/****************************************/
/*          Wait for shutdown           */
/****************************************/
void A_CLI_Connection_Handler_Base::Wait_Shutdown()
{
    // Only join if we are running
    if( m_is_running == true ){
        m_connection_thread.join();
    }
}


/*************************************/
/*          Stop the Handler         */
/*************************************/
void A_CLI_Connection_Handler_Base::Signal_Shutdown()
{
    // set the flag
    m_is_running = false;
    
    // Finalize
    m_console_render_manager->Finalize();
}



} // End of CLI Namespace

