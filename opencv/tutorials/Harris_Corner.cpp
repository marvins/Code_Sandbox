/******************************/
/*   Harris Corner Detector   */
/******************************/
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <fstream>
#include <iomanip>
#include <iostream>

using namespace cv;
using namespace std;

/**************************************/
/*   Compute Harris Transformation    */
/**************************************/
void Harris_Transform( const Mat& img, Mat& corners, double sigmaD, double sigmaI, double alpha ){

   //create sobel differentiation masks
   Mat sobelX(3,3,CV_32FC1,Scalar(0)), sobelY;
   sobelX.at<float>(0,0) = -1.0;
   sobelX.at<float>(1,0) = -2.0;
   sobelX.at<float>(2,0) = -1.0;
   sobelX.at<float>(0,2) = 1.0;
   sobelX.at<float>(1,2) = 2.0;
   sobelX.at<float>(2,2) = 1.0;

   transpose(sobelX,sobelY);

   //create window for blurring
   int win = 5*sigmaD;
   if(win % 2 == 0)
      win++;

   //blur image before differentiation
   Mat blurred;
   GaussianBlur(img,blurred,Size(win,win),sigmaD);

   // apply Sobel (differentiation)
   Mat imgX, imgY, imgXX, imgYY, imgXY;
   filter2D(blurred,imgX,CV_32FC1,sobelX);
   filter2D(blurred,imgY,CV_32FC1,sobelY);

   // multiply then blur (harris_sigmaI)
   win = 5*sigmaI;
   if ( win % 2 == 0 )
      win++;

   GaussianBlur( imgX.mul(imgY), imgXY, Size(win,win), sigmaI);
   GaussianBlur( imgX.mul(imgX), imgXX, Size(win,win), sigmaI);
   GaussianBlur( imgY.mul(imgY), imgYY, Size(win,win), sigmaI);

   // create cornerness image
   corners = Mat(img.size(), CV_32FC1);

   // calculate cornerness values
   Mat A(2,2,CV_32FC1);
   for ( int r = 0; r < img.rows; r++ )
      for ( int c = 0; c < img.cols; c++ )
      {
         A.at<float>(0,0) = imgXX.at<float>(r,c);
         A.at<float>(1,1) = imgYY.at<float>(r,c);
         A.at<float>(1,0) = imgXY.at<float>(r,c);
         A.at<float>(0,1) = imgXY.at<float>(r,c);
         corners.at<float>(r,c) = determinant(A) - alpha * pow(trace(A)[0],2);
      }
}

/*******************/
/*   Main Driver   */
/*******************/
int main( int argc, char* argv[] ){

   //load image in grayscale
   Mat image = imread( argv[1], 0);

   //convert image to floating point
   Mat fimage;
   image.convertTo(fimage, CV_32FC1);

   //apply Harris corner detector
   Mat corners;
   Harris_Transform( fimage, corners, 0.5, 2.5, 0.01);

   //normalize results
   normalize( corners, corners,  0, 1, CV_MINMAX);   

   //threshold image
   threshold( corners, corners, .7, 1, THRESH_BINARY);

   //draw results onto output image
   Mat output1;
   cvtColor( image, output1, CV_GRAY2BGR);
   for(int i=0; i<image.cols; i++)
      for(int j=0; j<image.rows; j++)
         if( fabs( corners.at<float>(j,i) -1) < 0.01)
            output1.at<Vec3b>(j,i) = Vec3b(0,255,0);

   //write results
   imwrite( argv[2], output1); 

   return 0;
}
