#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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
