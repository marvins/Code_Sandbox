#ifndef __SRC_COORDINATES_COORDINATEUTM_H__
#define __SRC_COORDINATES_COORDINATEUTM_H__

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
        
        CoordinateUTM( const int& _zone, const char& _hemisphere, const double& _easting, const double& _northing );

        std::string toString()const;

        int zone;
        char hemisphere;
        double easting;
        double northing;
        
        double elevation;
        int datum;
};

}

#endif
