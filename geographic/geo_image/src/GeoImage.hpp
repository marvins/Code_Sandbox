/**
 * \include GeoImage.md
*/
#ifndef __GEOIMAGE_CORE_H__
#define __GEOIMAGE_CORE_H__

#define DEPLOYED

// version file
#include <geoimage/core/GeoImage_Version.hpp>

//IMAGE Libraries
#include <geoimage/image/GeoImage.hpp>

//IO Libraries
#include <geoimage/io/GeoLoader.hpp>
#include <geoimage/io/GDALLoader.hpp>
#include <geoimage/io/IO_Configuration.hpp>

#ifdef __NOTHING__
#include <geoimage/TACID.hpp>
#include <geoimage/DEM.h>
#include <geoimage/Enumerations.h>
#include <geoimage/GeoImage.h>
#include <geoimage/GeoHeader.h>
#include <geoimage/GDAL_Data.h>
#include <geoimage/NITFHeader.h>
#include <geoimage/PixelType.h>
#include <geoimage/OpenCVUtils.h>
#include <geoimage/GDAL2OpenCV.h>
#include <geoimage/DTEDUtils.h>
#include <geoimage/GS2_Header.hpp>
#endif

#endif

