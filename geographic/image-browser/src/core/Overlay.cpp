/** 
 * @file    Overlay.cpp
 * @author  Marvin Smith
 * @date    10/2/2013
*/
#include <src/core/Overlay.hpp>

#include <src/core/StringUtilities.hpp>

// C++ Libraries
#include <sstream>

using namespace std;

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


/*****************************************************************/
/*          Print the Overlay as a Google Maps String            */
/*****************************************************************/
string Overlay::toGoogleMapsString( string& varname, const int& idx)const
{

    varname = string("rectangle") + num2str(idx);

    string var = string("var rectangle") + num2str(idx) 
               + string(" = new google.maps.Rectangle({ ")
               + string("strokeColor: '#FF0000',") 
               + string("strokeOpacity: 0.8, ")
               + string("strokeWeight: 2, ")
               + string("fillColor: '#FF0000', ")
               + string("fillOpacity: 0.35, ")
               + string("map: map, ")
               + string("bounds: new google.maps.LatLngBounds(")
               + string("new google.maps.LatLng(") + num2str(shape.tl.y) + string(", ") + num2str(shape.tl.x) + string("),")
               + string("new google.maps.LatLng(") + num2str(shape.br.y) + string(", ") + num2str(shape.br.x) + string("))")
               + string("});");
    return var;
}


/******************************************************/
/*          Print the Overlay as a Log String         */
/******************************************************/
std::string Overlay::To_Log_String()const
{
    // Create stringstream
    std::stringstream sin;

    sin << "Overlay:  Rectangle: " << shape;
    return sin.str();
}

