/**
 * @file    Geo_Options.hpp
 * @author  Marvin Smith
 * @date    3/2/2018
 */
#ifndef GEO_IMAGE_BROWSER_GEO_OPTIONS_HPP
#define GEO_IMAGE_BROWSER_GEO_OPTIONS_HPP


// C++ Libraries
#include <string>


/**
 * @class Geo_Options
 */
class Geo_Options
{
    public:

        /**
         * @brief Constructor
         */
        Geo_Options( int argc, char* argv[] );


    private:

        /// Class Name
        std::string m_class_name;

        /// Configuration File


}; // End of Geo_Options Class


#endif