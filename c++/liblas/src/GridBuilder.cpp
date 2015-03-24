#include "GridBuilder.hpp"

GridBuilder::GridBuilder( PointExtractor& point_extractor,
                          cv::Size const& grid_size,
                          cv::Rect_<double>const& grid_bounds)
  : m_point_extractor(point_extractor),
    m_grid_size(grid_size),
    m_grid_bounds(grid_bounds)
{
    
    // Create the markers
    m_grid_markers.resize(grid_size.width * grid_size.height );

    // Iterate over the points

}


