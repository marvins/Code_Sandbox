/**
 * @file   Rect.cpp
 * @author Marvin Smith
 * @date   10/1/2013
*/

#include "Rect.hpp"

Rect::Rect(){
    
    tl = Point(0,0);

    br = Point(0,0);

}

Rect::Rect( Point const& topLeft, Point const& bottomRight ){

    tl = topLeft;
    br = bottomRight;
}


