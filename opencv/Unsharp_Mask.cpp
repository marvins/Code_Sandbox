#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

using namespace cv;

void UnsharpFilter( Mat const& input_image, Mat& output_image, double sigma, double alpha ){

   //set kernel size
   int kernel_size = 5*sigma;
   Size kernel( kernel_size, kernel_size);
   
   //split image into channels
   vector<Mat> channels;
   split( input_image, channels );
   
   //iterate over each channel
   Mat gaussian, diff;
   for( size_t i=0; i<channels.size(); i++ ){

      //compute gaussian kernel
      GaussianBlur( channels[i], gaussian, kernel, sigma);

      //subtract images
      diff = channels[i] - gaussian;

      //add images back in
      channels[i] = channels[i] + alpha * diff;
   }

   //merge channels together
   Mat merged_image;
   merge( channels, merged_image );

   //return image to uchar
   merged_image.convertTo( output_image, CV_8UC3);

}


int main( int argc, char* argv[] ){

   //some relevant variables
   double sigma    = 1;
   double alpha = 5;

   Mat image01 = imread( argv[1], 0);
   Mat image02 = imread( argv[1], 1);

   Mat imageout01, imageout02;
   UnsharpFilter( image01, imageout01, 1, 5 );
   UnsharpFilter( image02, imageout02, 1, 5 );

   namedWindow("WINDOW");
   imshow("WINDOW", image01);
   waitKey(0);
   imshow("WINDOW", imageout01);
   waitKey(0);
   imshow("WINDOW", image02);
   waitKey(0);
   imshow("WINDOW", imageout02);
   waitKey(0);

   //Mat gaussian, diff;
   
   //int kernel_size = 5*sigma;
   //Size kernel( kernel_size, kernel_size);

   //read image in color
   //Mat image = imread( argv[1]);

   //convert to floating point
   //Mat fimage;
   //image.convertTo( fimage, CV_32FC3);

   //split into 3 channels
   //vector<Mat> fchannels;
   //split( fimage, fchannels);

   //iterate over each color channel
   /*for( size_t i=0; i<fchannels.size(); i++){

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
   */

   return 0;
}
