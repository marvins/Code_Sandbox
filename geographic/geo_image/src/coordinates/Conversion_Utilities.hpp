#ifndef __SRC_COORDINATES_CONVERSION_UTILITIES_HPP__
#define __SRC_COORDINATES_CONVERSION_UTILITIES_HPP__

#ifdef DEPLOYED
#include <geoimage/coordinates/CoordinateLatLon.hpp>
#include <geoimage/coordinates/CoordinateUTM.hpp>
#else
#include "CoordinateLatLon.hpp"
#include "CoordinateUTM.hpp"
#endif

namespace GEO{

int compute_utm_zone( CoordinateLatLon const& coord, bool& isNorth );

CoordinateLatLon convert_coordinate( CoordinateUTM    const& coord );
CoordinateUTM    convert_coordinate( CoordinateLatLon const& coord );


} // End of GEO Namespace

#endif

