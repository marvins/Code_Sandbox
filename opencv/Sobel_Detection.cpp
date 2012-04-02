#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){

   //some useful parameters
   double scale = 1;
   int delta = 0;
   int border = BORDER_DEFAULT;

   //open image from file
   Mat gray_image = imread( argv[1], 0);
   if( !gray_image.data ){
      cout << "Error: image data empty" << endl;
      return 0;
   }

   //convert to floating point image
   Mat float_image;
   gray_image.convertTo( float_image, CV_32FC1);

   //compute Sobel magnitude
   Mat dx, dy;
   Sobel( float_image, dx, CV_32FC1, 1, 0, 3, scale, delta, border );
   Sobel( float_image, dy, CV_32FC1, 0, 1, 3, scale, delta, border );
   
   //compute distance measurement
   Mat edge_data( gray_image.size(), CV_32FC1);
   
   MatConstIterator_<float> dx_it = dx.begin<float>();
   MatConstIterator_<float> dy_it = dy.begin<float>();
   MatIterator_<float> edge = edge_data.begin<float>();

   //iterate through image, computing magnitude
   while( dx_it != dx.end<float>() ){
      (*edge) = sqrt( pow( (*dx_it), 2) + pow( (*dy_it), 2) );
      dx_it++; dy_it++; edge++;
   }

   //write_result
   edge_data.convertTo( gray_image, CV_8UC1);
   imwrite( argv[2], gray_image);

   return 0;
}
