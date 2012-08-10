#ifndef __SRC_COORDINATES_LATLONCOORD_H__
#define __SRC_COORDINATES_LATLONCOORD_H__

#include <opencv2/core/core.hpp>

#include <string>

#include "CoordinateBase.h"

namespace GEO{

class CoordinateLatLon : public CoordinateBase {

    public:
        
        /** 
         * CoordinateLatLon Constructor 
        */
        CoordinateLatLon();
        
        /**
         * Decimal Degree Constructor 
         *
         * @param[in] latDD Latitude  in Decimal Degree Format
         * @param[in] lonDD Longitude in Decimal Degree Format
        */
        CoordinateLatLon( const double& latDD, const double& lonDD );
        
        /**
         * OpenCV Point Constructor 
         *
         * @param[in] coord OpenCV Coordinate
        */
        CoordinateLatLon( const cv::Point2f& coord );
        
        /**
         * Lat Lon Coordinate in Degree, Minutes Format
         *
         * @param[in] latDeg Latitude Degree Component
         * @param[in] latMin Latitude Minutes Component
         * @param[in] lonDeg Longitude Degree Component
         * @param[in] lonMin Longitude Minutes Component
        */
        CoordinateLatLon( const int& latDeg, const double& latMin, 
                          const int& lonDeg, const double& lonMin);
        
        /**
         * Lat Lon Coordinate in Degree, Minutes Format
         *
         * @param[in] latDeg Latitude  Degree  Component
         * @param[in] latMin Latitude  Minutes Component
         * @param[in] latSec Latitude  Seconds Component
         * @param[in] lonDeg Longitude Degree  Component
         * @param[in] lonMin Longitude Minutes Component
         * @param[in] lonSec Longitude Seconds Component
        */
        CoordinateLatLon( const int& latDeg, const int&  latMin, const double& latSec,
                          const int& lonDeg, const int&  lonMin, const double& lonSec );

        
        
        /** 
          * Convert the Coordinate into a readable string 
        */
        std::string toString()const;


        double lat; /*< Latitude  */
        double lon; /*< Longitude */

};

}// end of GEO namespace 

#endif
