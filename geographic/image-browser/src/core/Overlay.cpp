/** 
 * @file    Overlay.cpp
 * @author  Marvin Smith
 * @date    10/2/2013
*/
#include <src/core/Overlay.hpp>

/**
 * Default Constructor
*/
Overlay::Overlay( ){
    shape = Rect();
    color = Qt::black;
}

/**
 * Parameterized Constructor
*/
Overlay::Overlay( const Rect& poly, const QColor& color ){

    this->shape = poly;
    this->color = color;
}

