#ifndef __SRC_COORDINATES_LATLONCOORD_H__
#define __SRC_COORDINATES_LATLONCOORD_H__

#include <string>

#include <opencv2/core/core.hpp>


class CoordinateLatLon{

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
        CoordinateLatLon( const double& latDD, const double& lonDD, const double& _elevation = 0, std::string const& dat = "WGS84" );
        
        /**
         * OpenCV Point Constructor 
         *
         * @param[in] coord OpenCV Coordinate
        */
        CoordinateLatLon( const cv::Point2d& coord, const double& _elevation, std::string const& dat = "WGS84" );

        /**
         * OpenCV Point Constructor
         *
         * @param[in] coord OpenCV Coordinate
        */
        CoordinateLatLon( const cv::Point3d& coord, std::string const& dat = "WGS84" );
        
        /**
         * Lat Lon Coordinate in Degree, Minutes Format
         *
         * @param[in] latDeg Latitude Degree Component
         * @param[in] latMin Latitude Minutes Component
         * @param[in] lonDeg Longitude Degree Component
         * @param[in] lonMin Longitude Minutes Component
        */
        CoordinateLatLon( const int& latDeg, const double& latMin, 
                          const int& lonDeg, const double& lonMin,
                          const double& _elevation = 0,
                          std::string const& _datum = "WGS84" );
        
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
                          const int& lonDeg, const int&  lonMin, const double& lonSec,
                          const double& _elevation = 0,
                          std::string const& _datum = "WGS84" );

        
        
        /** 
          * Convert the Coordinate into a readable string 
        */
        std::string toString()const;
        
        /**
         * Convert the Coordinate into an OpenCV Point
        */
        cv::Point2d toPoint2d( )const;

        /**
         * Convert the Coordinate into an OpenCV Point
        */
        cv::Point3d toPoint3d( )const;

        double lat;        /*< Latitude               */
        double lon;        /*< Longitude              */
        double elevation;  /*< Meters above sea level */
        
        std::string datum; /*< Datum     */
};


#endif
