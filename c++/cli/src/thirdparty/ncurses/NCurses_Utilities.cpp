/**
 * @file    NCurses_Utilities.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "NCurses_Utilities.hpp"

namespace NCURSES{


/**********************************/
/*          Constructor           */
/**********************************/
An_NCurses_Context::An_NCurses_Context()
  : screen(NULL),
    main_window(NULL),
    tty_terminal_name(""),
    tty_in(NULL),
    tty_out(NULL),
    m_class_name("An_NCurses_Context")
{
}



/*************************************/
/*      Create NCurses Context       */
/*************************************/
void Initialize( An_NCurses_Context::ptr_t  context )
{
    // Make sure the context is not null
    if( context == nullptr ){
        return;
    }

    // Create the screen
    context->screen  = newterm( 0, 
                                context->tty_in,
                                context->tty_out );

    // Do not do nl->cr/nl

    // CBreak
    cbreak();

    // Setup colors
    

    // Create the window
    context->main_window = newwin( LINES, COLS, 0, 0 );

    // Set the keyboard
    keypad( context->main_window, TRUE );
}


/****************************************/
/*      Finalize NCurses Context        */
/****************************************/
void Finalize( An_NCurses_Context::ptr_t   context )
{

    // Make sure the context is not null
    if( context == nullptr ){
        return;
    }
    
    // End the window
    endwin();

    // Delete the window
    if( context->main_window != NULL && context->main_window != nullptr )
    {
        delwin( context->main_window );
    }

    // Delete the screen
    if( context->screen != NULL && context->screen != nullptr )
    {
        delscreen( context->screen );
    }

}


/*****************************/
/*      Abort NCurses        */
/*****************************/
void Abort()
{
    // Shutdown
    endwin();
}



} // End of NCURSES Namespace

