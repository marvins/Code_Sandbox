/**
 * @file    Overlay.hpp
 * @author  Marvin Smith
 * @date    10/2/2013
*/
#ifndef __SRC_CORE_OVERLAY_HPP__
#define __SRC_CORE_OVERLAY_HPP__

#include <QColor>

#include <string>

#include <src/core/Rect.hpp>

/**
 * @class Overlay
*/
class Overlay{
    
    public:
    
        /**
         * Default Constructor
        */
        Overlay();

        /**
         * Parameterized Constructor
        */
        Overlay( Rect const& poly, QColor const& color );
        
        std::string toGoogleMapsString( std::string& varname, const int& idx = 0)const;

        /// Shape to view
        Rect  shape;

        /// Color to present it as
        QColor color;

};

#endif
