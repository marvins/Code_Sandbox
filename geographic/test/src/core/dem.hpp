#ifndef __SRC_CORE_DEM_HPP__
#define __SRC_CORE_DEM_HPP__

#include "Configuration.hpp"

#include <opencv2/core/core.hpp>

double query_dem( cv::Point3f const& pnt, Options& options );

#endif
