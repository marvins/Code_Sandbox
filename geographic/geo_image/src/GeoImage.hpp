/**
 * \include GeoImage.md
*/
#ifndef __GEOIMAGE_CORE_H__
#define __GEOIMAGE_CORE_H__

#define DEPLOYED

// version file
#include <geoimage/core/GeoImage_Version.hpp>

//COORDINATE Libraries
#include <geoimage/coordinates/Datum.hpp>
#include <geoimage/coordinates/CoordinateLatLon.hpp>
#include <geoimage/coordinates/CoordinateUTM.hpp>
#include <geoimage/coordinates/Conversion_Utilities.hpp>
#include <geoimage/coordinates/Coordinate_Math.hpp>

//CORE Libraries
#include <geoimage/core/Enumerations.hpp>

//DEM Libraries
#include <geoimage/dem/DEM.hpp>

//EXTENSION Libraries
#include <geoimage/extensions/GS_ICD_Reference.hpp>
#include <geoimage/extensions/GS2_Header.hpp>
#include <geoimage/extensions/TACID.hpp>

//IMAGE Libraries
#include <geoimage/image/GeoImage.hpp>
#include <geoimage/image/GeoHeader.hpp>

//IO Libraries
#include <geoimage/io/GeoLoader.hpp>
#include <geoimage/io/GDALLoader.hpp>
#include <geoimage/io/IO_Configuration.hpp>

//UTILITY Libraries
#include <geoimage/utilities/File_Utilities.hpp>
#include <geoimage/utilities/OpenCV_Utilities.hpp>
#include <geoimage/utilities/String_Utilities.hpp>

#endif

