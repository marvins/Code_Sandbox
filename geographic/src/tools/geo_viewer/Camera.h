#ifndef _CAMERA_STRUCTURE_OBJECT_FOR_AIRHOCKEY_AND_STUFF___DOT_H_
#define _CAMERA_STRUCTURE_OBJECT_FOR_AIRHOCKEY_AND_STUFF___DOT_H_

#include "GL.h"
#include "vector.h"
#include "mat.h"

enum CameraMode {CAMERA_FREE, CAMERA_Y_LOCK_VERT, CAMERA_Y_LOCK_BOTH};

// The world has no well defined up in free mode

// CAMERA_FREE        full pitch/yaw/roll camera
// CAMERA_Y_LOCK_VERT horizontal movements are restricted to XZ plane and roll is
//                    disabled, vertical movement is restricted to Y-axis
// CAMERA_Y_LOCK_BOTH forward and horizontal movements are restricted to XZ plane
//                    and roll is disabled, vertical movement is restricted to
//                    Y-axis

// Note: Y_LOCK modes give a sense that the worlds Y axis is up

class Camera
{
public:
   Camera(vec4 position = vec4(0.0,0.0,0.0,1.0), CameraMode mode = CAMERA_FREE);
   Camera(const Camera& rhs);

   Camera& operator=(const Camera& rhs);

   void resetView();
 
   // theta is in degrees
   void rotateStraight(GLfloat theta);
   void rotateVert(GLfloat theta);
   void rotateHoriz(GLfloat theta);

   void moveStraight(GLfloat dist);
   void moveHoriz(GLfloat dist);
   void moveVert(GLfloat dist);

   void setPosition(vec4 position);
   mat4 getTranslation() const;
   mat4 getRotation() const;

   const mat4& getTransform();
protected:
   void updateTransform();

   vec4 m_position;

   // direction camera is looking
   vec4 m_orientation;

   // the up vector (rotate around this when looking left/right)
   vec4 m_up;

   // the vector looking right (rotate around this when looking up/down)
   // (happens to be normal to the up and orientation vectors)
   vec4 m_normal;

   // rotation and translation matrices for the camera
   mat4 m_rotation;
   mat4 m_translation;
   
   // the current tranformation matrix for the camera
   mat4 m_transform;
   
   // if the same operation is done twice, the rotation matrix does
   // not need to be recomputed.  This is a special case but it should
   // come up quite often
   enum Dir {VERT, HORIZ, STRAIGHT, NONE};
   GLfloat m_last_theta;
   Dir m_last_direction;
   mat4 m_last_rot;

   CameraMode m_mode;
};

#endif // _CAMERA_STRUCTURE_OBJECT_FOR_AURHOCKEY_AND_STUFF_DOT___H_

