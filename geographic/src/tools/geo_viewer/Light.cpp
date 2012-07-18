#include "Light.h"

Light::Light(){
   m_position = vec4( 0.0, 1.0, 0.0, 1.0);
   m_ambient  = vec4( 0.7, 0.7, 0.7, 1.0);
   m_diffuse  = vec4( 1.3, 1.3, 1.3, 1.0);
   m_specular = vec4( 0.5, 0.5, 0.5, 1.0);
}

void Light::move_x( GLfloat dist ){
   m_position.x += dist;
}

void Light::move_z( GLfloat dist ){
   m_position.z += dist;
}
