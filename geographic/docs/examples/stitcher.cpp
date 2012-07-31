#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main( ){

    cv::Mat upper = imread("upper.png");
    cv::Mat lower = imread("lower.png");

    cv::Mat pano( upper.rows+lower.rows, upper.cols, CV_8UC3 );
    
    for( size_t i=0; i<upper.cols; i++ )
    for( size_t j=0; j<upper.rows; j++ )
        pano.at<Vec3b>(j,i) = upper.at<Vec3b>(j,i);

    for( size_t i=0; i<lower.cols; i++ )
    for( size_t j=0; j<lower.rows; j++ )
        pano.at<Vec3b>(j+upper.rows,i) = lower.at<Vec3b>(j,i);
    
    Point pointupL(          0, upper.rows-8);
    Point pointupR( upper.cols, upper.rows-8);
    Point pointdnL(          0, upper.rows+8);
    Point pointdnR( upper.cols, upper.rows+8);

    //line( pano, pointupL, pointupR, Scalar(0,0,255), 2 );
    //line( pano, pointdnL, pointdnR, Scalar(0,0,255), 2 );

    imwrite("pano.png", pano);

    return 0;
}

