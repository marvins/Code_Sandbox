#ifndef __SRC_STRUCTURES_SHAPES_CUBE_H__
#define __SRC_STRUCTURES_SHAPES_CUBE_H__

//STL and CSTD Libraries
#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>

#include "vector.h"

using namespace std;

class DEMPoint{
    public:
        
        DEMPoint();
        DEMPoint( int a, int b, int l );
        DEMPoint( int a, int b, int l, double col );
        double x;
        double y;
        double z;
        double c;
};

class Cube {

   public:
      Cube();
      Cube(vector<vec4>const& a );
      Cube(Cube const& a );
      
      void set_structure( cv::Mat const& dted_image, cv::Point2f const& ul, cv::Point2f const& br );

      virtual ~Cube( );

      virtual void set_size( vector<vec4>const& a );

      virtual void draw_shape( );

      virtual size_t num_vertices( )const;
      virtual ostream& print_shape( ostream& ostr );
   
      cv::Point2f get_center()const;

   private:
      vec4 data[8];
      vector<int> indeces;
      vector<DEMPoint> elevation_info;

      cv::Point2f center;
};


#endif
