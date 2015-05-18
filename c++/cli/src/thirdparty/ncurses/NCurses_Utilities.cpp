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
  : m_class_name("An_NCurses_Context"),
    screen(NULL),
    tty_in(NULL),
    tty_out(NULL)
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

    // Create the window
    context->screen  = newterm( 0, 
                                context->tty_in,
                                context->tty_out );


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
    
    // Delete the screen
    if( context->screen != NULL || context->screen != nullptr )
    {
        delscreen( context->screen );
    }

}


} // End of NCURSES Namespace

