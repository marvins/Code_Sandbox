/**
 * @file    A_CLI_Connection_Handler_Local.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Connection_Handler_Local.hpp"

// C++ Standard Libraries
#include <iostream>
#include <unistd.h>

// CLI Libraries
#include "A_Console_Render_Manager_NCurses.hpp"

namespace CLI{


/*************************/
/*      Constructor      */
/*************************/
A_CLI_Connection_Handler_Local::A_CLI_Connection_Handler_Local( A_CLI_Connection_Handler_Base_Config::ptr_t configuration )
  : A_CLI_Connection_Handler_Base(),
    m_class_name("A_CLI_Connection_Handler_Local")
{
    
    // Cast the configuration
    m_configuration = std::dynamic_pointer_cast<A_CLI_Connection_Handler_Local_Config>(configuration);
}


/*****************************/
/*       Destructor          */
/*****************************/
A_CLI_Connection_Handler_Local::~A_CLI_Connection_Handler_Local()
{

}


/***********************************************/
/*          Run the message handler            */
/***********************************************/
void A_CLI_Connection_Handler_Local::Run_Handler()
{
    // Keyboard input value
    int key;

    if( this->m_console_render_manager != nullptr ){
        this->m_console_render_manager->Initialize();
        this->m_render_state = this->m_console_render_manager->Get_Render_State();
    }

    if( this->m_render_state == nullptr ){
        std::cerr << "error: Unable to process the render state." << std::endl;
        m_is_running = false;
        return;
    }

    // Loop until time to quit
    while( true ){

        // Check the manager
        if( this->m_console_render_manager == nullptr ){
            break;
        }


        // Render the screen
        this->m_console_render_manager->Refresh();
        
        // Check keyboard value
        key = this->m_console_render_manager->Wait_Keyboard_Input();
    
        // Exit if we have an appropriate exit command
        if( key == 27 || key == KEY_ENTER || key == 10 )
        {
            // Process the command
            this->Process_Command();
            
        }

        // Otherwise, add the character to the cli render state
        else{
            this->m_render_state->Process_Input( key );
        }

        // Check if time to exit
        if( m_is_running == false ){
            break;
        }

    }

    // Set the running flag
    m_is_running = false;
    
}



} // End of CLI Namespacee

