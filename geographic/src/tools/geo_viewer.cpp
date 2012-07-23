/*
   Marvin Smith
 */
/**
  @file    main.cpp
  @brief   Main driver of OpenGL Visualization Program.

  @author  Marvin Smith
  @date    7/22/2012
 */


//OpenGL Libraries
#ifdef __APPLE__
//#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
#endif

//Personal Libraries
#include "geo_viewer/initialization.h"
#include "geo_viewer/keyboard.h"
#include "geo_viewer/Options.h"
#include "geo_viewer/visualization.h"


//using namespace std;

/*
   Create list of parameters for program
   - stores application-specific values.
 */
Options options;


/**
  Main Driver, initializes OpenGL, and starts the main loop.
  @author  Marvin Smith
  @param argv  List of input arguements
  @date 7/19/2012
 */
void init_viewer( ){

    ///initialize parameters
    options.window_width  = 640;
    options.window_height = 480;
    options.start_x       =   5;
    options.start_y       =  40;

    options.debug_mode    = 1;
    
    options.camera.eye = vec3( 0, 0, -4);
    options.camera.at  = vec3( 0, 0,  0);
    options.camera.up  = vec3( 0, 1,  0);
    
    options.cam_lookStep = 0.1;
    options.cam_moveStep = 0.05;
    options.cam_timerStep = 10;

    options.dted_tile.set_filename("data/dted/w119/n037.dt2");
    options.dted_tile.set_init(true);

}

int main( int argc, char* argv[] ){
    srand(time(NULL));
    
    try{

    /* Initialize everything */
    init_viewer( );

    /* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
       X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */
    glutInit(&argc, argv);  

    /* Select type of Display mode:   
       Double buffer 
       RGBA color
       Alpha components supported 
       Depth buffered for automatic clipping 
    */ 
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  

    /* get a 640 x 480 window */
    glutInitWindowSize( options.window_width, options.window_height );  

    /* the window starts at the upper left corner of the screen */
    glutInitWindowPosition( options.start_x, options.start_y );  

    /* Open a window */
    options.main_window = glutCreateWindow( options.window_name.c_str() );  

    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawGLScene);  

    /* When mouse buttons are pushed */
    //glutMouseFunc(mouseEvent);

    /* When the mouse is dragged */
    //glutMotionFunc(mouseMove);

    /* Even if there are no events, redraw our gl scene. */
    glutTimerFunc(1, DrawGLTimer, 0);
    glutTimerFunc(6, cameraTimer, 0);
    
    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);

    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyboardPress);
    glutKeyboardUpFunc( &keyboardUp );

    /* Initialize our window. */
    InitGL( options.window_width, options.window_height );

    /* Start Event Processing Engine */ 
    glutMainLoop();  
    } catch( string e ){
        cout << e << endl;
    }
    return 1;
}

