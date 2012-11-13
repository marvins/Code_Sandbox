#include "DEM_Utilities.hpp"

#include <cmath>
#include <iostream>
#include <sstream>


#if USE_OPENCV == 1
using namespace cv;
#endif

using namespace std;

namespace GEO{

std::string DTED_coordinate2filename( double const& lat, double const& lon ){

    int lowerLat = std::floor(lat);
    int lowerLon = std::floor(lon);
    
    string latH = "n";
    string lonH = "e";
    if( lowerLat < 0 ){
        lowerLat = fabs(lowerLat);
        latH = "s";
    }
    if( lowerLon < 0 ){
        lowerLon = fabs(lowerLon);
        lonH = "w";
    }
  
    string slat, slon;
    stringstream sin;
    sin << lowerLat;
    sin >> slat;
    sin.clear();
    sin.str("");
    sin << lowerLon;
    sin >> slon;
    sin.clear();
    sin.str("");
    string spaceLat, spaceLon;
    if(      lowerLat < 10 ) spaceLat = "00";
    else if( lowerLat < 100) spaceLat = "0";
    else                     spaceLat = "";
    if(      lowerLon < 10 ) spaceLon = "00";
    else if( lowerLon < 100) spaceLon = "0";
    else                     spaceLon = "";
    return (lonH + spaceLon + slon + string("/") + latH + spaceLat+ slat + string(".dt2"));
}


#if USE_OPENCV == 1
void DTED_adjust_needed_tiles( int& lat_needed, int& lon_needed, const Point2f& _min, const Point2f& _max ){
    
    // 
    double lon_range = _max.x - _min.x;
    if( fabs( lon_range - lon_needed + 1 ) < 0.0001 && fabs( std::floor(_min.x) - _min.x) < 0.00001 ) lon_needed--;
    
    double lat_range = _max.y - _min.y;
    if( fabs( lat_range - lat_needed + 1 ) < 0.0001 && fabs( std::floor(_min.y) - _min.y) < 0.00001 ) lat_needed--;

}
#endif

}///end of GEO namespace
