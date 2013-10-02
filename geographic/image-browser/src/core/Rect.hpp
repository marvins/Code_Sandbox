/**
 * @file   Rect.hpp
 * @author Marvin Smith
 * @date   10/1/2013
*/
#ifndef __SRC_CORE_RECT_HPP__
#define __SRC_CORE_RECT_HPP__

#include <iostream>
#include <fstream>

#include <src/core/Point.hpp>

/**
 * @class Rect
*/
class Rect{

    public:

        /**
         * Default Constructor
        */
        Rect();

        /**
         * Parameterized Constructor
        */
        Rect( const Point& topLeft, const Point& bottomRight );

        
        /// Top Left
        Point tl;

        /// Bottom Right
        Point br;

};

std::ostream& operator << ( std::ostream& ostr, Rect const& rect );

#endif
