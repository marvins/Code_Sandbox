//
// Created by Marvin Smith on 1/1/18.
//

#include "Coordinate_UTM.hpp"


/********************************/
/*          Constructor         */
/********************************/
Coordinate_UTM::Coordinate_UTM()
 : Coordinate_TransMerc(DatumType::WGS84),
   m_is_northern(true)
{
}


/********************************/
/*          Constructor         */
/********************************/
Coordinate_UTM::Coordinate_UTM(const int32_t&  grid_zone,
                               const bool&     is_northern,
                               const double&   easting,
                               const double&   northing)
  : Coordinate_TransMerc( easting,
                          northing,
                          DatumType::WGS84),
    m_grid_zone(grid_zone),
    m_is_northern(is_northern)
{
}
