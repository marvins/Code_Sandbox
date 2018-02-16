//
// Created by Marvin Smith on 1/1/18.
//

#ifndef OPENCL_COORDINATE_CONVERTER_OPENCL_CONVERT_02_HPP
#define OPENCL_COORDINATE_CONVERTER_OPENCL_CONVERT_02_HPP


// Coordinate Libraries
#include <Coordinate.hpp>

// Project Libraries
#include "OpenCL_Utilities.hpp"


class OpenCL_Converter_Config_02 : public Coordinate_Converter_Config_Base
{
    public:

        typedef std::shared_ptr<OpenCL_Converter_Config_02> ptr_t;

        /**
         * @brief Constructor
         */
        OpenCL_Converter_Config_02( const cl_device_type& device_type = CL_DEVICE_TYPE_ALL );


        /**
         * @brief Get the OpenCL Device Type
         */
        cl_device_type Get_OCL_Device_Type()const{
            return m_ocl_device_type;
        }


    private:

        /// Class Name
        std::string m_class_name;

        /// Preferred OpenCL Device Type
        cl_device_type m_ocl_device_type;
};

class OpenCL_Converter_02 : public Coordinate_Converter_Base
{
    public:

        /**
         * @brief Constructor
         */
        OpenCL_Converter_02( OpenCL_Converter_Config_02::ptr_t config );


        /**
         * @brief Destructor
         */
        virtual ~OpenCL_Converter_02();


        /**
         * @brief Convert a Geographic Coordinate to UTM
         * @param coordinate
         * @return
         */
        Coordinate_UTM  Convert_To_UTM( const Coordinate_Geographic& coordinate );


        std::vector<Coordinate_UTM> Convert_To_UTM( const std::vector<Coordinate_Geographic>& coordinates );

    private:

        void Initialize_OpenCL();


        void Finalize_OpenCL();


        /// Class Name
        std::string m_class_name;

        /// Configuration
        OpenCL_Converter_Config_02::ptr_t m_config;

        /// Chosen OpenCL Platform
        cl_platform_id m_ocl_platform;

        /// Current OpenCL Device
        cl_device_id m_ocl_device;
};


#endif //OPENCL_COORDINATE_CONVERTER_OPENCL_CONVERT_HPP
