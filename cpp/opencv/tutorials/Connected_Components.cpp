#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

/*  a very basic DFS implementation of the connected components */
/*  algorithm.                                                  */
vector<vector<Point> > Connected_Components( Mat const& image ){

   //misc parameters
   Mat map( image.size(), CV_8UC1);
   map = uchar(255);

   //create list
   vector<vector<Point> > list;
   list.clear();

   //create new queue
   vector<Point> part;
   vector<Point> queue;
   Point pos, pnt, new_pnt;

   //iterate through map
   for(int i=0; i<map.rows-1; i++){
      for(int j=0; j<map.cols-1; j++){

         //check if both image and map are white
         if( image.at<uchar>(i,j) == uchar(255) && map.at<uchar>(i,j) == uchar(255) ){

            //find components, add new point to queue
            queue.clear();
            queue.push_back(Point(j,i));

            //start of dfs
            while(!queue.empty()){

               //get next point on queue
               pos = queue.back();
               queue.pop_back();

               //insert point into blob list
               part.push_back( pos );

               //mark map as 'read'
               map.at<uchar>(pos) = uchar(100);

               //Iterate Through Neighbors
               for(int m=pos.y-1; m <= pos.y+1; m++){
                  for(int n=pos.x-1; n <= pos.x+1; n++){

                     //make sure pixel is in the image
                     if((!(m == pos.y && n == pos.x)) && m >= 0 && n >= 0 && m < map.rows-1 && n < map.cols-1){

                        //If pixel and map are both white
                        if( image.at<uchar>(m,n) == 255 && map.at<uchar>(m,n) == 255){

                           //Tag Point
                           map.at<uchar>(m,n) = float(100);

                           //add point to queue
                           queue.push_back(Point(n,m));
                        }
                     }
                  }
               }
            }//end of while loop

            //add part to list
            list.push_back(part);

            //clear temp storage
            part.clear();
         }
      }
   }

   //return regions
   return list;
}


int main( int argc, char* argv[ ] ){

   //load image
   if( argc < 3 ){
      cout << "usage: ./Flood_Fill <input_image> <output_image>" << endl;
      return 0;
   }
   Mat imagein = imread( argv[1], 0);

   //create region list
   vector<vector<Point> > regions;

   //threshold image into binary image
   threshold( imagein, imagein, 50, 255, THRESH_BINARY_INV );

   //compute connected components
   regions = Connected_Components( imagein );

   cout << "found " << (int)regions.size() << endl;

   //output each detected region
   Mat imageout = imread( argv[1] );
   for(size_t i=0; i<regions.size(); i++)
      for(size_t j=0; j<regions[i].size(); j++)
         imageout.at<Vec3b>(regions[i][j]) = Vec3b(0,255,0);

   //write data
   imwrite(argv[2], imageout);

   return 0;
}
