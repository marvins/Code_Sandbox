/**
 * @file    Point.hpp
 * @author  Marvin Smith
 * @date    10/1/2013
*/
#ifndef __SRC_CORE_POINT_HPP__
#define __SRC_CORE_POINT_HPP__

#include <iostream>
#include <fstream>

/**
 * @class Point
*/
class Point{

    public:
        
        /**
         * Default Construtor
        */
        Point( double const& x = 0, double const& y = 0 );

        /// X Coordinate
        double x;

        /// Y Coordinate
        double y;
};

std::ostream& operator << ( std::ostream& ostr, Point const& pt );

#endif
