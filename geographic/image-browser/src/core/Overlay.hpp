/**
 * @file    Overlay.hpp
 * @author  Marvin Smith
 * @date    10/2/2013
*/
#ifndef __SRC_CORE_OVERLAY_HPP__
#define __SRC_CORE_OVERLAY_HPP__

#include <QColor>

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

        /// Shape to view
        Rect  shape;

        /// Color to present it as
        QColor color;

};

#endif
