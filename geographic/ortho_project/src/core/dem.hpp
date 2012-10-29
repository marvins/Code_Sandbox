#ifndef __SRC_CORE_DEM_HPP__
#define __SRC_CORE_DEM_HPP__

#include "Configuration.hpp"
#include "Utilities.hpp"

#include <opencv2/core/core.hpp>

void dem_correction( cv::Mat& world_position, const double& gsd, Options& options, cv::Mat const& minPnt, cv::Mat const& maxPnt );

#endif
