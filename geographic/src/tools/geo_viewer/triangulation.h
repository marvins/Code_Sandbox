/**
  * @file Triangulation.h
  * @brief Contains code pertaining to polygon triangulation
  *
  * @author Marvin Smith
*/
#ifndef __SRC_STRUCTURES_MATH_TRIANGULATION_H__
#define __SRC_STRUCTURES_MATH_TRIANGULATION_H__

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>

#include "vector.h"

using namespace std;

/**
  * @class Triangulate
  * @brief useful functions for PiP and math problems
*/
class Triangulate{

   public:

      /**
        * triangulate a contour/polygon, places results in STL
        * vector as series of triangles
        *
        * @param[in] contour list of vertices making up polygon
        * @param[out] result  list of triangles making up polygon
        * @return whether or not this is a non-simple polygon
      */
      static bool Process( const vector<vec4>& contour, vector<vec4>& result);

      /**
        * Compute the area of a contour/Polygon
        *
        * @param[in] contour Set of vertices to measure
        * @return Area of region
      */
      static float Area( const vector<vec4>& contour );

      /**
        * Decide if point (Px,Py) is inside triangle defined by
        * (Ax,Ay) (Bx,By) (Cx,Cy).
        *
        * @param[in] Ax First triangle corner X
        * @param[in] Ay First triangle corner Y
        * @param[in] Bx Second triangle corner X
        * @param[in] By Second triangle corner Y
        * @param[in] Cx Third triangle corner X
        * @param[in] Cy Third triangle corner Y
        * @param[in] Px Test Point X
        * @param[in] Py Test Point Y
        * @return if the point is inside
      */
      static bool InsideTriangle( float Ax, float Ay, float Bx, float By, 
                                  float Cx, float Cy, float Px, float Py  );

   private:
      /**
        * Remove the triangle from the set if it fails
        *
        * @param[in/out] contour Polygon to be tested
        * @param[in] u
        * @param[in] v
        * @param[in] w
        * @param[in] n
        * @param[in] w
        * @param[in] V
      */
      static bool Snip( const vector<vec4>& contour, int u, int v, int w, int n, int *V);

};

#endif
