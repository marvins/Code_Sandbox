#include "camera.h"

#include "mat.h"

#include "Quaternion.h"

Camera::Camera(){
    eye = vec3( 0,0,-5);
    at  = vec3(0,0,0);
    up  = vec3( 0, 1, 0);

}

Camera::Camera( vec3 const& e, vec3 const& a, vec3 const& u ){

    eye = e;
    at = a;
    up = u;

}


void Camera::shift_horizontal( GLfloat const& amt ){

    //find the difference
    vec3 diff = at - eye;
    vec3 nrml = normalize( diff );
    
    //create a left-facing vector
    vec3 lft = normalize(cross( up, nrml ))*amt*-1;
    
    //add difference to both eye and center
    eye += lft;
    at  += lft;

}


void Camera::shift_straight( GLfloat const& amt ){

    //find the difference
    vec3 diff = at - eye;
    vec3 nrml = normalize( diff );
    
    //apply the distance to the front-facing vector
    vec3 res = nrml * amt;

    //add difference to both eye and vector
    eye += res;
    at  += res;
}

void Camera::shift_vertical( GLfloat const& amt ){

    //apply the result to the up vector
    vec3 res = up*amt;
    
    //apply to both the eye and at vector
    eye += res;
    at  += res;

}

void Camera::rotate_straight( GLfloat const& theta ){

    //compute the diff vector
    vec3 diff = normalize(at - eye);

    //use the diff as the axis of rotation
    vec3 axRot = diff;

    //rotate point about axis of rotation
    Quaternion rot( theta*3.14152/180.0, axRot );
    Quaternion valA( diff );
    Quaternion valU( up );
    
    at = (rot*valA*rot.conj()).imag() + eye;
    up = (rot*valU*rot.conj()).imag();

}

void Camera::rotate_vertical( GLfloat const& theta ){

    //compute the diff vector
    vec3 diff = normalize(at - eye);

    //compute the axis of rotation
    vec3 axisRotation = normalize(cross( up, diff));
    
    //rotate point about axis of rotation
    Quaternion rot( theta*3.14152/180.0, axisRotation);
    Quaternion valA( diff );
    Quaternion valU( up );

    at = (rot*valA*rot.conj()).imag() + eye;
    up = (rot*valU*rot.conj()).imag();

}

void Camera::rotate_horizontal( GLfloat const& theta ){
    
    
    //rotate point about axis of rotation
    vec3 diff = normalize( at - eye );
    
    //rotate point about axis of rotation
    Quaternion rot( theta*3.14152/180.0, up);
    Quaternion valA( diff );

    at = (rot*valA*rot.conj()).imag() + eye;
    
}

