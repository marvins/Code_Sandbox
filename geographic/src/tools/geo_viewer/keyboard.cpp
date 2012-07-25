#include "keyboard.h"

#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#endif


/**
 * Camera Class
*/
struct CameraAction
{
   CameraAction() :
      rotateStraight(0),
      rotateVert(0),
      rotateHoriz(0),
      moveStraight(0),
      moveHoriz(0),
      moveVert(0),
      keysPressed(0) {}

   int rotateStraight;
   int rotateVert;
   int rotateHoriz;
   int moveStraight;
   int moveHoriz;
   int moveVert;

   // number of (meaningful) keys currently held down
   int keysPressed;
};


CameraAction action;

   
void cameraTimer(int value)
{
    if ( action.moveHoriz != 0 ){
        options.camera.shift_horizontal(action.moveHoriz*options.cam_moveStep);
    }
    if ( action.moveStraight != 0 ){
        options.camera.shift_straight(action.moveStraight*options.cam_moveStep);
    }
    if ( action.moveVert != 0 ){
        options.camera.shift_vertical(action.moveVert*options.cam_moveStep);
    }
    if ( action.rotateVert != 0 ){
        options.camera.rotate_vertical(action.rotateVert*options.cam_moveStep);
    }
    if ( action.rotateHoriz != 0 ){
        options.camera.rotate_horizontal(action.rotateHoriz*options.cam_moveStep);
    }
    if ( action.rotateStraight != 0 ){
        options.camera.rotate_straight( action.rotateStraight*options.cam_moveStep);
    }
 
    glutTimerFunc( 10, cameraTimer, value);

}

void keyboardPress( unsigned char key, int x, int y ){

    /** Process for the key */
    bool cameraKeyPressed = false;
    
    switch( key ){

        /********************************/
        /*        Camera Actions        */
        /********************************/
        case 'a':
            action.moveHoriz--;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;

        case 'd':
            action.moveHoriz++;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;

        case 'w':
            action.moveStraight++;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;

        case 's':
            action.moveStraight--;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;
        
        case 'e':
            action.moveVert++;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;
        
        case 'q':    
            action.moveVert--;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;

        case 'i':
            action.rotateVert++;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;
        
        case 'k':
            action.rotateVert--;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;
        
        case 'l':
            action.rotateHoriz++;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;
        
        case 'j':
            action.rotateHoriz--;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;
        
        case 'u':
            action.rotateStraight = -1;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;

        case 'o':
            action.rotateStraight = 1;
            action.keysPressed++;
            cameraKeyPressed = true;
            break;
        
        case '7':
            options.znear -= 1;
            break;
        case '8':
            options.znear += 1;
            break;
        case '9':
            options.zfar -= 1;
            break;
        case '0':
            options.zfar += 1;
            break;

            /**********************/
            /*    Quit Program    */
            /**********************/
        case 033: //escape key
            exit( EXIT_SUCCESS );     
    }


}

void keyboardUp( unsigned char key, int x, int y )
{
    /** Process for the key */
    switch( key ){


        /********************************/
        /*        Camera Actions        */
        /********************************/
        case 'a':
            action.moveHoriz++;
            action.keysPressed--;
            break;

        case 'd':
            action.moveHoriz--;
            action.keysPressed--;
            break;
        
        case 'w':
            action.moveStraight--;
            action.keysPressed--;
            break;
        
        case 's':
            action.moveStraight++;
            action.keysPressed--;
            break;
        
        case 'q':
            action.moveVert++;
            action.keysPressed--;
            break;

        case 'e':
            action.moveVert--;
            action.keysPressed--;
            break;

        case 'i':
            action.rotateVert--;
            action.keysPressed--;
            break;

        case 'k':
            action.rotateVert++;
            action.keysPressed--;
            break;
        
        case 'l':
            action.rotateHoriz--;
            action.keysPressed--;
            break;

        case 'j':
            action.rotateHoriz++;
            action.keysPressed--;
            break;
        
        case 'u':
            action.rotateStraight = 0;
            action.keysPressed--;
            break;

        case 'o':
            action.rotateStraight = 0;
            action.keysPressed--;
            break;
        
            /**********************/
            /*    Quit Program    */
            /**********************/
        case 033: //escape key
            exit( EXIT_SUCCESS );     
        
        action.keysPressed = 0;
    }

}

