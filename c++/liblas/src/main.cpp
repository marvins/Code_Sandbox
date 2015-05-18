
// Boost Libraries
#include <boost/filesystem.hpp>

// OpenCV Libraries
#include <opencv2/core/core.hpp>

// C++ Standard Libraries
#include <iostream>

// Las Libraries
#include "GridBuilder.hpp"
#include "PointExtractor.hpp"

/**
 * @brief Main Function
*/
int main( int argc, char* argv[] ){

    // Check args
    if( argc < 2 ){
        std::cerr << "Not enough arguments" << std::endl;
        return 1;
    }

    // Define the grid size
    cv::Size grid_size(5000, 5000);

    // FLag if we want to use the grid bounds
    bool use_point_bounds = true;

    // Define the grid bounds
    cv::Rect_<double> grid_bounds(0, 0, 100, 100);

    //  Create the point extractor
    PointExtractor point_extractor((boost::filesystem::path(argv[1])));

    // Pass to the grid builder
    GridBuilder grid_builder( point_extractor, grid_size, grid_bounds, use_point_bounds );

    return 0;
}

