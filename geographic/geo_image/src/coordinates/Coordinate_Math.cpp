#include "Coordinate_Math.hpp"

#include <cmath>

using namespace std;

namespace GEO{

double coordinate_distance( CoordinateUTM const& coordA, CoordinateUTM const& coordB ){

    // since UTM is in meters, just use the distance formula
    return sqrt( pow( coordA.easting   - coordB.easting,   2) + 
                 pow( coordA.northing  - coordB.northing,  2) +
                 pow( coordA.elevation - coordB.elevation, 2));

}


}
