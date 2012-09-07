#ifndef __SRC_ORTHO_ORTHORECTIFY_HPP__
#define __SRC_ORTHO_ORTHORECTIFY_HPP__

#include <opencv2/core/core.hpp>

#include "../core/Configuration.hpp"

cv::Mat orthorectify( cv::Mat const& image, Options& options );

#endif
