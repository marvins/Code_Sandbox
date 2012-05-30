#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <opencv2/stitching/stitcher.hpp>

using namespace cv;

int main( ){

   vector<Mat> inputImg;
   vector<Mat> outputImg;

   inputImg.push_back(imread("outdoor01.jpg",0));
   inputImg.push_back(imread("outdoor02.jpg",0));
   inputImg.push_back(imread("outdoor03.jpg",0));
   inputImg.push_back(imread("outdoor04.jpg",0));
   inputImg.push_back(imread("outdoor05.jpg",0));
   Stitcher pano;

   pano.composePanorama( inputImg, outputImg );

   return 0;
}
