//
// Created by Marvin Smith on 1/1/18.
//

#include "OpenCL_Convert_01.hpp"


/********************************/
/*          Constructor         */
/********************************/
OpenCL_Converter_Config_01::OpenCL_Converter_Config_01()
  : Coordinate_Converter_Config_Base(),
    m_class_name("OpenCL_Converter_Config_01")
{
}

/********************************/
/*          Constructor         */
/********************************/
OpenCL_Converter_01::OpenCL_Converter_01(OpenCL_Converter_Config_01::ptr_t config)
 : Coordinate_Converter_Base(config),
   m_class_name("OpenCL_Converter_01")
{
    m_config = std::dynamic_pointer_cast<OpenCL_Converter_Config_01>(config);
}
