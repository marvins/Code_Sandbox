#ifndef __GRID_BUILDER_HPP__
#define __GRID_BUILDER_HPP__

// OpenCV Libraries
#include <opencv2/core/core.hpp>

// Our Libraries
#include "GridMarker.hpp"
#include "PointExtractor.hpp"

/**
 * @class GridBuilder
*/
class GridBuilder{

    public:
        
        /**
         * @Constructor
        */
        GridBuilder( PointExtractor& point_extractor,
                     cv::Size const& grid_size,
                     cv::Rect_<double> const& grid_bounds );


    private:
        
        /// Point Source
        PointExtractor& m_point_extractor;

        /// Matrix data
        cv::Mat m_grid;

        /// Grid Size
        cv::Size m_grid_size;

        /// Grid Bounds
        cv::Rect_<double> m_grid_bounds;

        /// Grid Container
        std::vector<GridMarker> m_grid_markers;

}; // End of GridBuilder Class


#endif
