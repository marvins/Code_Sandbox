//
// Created by Marvin Smith on 1/1/18.
//

#ifndef OPENCL_COORDINATE_CONVERTER_OPENCL_CONVERT_HPP
#define OPENCL_COORDINATE_CONVERTER_OPENCL_CONVERT_HPP


// Coordinate Libraries
#include <Coordinate.hpp>


class OpenCL_Converter_Config_01 : public Coordinate_Converter_Config_Base
{
    public:

        typedef std::shared_ptr<OpenCL_Converter_Config_01> ptr_t;

        /**
         * @brief Constructor
         */
        OpenCL_Converter_Config_01();

    private:

        /// Class Name
        std::string m_class_name;
};

class OpenCL_Converter_01 : public Coordinate_Converter_Base
{
    public:

        /**
         * @brief Constructor
         */
        OpenCL_Converter_01( OpenCL_Converter_Config_01::ptr_t config );


        /**
         * @brief Destructor
         */
        virtual ~OpenCL_Converter_01();


        /**
         * @brief Convert a Geographic Coordinate to UTM
         * @param coordinate
         * @return
         */
        Coordinate_UTM  Convert_To_UTM( const Coordinate_Geographic& coordinate );


        std::vector<Coordinate_UTM> Convert_To_UTM( const std::vector<Coordinate_Geographic>& coordinates );

    private:

        /// Class Name
        std::string m_class_name;

        /// Configuration
        OpenCL_Converter_Config_01::ptr_t m_config;
};


#endif //OPENCL_COORDINATE_CONVERTER_OPENCL_CONVERT_HPP
