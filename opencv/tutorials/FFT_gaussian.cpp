#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include<iostream>
#include<fstream>
#include<stdio.h>
#include<stdlib.h>
#include<string>

#include "Frequency_Transforms.h"

using namespace cv;
using namespace std;

float pi = 3.14159265;
/*
    Create Gaussian Kernels 2D
    - This function initializes a 2 dimensional gaussian kernel. 
*/
Mat create_kernel_gaussian_2D(const float sigma){

      int len = (int)(5*sigma);
      if(len%2==0) len++;

      Mat output(len,len,CV_32FC1);
      for(int i=0;i<len;i++)
        for(int j=0;j<len;j++)
          output.at<float>(i,j) = (1/(2*pi*sigma*sigma))*exp(-1.0*(pow(i-(len/2.0),2)+pow(j-(len/2.0),2))/(2.0*sigma*sigma));
   return output;
}

int main(int argc, char* argv[])
{

   //load image to be smoothed
   Mat image = imread( argv[1], 0);
   float sigma = 2;

   //create gaussian kernel
   Mat ker = create_kernel_gaussian_2D(sigma);

   //create new image size struct
   Size sz( image.rows + ker.rows - 1, image.cols + ker.cols - 1);

   //create modified Kernel with new size
   Mat new_ker( sz, CV_32FC1);
   new_ker = Scalar(0);
   
   for(int i=0; i<ker.cols; i++)
      for(int j=0; j<ker.rows; j++)
         new_ker.at<float>(j,i) = ker.at<float>(j,i);

   //Apply FFT on gaussian kernel
   Mat_<complex<float> > fft_ker = fft_2D( new_ker, SHIFT);


   /*  Create Image  */
   Mat fimg( sz, CV_32FC1);
   for(int i=0; i<fimg.rows; i++)
      for(int j=0; j<fimg.cols; j++){
         if( i < image.rows && j < image.cols )
            fimg.at<float>(i,j) = (float)image.at<uchar>(i,j);
         else
            fimg.at<float>(i,j) = 0;
      }



   //apply fft on image  
   Mat_<complex<float> > FFT_mat = fft_2D( fimg, SHIFT);


   /*   COMBINE IMAGES  */
   Mat_<complex<float> > FFT_GAUSSIAN( fft_ker.rows, fft_ker.cols);
   for(int i=0; i<fft_ker.cols; i++)
      for(int j=0; j<fft_ker.rows; j++)
         FFT_GAUSSIAN(j,i) = FFT_mat(j,i) * fft_ker(j,i);



   Mat gaussian_out = fft_2D_inverse( FFT_GAUSSIAN, SHIFT);
   Mat gaussian_save;
   normalize( gaussian_out, gaussian_out, 0, 255, CV_MINMAX);
   gaussian_out.convertTo(gaussian_save, CV_8UC1);
   imwrite( argv[2],gaussian_save);

   return 0;
}
