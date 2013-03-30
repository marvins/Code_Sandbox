#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

/*  simple functor which when called returns true when the value  */
/*  is within a specified range.                                  */
class range_func{
   public:
      range_func():val(0), ran(1){}
      range_func( const int v, const int r ): val(v), ran(r){ }

      bool operator ( )(const int query )const{
         if( fabs(query - val) < ran)
            return true;
         return false;
      }
   protected:
      int val, ran;
};


/*  Templated flood fill function. Build a functor which overloads the     */
/*  function operator ( ) and returns a bool to use. Allows for the fill   */
/*  to have many possible uses                                             */
template< typename TP >
vector<Point> Flood_Fill( const Mat& img, const Point& seed, TP const&  func ){

   //create new queue
   vector<Point> queue;
   Point pos, pnt, new_pnt;
   vector<Point> list;

   //clean queue and begin searching
   queue.clear();
   queue.push_back(seed);

   Mat map( img.size(), CV_8UC1);
   map = Scalar(255);

   //start of dfs
   while(!queue.empty()){

      //get next point on queue
      pos = queue.back();
      queue.pop_back();

      //insert point into blob list
      list.push_back( pos );

      //mark map as 'read'
      map.at<uchar>(pos) = uchar(100);

      //Iterate Through Neighbors
      for(int m=pos.y-1; m <= pos.y+1; m++){
         for(int n=pos.x-1; n <= pos.x+1; n++){

            //make sure pixel is in the image
            if((!(m == pos.y && n == pos.x)) && m > 0 && n > 0 && m < map.rows-1 && n < map.cols-1){

               //If pixel and map are both white
               if( func((int)img.at<uchar>(m,n))  && map.at<uchar>(m,n) == uchar(255)){ 

                  //Tag Point
                  map.at<uchar>(m,n) = uchar(0);

                  //add point to queue
                  queue.push_back(Point(n,m));
               }
            }
         }
      }
   }//end of while loop

   return list;
}


int main( int argc, char* argv[ ] ){

   //load image
   if( argc < 3 ){
      cout << "usage: ./Flood_Fill <input_image> <output_image>" << endl;
      return 0;
   }
   Mat imagein = imread( argv[1], 0);

   //apply flood fill
   Point seed(225, 91);
   range_func func(0, 10);
   vector<Point> region;
   
   region = Flood_Fill( imagein, seed, func); 

   //print all detected pixels to the image as green
   Mat imageout = imread( argv[1] );
   for(size_t i=0; i<region.size(); i++)
      imageout.at<Vec3b>( region[i] ) = Vec3b(0, 255, 0);

   imwrite( argv[2], imageout);

   return 0;
}
