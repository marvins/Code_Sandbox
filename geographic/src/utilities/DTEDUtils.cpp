#include "DTEDUtils.h"

#include <cmath>
#include <iostream>
#include <sstream>

using namespace std;

namespace GEO{

std::string DTEDUtils::coordinate2filename( double const& lat, double const& lon ){

    int lowerLat = std::floor(lat);
    int lowerLon = std::floor(lon);
    
    cout << lowerLat << ", " << lowerLon << endl;
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

    cout << "slat: " << slat << ", slon: " << slon << endl;

    std::string filename;
    return "hello";
}

}///end of GEO namespace
