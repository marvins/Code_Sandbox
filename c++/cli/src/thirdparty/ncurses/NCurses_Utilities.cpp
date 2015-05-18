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

    // Create the window



}


/****************************************/
/*      Finalize NCurses Context        */
/****************************************/
void Finalize( An_NCurses_Context::ptr_t   context )
{


}


} // End of NCURSES Namespace

