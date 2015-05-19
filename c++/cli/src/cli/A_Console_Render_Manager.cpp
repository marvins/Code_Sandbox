/**
 * @file    A_Console_Render_Manager.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_Console_Render_Manager.hpp"

// C++ Standard Libraries
#include <iostream>

namespace CLI{


/****************************/
/*      Constructor         */
/****************************/
A_Console_Render_Manager::A_Console_Render_Manager()
  : m_class_name("A_Console_Render_Manager"),
    m_context(nullptr)
{
}


/****************************************/
/*      Update the NCurses Context      */
/****************************************/
void A_Console_Render_Manager::Update_NCurses_Context( NCURSES::An_NCurses_Context::ptr_t context )
{
    m_context = context;
}

/********************************/
/*      Initialize Curses       */
/********************************/
void A_Console_Render_Manager::Initialize()
{
    // Initialize NCurses
    NCURSES::Initialize( m_context );
}

/********************************/
/*        Finalize Curses       */
/********************************/
void A_Console_Render_Manager::Finalize()
{
    // Finalize NCurses
    NCURSES::Finalize( m_context );
}


/********************************/
/*      Refresh the screen      */
/********************************/
void A_Console_Render_Manager::Refresh()
{

    // Clear the window
    if( m_context->main_window == NULL || 
        m_context->main_window == nullptr ){
        std::cerr << "warning: Main Window is still null." << std::endl;
        return;
    }

    wrefresh( m_context->main_window );

    // Draw everthing

}


/************************************/
/*      Wait on Keyboard Input      */
/************************************/
int A_Console_Render_Manager::Wait_Keyboard_Input()
{

    // Wait
    return wgetch( m_context->main_window );
}

} // End of CLI Namespace

