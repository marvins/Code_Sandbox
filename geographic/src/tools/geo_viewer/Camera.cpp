#include "Camera.h"

Camera::Camera(vec4 position, CameraMode mode) :
   m_orientation(0.0,0.0,-1.0,1.0),
   m_up(0.0,1.0,0.0,1.0),
   m_normal(1.0,0.0,0.0,1.0),
   m_last_direction(NONE),
   m_mode(mode)
{
   // m_rotation is an identity using default constructor

   m_translation = mat4(1.0,0.0,0.0,-position.x,
                        0.0,1.0,0.0,-position.y,
                        0.0,0.0,1.0,-position.z,
                        0.0,0.0,0.0,1.0);

   updateTransform();
}

Camera::Camera(const Camera& rhs) :
   m_orientation(rhs.m_orientation),
   m_up(rhs.m_up),
   m_normal(rhs.m_normal),
   m_rotation(rhs.m_rotation),
   m_translation(rhs.m_translation),
   m_transform(rhs.m_transform),
   m_last_theta(rhs.m_last_theta),
   m_last_direction(rhs.m_last_direction),
   m_last_rot(rhs.m_last_rot),
   m_mode(rhs.m_mode)
{}

Camera& Camera::operator=(const Camera& rhs)
{
   m_orientation    = rhs.m_orientation;
   m_up             = rhs.m_up;
   m_normal         = rhs.m_normal;
   m_rotation       = rhs.m_rotation;
   m_translation    = rhs.m_translation;
   m_transform      = rhs.m_transform;
   m_last_theta     = rhs.m_last_theta;
   m_last_direction = rhs.m_last_direction;
   m_last_rot       = rhs.m_last_rot;
   m_mode           = rhs.m_mode;

   return *this;
}

void Camera::resetView()
{
   m_orientation = vec4(0.0,0.0,-1.0,1.0);
   m_up = vec4(0.0,1.0,0.0,1.0);
   m_normal = vec4(1.0,0.0,0.0,1.0);

   updateTransform();
}

void Camera::rotateStraight(GLfloat theta)
{
   // yaw function is disabled if camera is Y-Lock camera
   if ( m_mode == CAMERA_Y_LOCK_VERT || m_mode == CAMERA_Y_LOCK_BOTH )
      return;

   if ( m_last_direction != STRAIGHT || m_last_theta != theta )
   {
      m_last_rot = RotateAround( theta, m_orientation, true );
      m_last_direction = STRAIGHT;
      m_last_theta = theta;
   }

   m_up = m_last_rot*m_up;
   m_normal = m_last_rot*m_normal;

   m_rotation = LookAt( vec4(0.0,0.0,0.0,1.0), m_orientation, m_up );
  
   updateTransform();
}

void Camera::rotateVert(GLfloat theta)
{
   if ( m_last_direction != VERT || m_last_theta != theta )
   {
      m_last_rot = RotateAround( theta, m_normal, true );
      m_last_direction = VERT;
      m_last_theta = theta;
   }
  
   m_orientation = m_last_rot*m_orientation; 
   m_up = m_last_rot*m_up;

   m_rotation = LookAt( vec4(0.0,0.0,0.0,1.0), m_orientation, m_up );
  
   updateTransform();
}

void Camera::rotateHoriz(GLfloat theta)
{
   if ( m_last_direction != HORIZ || m_last_theta != theta )
   {
      if ( m_mode == CAMERA_FREE )
         m_last_rot = RotateAround( theta, m_up, false );
      else
         m_last_rot = RotateY( theta );

      m_last_direction = HORIZ;
      m_last_theta = theta;
   }
   
   m_orientation = m_last_rot*m_orientation;
   m_normal = m_last_rot*m_normal;
  
   // need to transform up vector during Y-lock mode
   if ( m_mode == CAMERA_Y_LOCK_VERT || m_mode == CAMERA_Y_LOCK_BOTH )
      m_up = m_last_rot*m_up;

   m_rotation = LookAt( vec4(0.0,0.0,0.0,1.0), m_orientation, m_up );
   
   updateTransform();
}

void Camera::moveStraight(GLfloat dist)
{
   if ( m_mode == CAMERA_FREE || m_mode == CAMERA_Y_LOCK_VERT )
   {
      vec4 translate = m_orientation*dist;

      m_translation[0][3] -= translate.x;
      m_translation[1][3] -= translate.y;
      m_translation[2][3] -= translate.z;
   }
   else // CAMERA_Y_LOCK_BOTH
   {
      // move forward along the y plane
      vec3 translate = cross(vec4(0.0,1.0,0.0,1.0), m_normal)*dist;

      m_translation[0][3] -= translate.x;
      m_translation[1][3] -= translate.y; // should be zero
      m_translation[2][3] -= translate.z;
   }

   updateTransform();
}

void Camera::moveHoriz(GLfloat dist)
{
   // same for free and y locked camera
   vec4 translate = m_normal*dist;

   m_translation[0][3] -= translate.x;
   m_translation[1][3] -= translate.y;
   m_translation[2][3] -= translate.z;
   
   updateTransform();
}

void Camera::moveVert(GLfloat dist)
{
   if ( CAMERA_FREE )
   {
      vec4 translate = m_up*dist;

      m_translation[0][3] -= translate.x;
      m_translation[1][3] -= translate.y; 
      m_translation[2][3] -= translate.z;
   }
   else
   {
      // move up the y axis
      m_translation[1][3] -= dist;
   }

   updateTransform();
}

void Camera::setPosition(vec4 position)
{
   m_translation[0][3] = -position.x;
   m_translation[1][3] = -position.y;
   m_translation[2][3] = -position.z;

   updateTransform();
}

mat4 Camera::getTranslation() const
{
   return m_translation;
}

mat4 Camera::getRotation() const
{
   return m_rotation;
}

const mat4& Camera::getTransform()
{
   return m_transform;
}

void Camera::updateTransform()
{
   m_transform = m_rotation*m_translation;
}

