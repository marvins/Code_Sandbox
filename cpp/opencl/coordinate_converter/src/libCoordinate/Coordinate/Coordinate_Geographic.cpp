//
// Created by Marvin Smith on 1/1/18.
//

#include "Coordinate_Geographic.hpp"


/********************************/
/*          Constructor         */
/********************************/
Coordinate_Geographic::Coordinate_Geographic(const DatumType &datum)
  : Coordinate_Base(datum),
    m_longitude_degrees(0),
    m_latitude_degrees(0)
{
}

/********************************/
/*          Constructor         */
/********************************/
Coordinate_Geographic::Coordinate_Geographic( const double&     longitude_degrees,
                                              const double&     latitude_degrees,
                                              const DatumType&  datum)
  : Coordinate_Base(datum),
    m_longitude_degrees(longitude_degrees),
    m_latitude_degrees(latitude_degrees)
{
}
