/**
 * @file   Rect.cpp
 * @author Marvin Smith
 * @date   10/1/2013
*/

#include "Rect.hpp"

using namespace std;

Rect::Rect(){
    
    tl = Point(0,0);

    br = Point(0,0);

}

Rect::Rect( Point const& topLeft, Point const& bottomRight ){

    tl = topLeft;
    br = bottomRight;
}


std::ostream& operator << ( std::ostream& ostr, Rect const& rect ){
    ostr << "Rect( TL:" << rect.tl << ", BR: " << rect.br << ")";
    return ostr;
}
