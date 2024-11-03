//
// Created by Marvin Smith on 1/1/18.
//

#include "Coordinate_Converter_Base.hpp"


/********************************/
/*          Constructor         */
/********************************/
Coordinate_Converter_Config_Base::Coordinate_Converter_Config_Base()
{
}

/********************************/
/*          Constructor         */
/********************************/
Coordinate_Converter_Base::Coordinate_Converter_Base(const Coordinate_Converter_Config_Base::ptr_t config)
  : m_class_name("Coordinate_Converter_Base"),
    m_config(config)
{
}
