#include "CoordinateLatLon.h"

#include <sstream>
#include <string>

using namespace cv;
using namespace std;

namespace GEO{

/**
 * Default constructor for Lat Lon Coordinate
*/
CoordinateLatLon::CoordinateLatLon(){
    lat = 0;
    lon = 0;
    datum = WGS84;
}

CoordinateLatLon::CoordinateLatLon( const double& latDD, const double& lonDD, const int& dat ){

    lat   = latDD;
    lon   = lonDD;
    datum = dat;

}


CoordinateLatLon::CoordinateLatLon( const Point2f& coord, const int& dat ){

    lat   = coord.y;
    lon   = coord.x;
    datum = dat;

}

CoordinateLatLon::CoordinateLatLon( const int& latDeg, const double& latMin, 
                                    const int& lonDeg, const double& lonMin,
                                    const int& dat ){

    datum = dat;
        
    throw string("ERROR: Not Implemented");
}

CoordinateLatLon::CoordinateLatLon( const int& latDeg, const int&    latMin, const double& latSec,
                                    const int& lonDeg, const int&    lonMin, const double& lonSec,
                                    const int& dat ){

    datum = dat;
    
    throw string("ERROR: Not Implemented");
}


std::string CoordinateLatLon::toString()const{

    std::string slat, slon;
    std::stringstream sin;
    
    sin << lat;
    sin >> slat;
    sin.clear();
    sin.str("");
    
    sin << lon;
    sin >> slon;
    
    string output = string("LatLon<") + slat + string(", ") + slon + string(", datum: ") + datum2string(datum) + string(" >");
    return output;

}

} //end of GEO namespace 
