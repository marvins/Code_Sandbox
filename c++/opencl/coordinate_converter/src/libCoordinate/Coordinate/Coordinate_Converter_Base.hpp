//
// Created by Marvin Smith on 1/1/18.
//

#ifndef OPENCL_COORDINATE_CONVERTER_COORDINATE_CONVERTER_BASE_HPP
#define OPENCL_COORDINATE_CONVERTER_COORDINATE_CONVERTER_BASE_HPP

// C++ Libraries
#include <memory>
#include <string>

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


    private:

        /// Class Name
        std::string m_class_name;

        /// Internal Configuration
        Coordinate_Converter_Config_Base::ptr_t m_config;
};


#endif //OPENCL_COORDINATE_CONVERTER_COORDINATE_CONVERTER_BASE_HPP
