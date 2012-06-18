#include "DTEDUtils.h"

#include <cmath>
#include <iostream>
#include <sstream>

using namespace std;

namespace GEO{

std::string DTEDUtils::coordinate2filename( double const& lat, double const& lon ){

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

}///end of GEO namespace
