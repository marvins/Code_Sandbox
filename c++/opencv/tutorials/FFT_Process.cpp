#include <complex>
#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Frequency_Transforms.h"

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){

   //basic error checking
   if( argc < 3 ){
      cout << "error: no input image specified" << endl;
      cout << "usage:  ./a.out <image file.jpg> <output file.jpg>" << endl;
      return -1;
   }

   //read image in grayscale
   Mat image = imread( argv[1], 0);
   
   //convert image to floating point
   Mat fimg;
   image.convertTo( fimg, CV_32FC1);
   
   //compute frequency domain image
   Mat_<complex<float> > fft_img = fft_2D( fimg, SHIFT );
  
   /*
      Process Frequency Image Info Here
   */

  //reconstruct image back into spatial domain
   Mat reconstructed;
   reconstructed  = fft_2D_inverse( fft_img, SHIFT );

   //prepare for output
   Mat uimg;
   reconstructed.convertTo( uimg, CV_8UC1);

   //compare sizes
   for(int i=0; i<image.rows; i++)
      for(int j=0; j<image.cols; j++)
         if( uimg.at<uchar>(i,j) != image.at<uchar>(i,j) ){
            cout << "images are not equal" << endl;
            cout << "position " << i << ", " << j << endl;
            cout << (int)uimg.at<uchar>(i,j) << ", " << (int)image.at<uchar>(i,j) << endl;
            return -1;
         }

   cout << "images are equal" << endl;
   imwrite(argv[2], uimg);

   return 0;
}
