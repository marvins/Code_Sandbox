/**
 * @file   GDALLoader.hpp
 * @author Marvin Smith
 * @date   10/1/2013
*/
#ifndef __SRC_CORE_GDALLOADER_HPP__
#define __SRC_CORE_GDALLOADER_HPP__

#include <string>

#include <cpl_conv.h>
#include <gdal_priv.h>

#include <src/core/Point.hpp>
#include <src/core/Rect.hpp>

/**
 * @class GDALLoader
*/
class GDALLoader{

    public:
        
        /**
         * Default Constructor
        */
        GDALLoader( const std::string& filename );
        
        /**
         Recover the bounding box
        */
        Rect  get_bbox()const;
        
        /**
         * Load all data
        */
        void load();
        
        /**
         * Filter all non-supported file types
        */
        static std::vector<std::string> filter( const std::vector<std::string> const& input );
        
        /**
         * Return the short name
        */
        std::string getShortName()const;
        
        /**
         * Return the long name
        */
        std::string getLongName()const;
    
    private:
        
        /// Filename
        std::string filename;
        
        /// Dataset
        GDALDataset* dataset;

        /// Driver
        GDALDriver*  driver;

};

#endif

