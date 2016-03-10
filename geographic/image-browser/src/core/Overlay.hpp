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
        

        /**
         * @brief Print the Overlay as a Google Maps String
        */
        std::string toGoogleMapsString( std::string& varname, const int& idx = 0)const;
        

        /**
         * @brief Print the Overlay as a Log String
        */
        std::string To_Log_String()const;

        /// Shape to view
        Rect  shape;

        /// Color to present it as
        QColor color;

};

#endif
