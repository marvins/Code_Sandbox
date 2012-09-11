#include "Utilities.hpp"

#include <iostream>

using namespace cv;
using namespace std;

/** 
  * Loads an OpenCV 3D point into a matrix
*/
Mat load_point( const Point3f& pnt ){
    
    Mat out(4, 1, CV_64FC1);
    out.at<double>(0,0) = pnt.x;
    out.at<double>(1,0) = pnt.y;
    out.at<double>(2,0) = pnt.z;
    out.at<double>(3,0) = 1;
    return out;
}

/**
  * Loads a 3d point into a matrix
*/
Mat load_point( double const& x, double const& y, double const& z ){

    Mat out(4, 1, CV_64FC1);
    out.at<double>(0,0) = x;
    out.at<double>(1,0) = y;
    out.at<double>(2,0) = z;
    out.at<double>(3,0) = 1;

    return out;
}

/**
 * Loads a 3D Vector into a matrix
*/
Mat load_vector( double const& x, double const& y, double const& z ){
    
    Mat out(4, 1, CV_64FC1);
    out.at<double>(0,0) = x;
    out.at<double>(1,0) = y;
    out.at<double>(2,0) = z;
    out.at<double>(3,0) = 0;

    return out;
}

/**
 * Print a matrix to the console
*/
void print_mat( const Mat& mat ){

    for( int y=0; y<mat.rows; y++){
        for( int x=0; x<mat.cols; x++){
            if(mat.type() == CV_64FC1)
                cout << mat.at<double>(y,x) << ", ";
        }
        cout << endl;
    }
}

/**
 * Round a value
*/
int _round( double const& value ){
    return std::floor( value + 0.5 );
}


cv::Point3f Mat2Point3f( Mat const& mat ){

    return Point3f( 
        mat.at<double>(0,0),
        mat.at<double>(1,0),
        mat.at<double>(2,0));

}

