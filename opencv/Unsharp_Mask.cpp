#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

using namespace cv;

int main( int argc, char* argv[] ){

   //some relevant variables
   double sigma    = 1;
   double alpha = 5;
   Mat gaussian, diff;
   
   int kernel_size = 5*sigma;
   Size kernel( kernel_size, kernel_size);

   //read image in color
   Mat image = imread( argv[1]);

   //convert to floating point
   Mat fimage;
   image.convertTo( fimage, CV_32FC3);

   //split into 3 channels
   vector<Mat> fchannels;
   split( fimage, fchannels);

   //iterate over each color channel
   for( size_t i=0; i<fchannels.size(); i++){

      //compute gaussian kernel
      GaussianBlur( fchannels[i], gaussian, kernel, sigma);

      //subtract images
      diff = fchannels[i] - gaussian;

      //add images back in
      fchannels[i] = fchannels[i] + alpha * diff;
   }

   //merge channels together
   merge( fchannels, fimage );

   //return image to uchar
   fimage.convertTo(image, CV_8UC3);

   //write result to file
   imwrite(argv[2], image);

   return 0;
}
