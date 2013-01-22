/******************************/
/*   Harris Corner Detector   */
/******************************/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

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
   cornerHarris( fimage, corners, 5, 3, 0.19 );
   
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
   imwrite(argv[2],output1);

   return 0;
}
