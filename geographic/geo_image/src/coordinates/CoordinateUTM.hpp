#ifndef __SRC_COORDINATES_COORDINATEUTM_H__
#define __SRC_COORDINATES_COORDINATEUTM_H__

#include <opencv2/core/core.hpp>

#include <string> 

#ifdef DEPLOYED
#include <geoimage/coordinates/Datum.hpp>
#else
#include "Datum.hpp"
#endif


namespace GEO{

class CoordinateUTM {
    
    public:
        
        CoordinateUTM();
        
        CoordinateUTM( const int& _zone, const bool& _isNorth, const double& _easting, const double& _northing, const double& _elevation = 0, const std::string& _datum = "WGS84" );
        
        cv::Point3f toPoint3f( )const;

        std::string toString()const;

        int zone;
        bool   isNorth;
        double easting;
        double northing;
        
        double elevation;
        std::string datum;
};

}

#endif
