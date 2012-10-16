#ifndef __SRC_ORTHO_ORTHORECTIFY_HPP__
#define __SRC_ORTHO_ORTHORECTIFY_HPP__

#include <opencv2/core/core.hpp>

#include "../core/Configuration.hpp"

#include <utility>

using namespace cv;
using namespace std;

Point convert_world2pixel( Mat const& world_coordinate, 
                           Mat const& camera_position,
                           Mat const& camera_principle_point,
                           Mat const& camera_normal,
                           Mat const& camera_rotation,
                           Size_<double> focal_plane_size,
                           Size image_size );

Mat  convert_pixel2world(  Mat const&       pixel_coord, 
                           Size const&      image_size, 
                           Size_<double>    image_plane_size, 
                           Mat const&       rotation_axis,
                           double const&    focal_length,
                           Mat const&       camera_position,
                           Mat const&       earth_normal      );

pair<double,double> compute_gsd( Mat const& tl, Mat const& tr, Mat const& bl, Mat const& br,  
                                 Size const& image_size, Mat const& rotation_axis );


cv::Mat orthorectify( cv::Mat const& image, Options& options );

#endif
