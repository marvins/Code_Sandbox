/**
 *
 * A set of core functions for the evaulation and bundling of imagery.
 *
*/
#ifndef __SRC_CORE_HPP__
#define __SRC_CORE_HPP__

#include "Options.hpp"
#include "Camera.hpp"
#include "Metrics.hpp"

/**
 * Evaluate the frame sets for the cameras and report 
 * any required metrics.
*/
Metrics evaluate_frame_sets( deque<Camera>& cameras, Options& options );


#endif

