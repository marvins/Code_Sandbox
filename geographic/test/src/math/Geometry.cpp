#include "Geometry.hpp"

#include "../core/Utilities.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat compute_plane_line_intersection( Mat const& P1, Mat const& P2, Mat const& N, Mat const& P3){

    
    double u = N.dot(P3-P1)/N.dot(P2-P1);
    
    Mat output = (P1 + u*(P2-P1));

    return output;
}

Mat compute_image2projected_coordinate( const int& xx, const int& yy ){



    return load_point( 0, 0, 0);
}

