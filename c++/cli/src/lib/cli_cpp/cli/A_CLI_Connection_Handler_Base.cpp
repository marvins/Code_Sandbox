/**
 * @file    A_CLI_Connection_Handler_Base.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_CLI_Connection_Handler_Base.hpp"

// C++ Standard Libraries
#include <iostream>


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


/**************************************/
/*        Process the Command         */
/**************************************/
void A_CLI_Connection_Handler_Base::Process_Command()
{
    // Check the command
    CMD::A_CLI_Command_Result result = m_cli_command_parser->Evaluate_Command( this->m_render_state->Get_Cursor_Text() );

    // Add to history
    this->m_console_render_manager->Add_Command_History( this->m_render_state->Get_Cursor_Text(), 
                                                         result );

    //  Look for CLI Shutdown
    if( result.Get_Parse_Status() == CMD::CLICommandParseStatus::CLI_SHUTDOWN ){
        m_is_running = false;
        std::cout << "shutdown requested" << std::endl;
    }

    
    // Look for CLI Help
    else if( result.Get_Parse_Status() == CMD::CLICommandParseStatus::CLI_HELP ){
        this->m_render_state->Set_Help_Mode( true );
        std::cout << "help requested" << std::endl;
    }


    // Otherwise, handle command
    else if( result.Get_Parse_Status() == CMD::CLICommandParseStatus::VALID ){
        m_cli_command_queue->Push_Command( result );
        std::cout << "valid command" << std::endl;
    }

    // Otherwise, error
    else{
        std::cout << "invalid command" << std::endl;
    }

    
    // Clear the cursor    
    this->m_render_state->Clear_Cursor_Text();

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

