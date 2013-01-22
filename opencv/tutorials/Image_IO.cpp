#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;

int main( int argc, char* argv[] ){

   //read image in color
   Mat color_img = imread( argv[1] );

   //read image in grayscale, NOTE 0 at end
   Mat gray_img  = imread( argv[1], 0 );

   //create display windows
   namedWindow("Fixed Ratio");   //window size is fixed
   namedWindow("Free Ratio", 0); //window may be resized

   //display images
   imshow("Fixed Ratio", color_img);
   imshow("Free Ratio", gray_img);

   //hold program for keyboard input
   waitKey(0);

   //write gray image to file
   imwrite( argv[2], gray_img);

   return 0;
}
