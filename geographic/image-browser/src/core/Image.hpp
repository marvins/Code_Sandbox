/**
 * @file   Image.hpp
 * @author Marvin Smith
 * @date   10/1/2013
*/
#ifndef __SRC_CORE_IMAGE_HPP__
#define __SRC_CORE_IMAGE_HPP__

#include <src/core/GDALLoader.hpp>

#include <src/core/Point.hpp>
#include <src/core/Rect.hpp>

#include <string>

/**
 * @class Image
*/
class Image{

    public:
        
        /**
         * Default Constructor
        */
        Image( const std::string& filename );
    
        /**
         * Return the basename of the image
        */
        std::string basename()const;
        
        /**
         * Return the canonical name for the image
        */
        std::string canonical()const;

        /**
         * Return the format as a formal string
        */
        std::string formatShortString()const;
        std::string formatLongString()const;
        
        /**
         * Return the bounding box of the image
        */
        Rect getBBox()const;

    private:
        
        /// Filename
        std::string filename;
        
        /// format short name
        std::string short_name;

        /// format long name
        std::string long_name;

        /// Bounding box
        Rect bbox;
};

#endif
