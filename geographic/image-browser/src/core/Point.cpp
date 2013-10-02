/**
 * @file    Point.cpp
 * @author  Marvin Smith
 * @date    10/1/2013
*/
#include "Point.hpp"

using namespace std;

Point::Point( const double& x, const double& y ){

    this->x = x;
    this->y = y;
}

std::ostream& operator << ( std::ostream& ostr, Point const& pt ){
    ostr << "Point(" << pt.x << ", " << pt.y << ")";
    return ostr;
}
