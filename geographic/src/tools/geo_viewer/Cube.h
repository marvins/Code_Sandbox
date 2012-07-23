#ifndef __SRC_STRUCTURES_SHAPES_CUBE_H__
#define __SRC_STRUCTURES_SHAPES_CUBE_H__

//STL and CSTD Libraries
#include <iostream>
#include <vector>

#include "vector.h"

using namespace std;

class Cube {

   public:
      Cube();
      Cube(vector<vec4>const& a );
      Cube(Cube const& a );

      virtual ~Cube( );

      virtual void set_size( vector<vec4>const& a );

      virtual void draw_shape( );

      virtual size_t num_vertices( )const;
      virtual ostream& print_shape( ostream& ostr );
   
   private:
      vec4 data[8];

};


#endif
