/**
 * @file    A_Console_Render_Manager_NCurses.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_Console_Render_Manager_NCurses.hpp"

// C++ Standard Libraries
#include <iostream>
#include <sstream>

namespace CLI{


/****************************/
/*      Constructor         */
/****************************/
A_Console_Render_Manager_NCurses::A_Console_Render_Manager_NCurses()
  : m_class_name("A_Console_Render_Manager_NCurses"),
    m_context(nullptr),
    m_render_state(std::make_shared<A_Console_Render_State>( CLIConnectionType::LOCAL ))
{
}


/****************************************/
/*      Update the NCurses Context      */
/****************************************/
void A_Console_Render_Manager_NCurses::Update_NCurses_Context( NCURSES::An_NCurses_Context::ptr_t context )
{
    m_context = context;
}

/********************************/
/*      Initialize Curses       */
/********************************/
void A_Console_Render_Manager_NCurses::Initialize()
{

    // Initialize NCurses
    NCURSES::Initialize( m_context );

    // Create new render state
    m_render_state.reset(new A_Console_Render_State( CLIConnectionType::LOCAL));

    // Set the size
    m_render_state->Set_Window_Size( LINES, COLS );

}

/********************************/
/*        Finalize Curses       */
/********************************/
void A_Console_Render_Manager_NCurses::Finalize()
{
    // Finalize NCurses
    NCURSES::Finalize( m_context );
}


/********************************/
/*      Refresh the screen      */
/********************************/
void A_Console_Render_Manager_NCurses::Refresh()
{

    // Clear the window
    clear();
    
    // Draw the header
    Print_Header();

    
    // Draw the main context
    Print_Main_Content();

    
    // Draw the footer
    Print_Footer();

    
    // Draw the CLI
    Print_CLI();


    // Refresh the window
    refresh();

}


/************************************/
/*      Wait on Keyboard Input      */
/************************************/
int A_Console_Render_Manager_NCurses::Wait_Keyboard_Input()
{

    // Wait
    return getch();
}


/****************************************/
/*          Print the header            */
/****************************************/
void A_Console_Render_Manager_NCurses::Print_Header()
{
    // header string
    std::string header_string = m_cli_title;

    // Move to the top-left corner
    mvprintw( 0, 0, header_string.c_str() );

}


/************************************************/
/*          Print the Main Context              */
/************************************************/
void A_Console_Render_Manager_NCurses::Print_Main_Content()
{



}


/****************************************/
/*          Print the Footer            */
/****************************************/
void A_Console_Render_Manager_NCurses::Print_Footer()
{


}


/********************************/
/*          Print the CLI       */
/********************************/
void A_Console_Render_Manager_NCurses::Print_CLI()
{
    // Set the CLI Row
    int cli_row = m_render_state->Get_Rows()-2;
    int cli_col = 2;

    // Print the Command-Line
    mvprintw( cli_row, cli_col, "cmd: " );
    cli_col += 5;
    
    // Print the rest
    printw( m_render_state->Get_Cursor_Text().c_str());
    
    // Set the cursor
    move( cli_row, cli_col + m_render_state->Get_Cursor_Pos() );
}


} // End of CLI Namespace

