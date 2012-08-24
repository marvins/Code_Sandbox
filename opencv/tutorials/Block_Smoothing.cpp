/**
 * Demonstration of simple block smoothing
*/
#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char* argv[] ){

   //read an image in grayscale
   Mat image = imread( argv[1], 0);

   //blur image
   /*  inputs:
      input image    (Mat)
      output image   (Mat)
      kernel size    (Size)
      anchor point   (Point)
            Note:(-1,-1) automatically choses center
      borderType     (int) <- see different descriptiosn
   */
   blur( image, image, Size(5,5), Point(-1,-1), BORDER_CONSTANT);

   //write image to file
   imwrite(argv[2], image);

   return 0;
}
