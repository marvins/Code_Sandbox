#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char* argv[] ){

   //read image in grayscale
   Mat image = imread(argv[1], 0);

   /*   input arguements for both
         - input  image  (Mat)
         - output image  (Mat)
         - output size   (Size) -defaults to expected size
   */
   //apply pyrUp upsampling
   Mat up_img;
   pyrUp( image, up_img );
   imwrite( argv[2], up_img);

   //apply pyrDown downsampling
   Mat down_img;
   pyrDown( image, down_img );
   imwrite( argv[3], down_img);

   return 0;
}
