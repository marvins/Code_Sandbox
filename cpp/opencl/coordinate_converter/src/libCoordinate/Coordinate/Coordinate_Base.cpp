/**
 * @file    Coordinate_Base.cpp
 * @author  Marvin Smith
 * @date    1/1/2017
 */
#include "Coordinate_Base.hpp"


/********************************/
/*          Constructor         */
/********************************/
Coordinate_Base::Coordinate_Base(const DatumType &datum)
 : m_class_name("Coordinate_Base"),
   m_datum(datum)
{
}
