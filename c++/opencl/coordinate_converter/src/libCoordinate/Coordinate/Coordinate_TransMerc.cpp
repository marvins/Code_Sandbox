/**
 * @file    Coordinate_TransMerc.cpp
 * @author  Marvin Smith
 * @date    1/13/2017
 */
#include "Coordinate_TransMerc.hpp"


/********************************/
/*          Constructor         */
/********************************/
Coordinate_TransMerc::Coordinate_TransMerc(const DatumType &datum)
{
}

/********************************/
/*          Constructor         */
/********************************/
Coordinate_TransMerc::Coordinate_TransMerc( const double&     easting,
                                            const double&     northing,
                                            const DatumType&  datum)
  : Coordinate_Base(datum),
    m_easting(easting),
    m_northing(northing)
{
}
