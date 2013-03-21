#include "CoordinateLatLon.hpp"

#include <sstream>
#include <string>

using namespace cv;
using namespace std;


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
CoordinateLatLon::CoordinateLatLon( const Point2d& coord, const double& _elevation, const string& _datum ){

    lat   = coord.y;
    lon   = coord.x;
    elevation = _elevation;
    datum = _datum;

}

/**
 * Constructor for decimal degrees.  Z is the elevation
*/
CoordinateLatLon::CoordinateLatLon( const cv::Point3d& coord, std::string const& _datum ){

    lat = coord.y;
    lon = coord.x;
    elevation = coord.z;
    
    datum = _datum;
}


/**
 * @brief CoordinateLatLon::CoordinateLatLon
 * @param latDeg  Latitude degree component
 * @param latMin  Latitude minute component
 * @param lonDeg  Longitude degree component
 * @param lonMin  Longitude degree component
 * @param _elevation
 * @param _datum Geographic Datum used (WGS84 is default)
 */
CoordinateLatLon::CoordinateLatLon( const int& latDeg, const double& latMin, 
                                    const int& lonDeg, const double& lonMin,
                                    const double& _elevation,
                                    const string& _datum ){

    // degrees are a direct copy
    lat = latDeg;
    lon = lonDeg;

    // minutes are divided by 60, then added
    lat += (latMin/60.0);
    lon += (lonMin/60.0);
    // set the rest
    elevation = _elevation;
    datum = _datum;
        
    throw string("ERROR: Not Implemented");
}

CoordinateLatLon::CoordinateLatLon( const int& latDeg, const int&    latMin, const double& latSec,
                                    const int& lonDeg, const int&    lonMin, const double& lonSec,
                                    const double& _elevation,
                                    const string& _datum ){

    // set the datum
    datum = _datum;
    
    // compute our required latitude
    if( latDeg >= 0 )
        lat = latDeg + latMin/60.0 + latSec/3600.0;
    else
        lat = latDeg - latMin/60.0 - latSec/3600.0;
    
    // set our required longitude
    if( lonDeg >= 0 )
        lon = lonDeg + lonMin/60.0 + lonSec/3600.0;
    else    
        lon = lonDeg - lonMin/60.0 - lonSec/3600.0;

    // set the elevation
    elevation = _elevation;

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


cv::Point2d CoordinateLatLon::toPoint2d( )const{
    return Point2d( lon, lat );
}

cv::Point3d CoordinateLatLon::toPoint3d( )const{
    return Point3d( lon, lat, elevation );
}
