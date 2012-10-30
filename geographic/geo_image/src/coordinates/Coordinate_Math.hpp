#ifndef __SRC_COORDINATES_COORDINATE_MATH_HPP__
#define __SRC_COORDINATES_COORDINATE_MATH_HPP__

#ifdef DEPLOYED
#include <geoimage/coordinates/CoordinateUTM.hpp>
#else
#include "CoordinateUTM.hpp"
#endif

namespace GEO{

double coordinate_distance( CoordinateUTM const& coordA, CoordinateUTM const& coordB );

}


#endif
