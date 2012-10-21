/**
 *
 * A set of core functions for the evaulation and bundling of imagery.
 *
*/
#ifndef __SRC_CORE_HPP__
#define __SRC_CORE_HPP__

#include "Camera.hpp"
#include "FilePtr.hpp"
#include "Metrics.hpp"
#include "Options.hpp"


/**
 * Compute the image bundles
*/
deque<ImageBundle> compute_image_bundles( deque<Camera>& cameras, Options const& options );


/**
 * Evaluate the frame sets for the cameras and report 
 * any required metrics.
*/
Metrics evaluate_frame_sets( deque<Camera>& cameras, Options& options, FilePtr& newest_file );


#endif

