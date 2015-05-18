#include "GridBuilder.hpp"

GridBuilder::GridBuilder( PointExtractor& point_extractor,
                          cv::Size const& grid_size,
                          cv::Rect_<double>const& grid_bounds,
                          bool use_point_bounds )
  : m_point_extractor(point_extractor),
    m_grid_size(grid_size),
    m_grid_bounds(grid_bounds)
{
    // Update the boudns if using point bounds
    if( use_point_bounds == true ){
        m_grid_bounds.x = point_extractor.Get_Min_Point().x;
        m_grid_bounds.y = point_extractor.Get_Min_Point().y;
        m_grid_bounds.width  = point_extractor.Get_Max_Point().x - point_extractor.Get_Min_Point().x; 
        m_grid_bounds.height = point_extractor.Get_Max_Point().y - point_extractor.Get_Min_Point().y; 
    }

    // misc variables
    int current_row, current_col;
    double x_ratio, y_ratio;

    // Create the markers
    m_grid_markers.resize(m_grid_size.width * m_grid_size.height );

    // Iterate over the points
    cv::Point3d current_point;
    while( point_extractor.Get_Number_Remaining_Points() > 0 ){

        // Get next point
        current_point = point_extractor.Pop_Next();

        // Compute the grid position
        x_ratio = (current_point.x - m_grid_bounds.tl().x)/m_grid_bounds.size().width;
        y_ratio = (current_point.y - m_grid_bounds.tl().y)/m_grid_bounds.size().height;

        current_col = m_grid_size.width * x_ratio;
        current_row = m_grid_size.height * (1 - y_ratio);

        // Update the grid marker
        if( current_col >= 0 && current_col < m_grid_size.width && 
            current_row >= 0 && current_row < m_grid_size.height ){
            m_grid_markers[current_row * m_grid_size.width + current_col].Add( current_point.z );
        }
    }

    // Update the z values for the grid
    for( int i=0; i<m_grid_markers.size(); i++ ){
        m_grid_markers[i].Set_Z();
    }
}


