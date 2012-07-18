#ifndef __LIGHT_STRUCTURE_OBJECT_FOR_AIRHOCKEY_AND_STUFF__DOT_H_
#define __LIGHT_STRUCTURE_OBJECT_FOR_AIRHOCKEY_AND_STUFF__DOT_H_

#include "GL.h"
#include "vector.h"
#include "mat.h"

#include <iostream>

using namespace std;

class Light{
   public:
      
      void move_x( GLfloat dist);       
      void move_z( GLfloat dist);       

      Light();
      vec4 m_position;
      vec4 m_ambient;
      vec4 m_diffuse;
      vec4 m_specular;
};

#endif
