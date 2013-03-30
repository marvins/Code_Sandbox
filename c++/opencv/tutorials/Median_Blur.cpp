#include <ctime>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){

   //read image in grayscale
   Mat image = imread(argv[1],0);

   //apply random salt and pepper noise
   // to reiterate point
   srand(unsigned(time(NULL)));

   int value;
   for(int i=0; i<image.rows; i++)
      for(int j=0; j<image.cols; j++){
         value = rand()%100;

         if( value < 25 )
            image.at<uchar>(i,j) = 0;
         else if( value < 50 )
            image.at<uchar>(i,j) = 255;
      }

    //output image to show before
    imwrite("noisy.jpg",image );

    //apply median filtering
    medianBlur( image, image, 11 );

    //write to file
    imwrite( argv[2], image );

    return 0;
}
