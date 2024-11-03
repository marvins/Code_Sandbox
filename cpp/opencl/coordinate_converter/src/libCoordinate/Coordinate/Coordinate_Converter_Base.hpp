//
// Created by Marvin Smith on 1/1/18.
//

#ifndef OPENCL_COORDINATE_CONVERTER_COORDINATE_CONVERTER_BASE_HPP
#define OPENCL_COORDINATE_CONVERTER_COORDINATE_CONVERTER_BASE_HPP

// C++ Libraries
#include <memory>
#include <string>
#include <vector>

// Project Libraries
#include "Coordinate_Geographic.hpp"
#include "Coordinate_UTM.hpp"


/**
 * @class Coordinate_Converter_Config_Base
 */
class Coordinate_Converter_Config_Base
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<Coordinate_Converter_Config_Base> ptr_t;


        /**
         * @brief Constructor
         */
        Coordinate_Converter_Config_Base();
};

/**
 * @class Coordinate_Converter_Base
 */
class Coordinate_Converter_Base
{
    public:

        /**
         * @brief Constructor
         */
        Coordinate_Converter_Base( const Coordinate_Converter_Config_Base::ptr_t config );


        /**
         * @brief Convert Coordinate
         */
        virtual Coordinate_UTM  Convert_To_UTM( const Coordinate_Geographic& coordinate ) = 0;


        /**
         * @brief Convert multiple coordinates at once
         * @param coordinates
         * @return
         */
        virtual std::vector<Coordinate_UTM> Convert_To_UTM( const std::vector<Coordinate_Geographic>& coordinates ) = 0;


    private:

        /// Class Name
        std::string m_class_name;

        /// Internal Configuration
        Coordinate_Converter_Config_Base::ptr_t m_config;
};


#endif //OPENCL_COORDINATE_CONVERTER_COORDINATE_CONVERTER_BASE_HPP
