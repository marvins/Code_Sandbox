
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){

   if( argc < 3 ){
      cout << "error: missing input image arguments" << endl;
      cout << "usage: ./High_Boost <input.jpg> <output.jpg> " << endl;
      return -1;
   }

   //some useful variables
   float a = 1.27;
   float w = 9*a -1;
   Mat gaussian, highpass;
   double sigma = 2;
   int ksize = 5 * sigma;
   if(ksize %2 == 0) ksize++;
   Size kernel(ksize, ksize);


   //read image in color
   Mat image = imread( argv[1] );

   //convert to floating point
   Mat fimage;
   image.convertTo( fimage, CV_32FC3);

   //split image into channels
   vector<Mat> fchannels;
   split( fimage, fchannels);
   
   //create Boost Kernel
   Mat A = (Mat_<float>(3,3) << -1, -1, -1, -1, w, -1, -1, -1, -1);

   //iterate through each channel
   for(size_t i=0; i<fchannels.size(); i++){

      //compute gaussian
      GaussianBlur( fchannels[i], gaussian, kernel, sigma);

      //filter image with highpass
      filter2D( fchannels[i], highpass, CV_32FC1, A, Point(-1,-1), 0, BORDER_DEFAULT);
  
      //add results
      fchannels[i] = highpass - gaussian;
   }

   //merge results
   merge( fchannels, fimage);

   //convert to uchar
   fimage.convertTo( image, CV_8UC3);

   //write to file
   imwrite( argv[2], image);

   return 0;
}

