#include "display.h"

void display_function( void )
{
    //Clear the screen
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    cout << "camera position: " << options.camera.getTranslation() << endl;;
    /******************************************/
    /*               Game Display             */
    /******************************************/
    glClearColor( 0.5, 0.5, 0.5, 1.0 );
    options.test_box->draw_shape(   options.camera.getTransform(), 
            options.projectionMatrix, 
            options.light.m_position); 

    glutSwapBuffers();

}



