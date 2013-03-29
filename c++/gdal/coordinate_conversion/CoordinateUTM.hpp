#ifndef __SRC_COORDINATES_COORDINATEUTM_H__
#define __SRC_COORDINATES_COORDINATEUTM_H__


#include <opencv2/core/core.hpp>

#include <string> 

class CoordinateUTM {
    
    public:
        
        CoordinateUTM();
        
        CoordinateUTM( const int& _zone, const bool& _isNorth, const double& _easting, const double& _northing, const double& _elevation = 0, const std::string& _datum = "WGS84" );
        
        CoordinateUTM( const int& _zone, const bool& _isNorth, const cv::Point2d& coord, const double& _elevation = 0, const std::string& _datum = "WGS84" );
        
        CoordinateUTM( const int& _zone, const bool& _isNorth, const cv::Point3d& coord, const std::string& _datum = "WGS84" );

        cv::Point2d toPoint2d( )const;
        cv::Point3d toPoint3d( )const;

        CoordinateUTM operator + ( CoordinateUTM const& rhs )const;
        CoordinateUTM operator - ( CoordinateUTM const& rhs )const;

        double mag2()const;
        double mag()const;

        std::string toString()const;

        int zone;
        bool   isNorth;
        double easting;
        double northing;
        
        double elevation;
        std::string datum;
};

CoordinateUTM operator * ( CoordinateUTM const& pnt, double const& s );
CoordinateUTM operator * ( double const& s, CoordinateUTM const& pnt );

double  dot( CoordinateUTM const& PA, CoordinateUTM const& PB );


#endif
