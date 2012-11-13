#ifndef __SRC_COORDINATES_COORDINATEUTM_H__
#define __SRC_COORDINATES_COORDINATEUTM_H__



#ifdef DEPLOYED
#include <geoimage/coordinates/Datum.hpp>
#include <geoimage/io/IO_Configuration.hpp>
#else
#include "Datum.hpp"
#include "../io/IO_Configuration.hpp"
#endif

#if USE_OPENCV == 1
#include <opencv2/core/core.hpp>
#endif

#include <string> 

namespace GEO{

class CoordinateUTM {
    
    public:
        
        CoordinateUTM();
        
        CoordinateUTM( const int& _zone, const bool& _isNorth, const double& _easting, const double& _northing, const double& _elevation = 0, const std::string& _datum = "WGS84" );
        
#if USE_OPENCV == 1
        cv::Point3f toPoint3f( )const;
#endif

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
