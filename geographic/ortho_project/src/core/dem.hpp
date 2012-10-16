#ifndef __SRC_CORE_DEM_HPP__
#define __SRC_CORE_DEM_HPP__

#include "Configuration.hpp"
#include "Utilities.hpp"

#include <opencv2/core/core.hpp>

void dem_correction( cv::Mat& world_position, const double& gsd, Options& options, cv::Mat const& minPnt, cv::Mat const& maxPnt );

double query_dem( cv::Mat const& pnt, const cv::Mat& stPnt, Options& options );
double query_dem( cv::Point2f const& pnt, const cv::Point2f& stPnt, Options& options );
double query_dem( cv::Point2f const& pnt, Options& options );
double query_dem( cv::Point3f const& pnt, cv::Point3f const& stPnt, Options& options );
double query_dem( cv::Point3f const& pnt, Options& options );

double query_dem( cv::Point2f const& pnt, cv::Point2f const& minP, cv::Point2f const& maxP, cv::Mat const& );

double query_max_elevation( const cv::Mat& minPnt,     const cv::Mat& maxPnt,     const cv::Mat& starepoint,     const Options& options );
double query_max_elevation( const cv::Point2f& minPnt, const cv::Point2f& maxPnt, const cv::Point2f& starepoint, const Options& options );

double query_max_elevation( const cv::Mat& dem );

#endif
