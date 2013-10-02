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
         * Return the format as a formal string
        */
        std::string formatShortString()const;
        std::string formatLongString()const;

    private:
        
        /// Filename
        std::string filename;

        /// GDAL Driver
        GDALLoader* driver;

};

#endif
