#ifndef __SRC_ORTHO_PERSPECTIVE_HPP__
#define __SRC_ORTHO_PERSPECTIVE_HPP__

#include <opencv2/core/core.hpp>

#include "../core/Configuration.hpp"

cv::Mat generate_perspective_test_image( Options& options );

void create_flat_test_image( Options const& options, cv::Mat& image, cv::Mat& dem );

void rotate_image_scene( cv::Mat const& input_image, cv::Mat const& dem_image, cv::Mat& output_image, Options const& options );

#endif
