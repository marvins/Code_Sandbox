#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char* argv[]){

   //image should be grayscale
   Mat image = imread( argv[1], 0);

   //blur image
   /*  Arguements
      - input image  (Mat)
      - output image (Mat)
      - kernel size  (Size)
      - sigma x      (double)
      - sigma y      (double) 
      - enum BORDER TYPE   
   */
   GaussianBlur( image, image, Size(11,11), 2);

   imwrite( argv[2], image );

   return 0;
}
