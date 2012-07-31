#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main( ){

    //read images from file
    cv::Mat nw = imread("nw.png");
    cv::Mat ne = imread("ne.png");
    cv::Mat sw = imread("sw.png");
    cv::Mat se = imread("se.png");

    //create pano
    int width  = nw.cols + ne.cols;
    int height = nw.rows + sw.rows;
    cv::Mat pano( height, width, CV_8UC3 );
    
    //load images into mat
    for( size_t i=0; i<nw.cols; i++ )
    for( size_t j=0; j<nw.rows; j++ )
        pano.at<Vec3b>(j,i) = nw.at<Vec3b>(j,i);

    for( size_t i=0; i<ne.cols; i++ )
    for( size_t j=0; j<ne.rows; j++ )
        pano.at<Vec3b>(j,i+nw.cols) = ne.at<Vec3b>(j,i);

    for( size_t i=0; i<sw.cols; i++ )
    for( size_t j=0; j<sw.rows; j++ )
        pano.at<Vec3b>(j+nw.rows,i) = sw.at<Vec3b>(j,i);

    for( size_t i=0; i<se.cols; i++ )
    for( size_t j=0; j<se.rows; j++ )
        pano.at<Vec3b>(j+nw.rows,i+nw.cols) = se.at<Vec3b>(j,i);


    imwrite("pano.png", pano);

    return 0;
}

