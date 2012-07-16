#include "display.h"

void display_function( void )
{
    //Clear the screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    switch (options.view_mode)
    {
        case 0:
        case 1:
            /******************************************/
            /*               Game Display             */
            /******************************************/
            glClearColor( 0.5, 0.5, 0.5, 1.0 );


            break;

    }
    glutSwapBuffers();

}



