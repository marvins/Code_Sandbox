#include "camera.h"

#include "mat.h"

Camera::Camera(){
    eye = vec4( 0,0,-5);
    at  = vec4(0,0,0);
    up  = vec4( 0, 1, 0);

}

Camera::Camera( vec4 const& e, vec4 const& a, vec4 const& u ){

    eye = e;
    at = a;
    up = u;

}


void Camera::shift_horizontal( GLfloat const& amt ){

    //find the difference
    vec4 diff = at - eye;
    vec4 nrml = normalize( diff );
    
    //create a left-facing vector
    vec4 lft = normalize(cross( up, nrml ))*amt*-1;
    
    //add difference to both eye and center
    eye += lft;
    at  += lft;

}


void Camera::shift_straight( GLfloat const& amt ){

    //find the difference
    vec4 diff = at - eye;
    vec4 nrml = normalize( diff );
    
    //apply the distance to the front-facing vector
    vec4 res = nrml * amt;

    //add difference to both eye and vector
    eye += res;
    at  += res;
}

void Camera::shift_vertical( GLfloat const& amt ){

    //apply the result to the up vector
    vec4 res = up*amt;
    
    //apply to both the eye and at vector
    eye += res;
    at  += res;

}

void Camera::rotate_vertical( GLfloat const& theta ){

    //compute the axis of rotation
    vec4 axisRotation = normalize(cross( up, eye));
    
    //rotate point about axis of rotation
    at = RotateAround( theta, axisRotation, true )*at;

}

void Camera::rotate_horizontal( GLfloat const& theta ){

    //rotate point about axis of rotation
    at = RotateAround( theta, up, true )*at;

}

