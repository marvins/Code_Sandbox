#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <iostream>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){

   // you can initialize the video object using
   // the same commands as open()
   VideoCapture v_stream;

   //Create VideoCapture Object
   if(argc > 1)
      v_stream.open(argv[1]); //load video file
   else
      v_stream.open(0);   //load default camera (webcam)

   //check to see if video loads properly
   if(!v_stream.isOpened()){
      cout << "Error: Video Stream not loaded" << endl;
      exit(0);
   }

   //extract data and write to screen
   Mat output;
   namedWindow("Video",0);
   char key;

   v_stream >> output; //pop frame from camera
   while( output.data ){ //make sure data is still present

      imshow("Video", output);
      key = waitKey(30); //wait 30 milliseconds;
      if( (int)key >= 0)  break; //exit if key pressed
      
      //pop next frame
      v_stream >> output;
   }

   return 0;
}

