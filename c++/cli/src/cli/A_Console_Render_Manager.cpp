/**
 * @file    A_Console_Render_Manager.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_Console_Render_Manager.hpp"


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
/*      Refresh the screen      */
/********************************/
void A_Console_Render_Manager::Refresh()
{

    // Clear the window
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

