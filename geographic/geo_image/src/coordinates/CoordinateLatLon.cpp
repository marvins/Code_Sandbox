#include "CoordinateLatLon.hpp"

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
    elevation = 0;
    datum = "WGS84";
}

/**
 * Constructor for Decimal Degree Computation
*/
CoordinateLatLon::CoordinateLatLon( const double& latDD, const double& lonDD, const double& _elevation, const string& _datum ){

    lat   = latDD;
    lon   = lonDD;
    elevation = _elevation;
    datum = _datum;

}


/**
 * Constructor for Decimal Degrees.  Note that the coord.x is lon and .y is lat.
*/
CoordinateLatLon::CoordinateLatLon( const Point2f& coord, const double& _elevation, const string& _datum ){

    lat   = coord.y;
    lon   = coord.x;
    elevation = _elevation;
    datum = _datum;

}

/**
 * Constructor for decimal degrees.  Z is the elevation
*/
CoordinateLatLon::CoordinateLatLon( const cv::Point3f& coord, std::string const& _datum ){

    lat = coord.y;
    lon = coord.x;
    elevation = coord.z;
    
    datum = _datum;
}


CoordinateLatLon::CoordinateLatLon( const int& latDeg, const double& latMin, 
                                    const int& lonDeg, const double& lonMin,
                                    const double& _elevation,
                                    const string& _datum ){

    
    elevation = _elevation;
    datum = _datum;
        
    throw string("ERROR: Not Implemented");
}

CoordinateLatLon::CoordinateLatLon( const int& latDeg, const int&    latMin, const double& latSec,
                                    const int& lonDeg, const int&    lonMin, const double& lonSec,
                                    const double& _elevation,
                                    const string& _datum ){

    datum = _datum;
    
    if( latDeg >= 0 )
        lat = latDeg + latMin/60.0 + latSec/3600.0;
    else
        lat = latDeg - latMin/60.0 - latSec/3600.0;
    
    if( lonDeg >= 0 )
        lon = lonDeg + lonMin/60.0 + lonSec/3600.0;
    else    
        lon = lonDeg - lonMin/60.0 - lonSec/3600.0;

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
    
    string output = string("LatLon<") + slat + string(", ") + slon + string(", datum: ") + datum + string(" >");
    return output;

}

#if USE_OPENCV == 1

cv::Point2f CoordinateLatLon::toPoint2f( )const{
    return Point2f( lon, lat );
}

cv::Point3f CoordinateLatLon::toPoint3f( )const{
    return Point3f( lon, lat, elevation );
}


#endif

} //end of GEO namespace 
