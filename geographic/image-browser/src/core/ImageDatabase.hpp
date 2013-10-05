/**
 * @file   ImageDatabase.hpp
 * @author Marvin Smith
 * @date   10/1/2013
*/
#ifndef __SRC_CORE_IMAGEDATABASE_HPP__
#define __SRC_CORE_IMAGEDATABASE_HPP__

#include <src/core/Image.hpp>
#include <src/core/Rect.hpp>

#include <string>
#include <vector>

/**
 * @class ImageDatabase
*/
class ImageDatabase{

    public:

        /**
         * Default Constructor
        */
        ImageDatabase();

        /**
         * Clear the database
        */
        void clear();
        
        /**
         * Return the size of the database
        */
        size_t size()const;

        /**
         * Add an image
        */
        void addItem( const std::string& filename );
        
        /**
         * Return a pointer to the image
        */
        Image& operator[]( const int& idx );

        /**
         * Return a copy of the image
        */
        Image operator[]( const int& idx )const;

        /**
         * Return the index of the image found
        */
        int findImage( const std::string& canonical_name );

    private:
        
        /// Image List
        std::vector<Image> data;
};

#endif
