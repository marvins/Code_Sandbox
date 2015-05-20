/**
 * @file    A_Console_Render_Manager.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_Console_Render_Manager.hpp"

// C++ Standard Libraries
#include <iostream>
#include <sstream>

// CLI Libraries
#include "../utils/ANSI_Utilities.hpp"

namespace CLI{


/****************************/
/*      Constructor         */
/****************************/
A_Console_Render_Manager::A_Console_Render_Manager()
  : m_class_name("A_Console_Render_Manager"),
    m_render_state(std::make_shared<A_Console_Render_State>(CLIConnectionType::SOCKET))
{
}


/********************************/
/*      Initialize Curses       */
/********************************/
void A_Console_Render_Manager::Initialize()
{

    // Create new render state
    m_render_state.reset(new A_Console_Render_State(CLIConnectionType::SOCKET));

    // Set the size
    m_render_state->Set_Window_Size( 20, 100 );

    // Build the console buffer
    Build_Console_Buffer();

}

/********************************/
/*        Finalize Curses       */
/********************************/
void A_Console_Render_Manager::Finalize()
{
}


/********************************/
/*      Refresh the screen      */
/********************************/
void A_Console_Render_Manager::Refresh()
{
    // CLear the buffer
    m_console_buffer = UTILS::ANSI_CLEARSCREEN + UTILS::ANSI_RESETCURSOR;

    // Draw the header
    Print_Header();

    
    // Draw the main context
    Print_Main_Content();

    
    // Draw the footer
    Print_Footer();

    
    // Draw the CLI
    Print_CLI();


}


/****************************************/
/*          Print the header            */
/****************************************/
void A_Console_Render_Manager::Print_Header()
{
    // Add the main header
    for( int i=0; i<m_render_state->Get_Cols(); i++ ){
        m_console_buffer+='-';
    }
    m_console_buffer += "\n\r";
    
    m_console_buffer += "      Console\n\r";
    
    
    for( int i=0; i<m_render_state->Get_Cols(); i++ ){
        m_console_buffer+='-';
    }
    m_console_buffer += "\n\r";


}


/************************************************/
/*          Print the Main Context              */
/************************************************/
void A_Console_Render_Manager::Print_Main_Content()
{

    // Put blank data here
    for( int i=0; i<m_render_state->Get_Rows()-6; i++ ){
        m_console_buffer += "\n\r";
    }

}


/****************************************/
/*          Print the Footer            */
/****************************************/
void A_Console_Render_Manager::Print_Footer()
{


}


/********************************/
/*          Print the CLI       */
/********************************/
void A_Console_Render_Manager::Print_CLI()
{
    // Move the cursor
    std::string output = "   ";
    output += UTILS::ANSI_GREEN + std::string("cmd: ") + UTILS::ANSI_RESET;
    std::cout << "Cursor: " << m_render_state->Get_Cursor_Text() << std::endl;
    m_console_buffer += output + m_render_state->Get_Cursor_Text() + "\n\r";

}


/****************************************/
/*      Build the Console Buffer        */
/****************************************/
void A_Console_Render_Manager::Build_Console_Buffer()
{
    // Create string
    m_console_buffer = "";//UTILS::ANSI_CLEARSCREEN;

}


} // End of CLI Namespace

