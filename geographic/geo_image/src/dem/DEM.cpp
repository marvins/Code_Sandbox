#include "DEM.hpp"

#include "../image/GeoImage.hpp"
#include "../utilities/DEM_Utilities.hpp"
#include "../utilities/File_Utilities.hpp"
#include "../utilities/Math_Utilities.hpp"
#include "../utilities/OpenCV_Utilities.hpp"

#include <cmath>
#include <iostream>
#include <string>


using namespace cv;
using namespace std;

namespace GEO{

    DEM_Params::DEM_Params( const int& ftype, std::string const& root_dir ){

        filetype      = ftype;
        dted_root_dir = root_dir;
    }// end of DEM_Params constructor
    

    
    /**
     * Default Constructor
    */
    DEM::DEM( ){
    
    }


    /**
     * Parameterized Constructor given a pre-existing DEM Tile and both corners
    */
    DEM::DEM( const Point2f& _tl, const Point2f& _br, const Mat& data ){
        
        //set the image corners
        m_min = Point2f( std::min(_tl.x, _br.x), std::min(_tl.y, _br.y));
        m_max = Point2f( std::max(_tl.x, _br.x), std::max(_tl.y, _br.y));
        
        //set the tile data
        tile = data.clone();

    }

    //DEM Constructor
    DEM::DEM( Point2f const& _min, Point2f const& _max, 
              DEM_Params const& params ){
        
        /** need to start looking at how many files we need
         * 
         * 1.  DTED uses 1 deg x 1 deg grids
         */
        double tempD;
        if( params.filetype == DTED ){

            //compute range
            m_min = Point2f( std::min( _min.x, _max.x), std::min( _min.y, _max.y));
            m_max = Point2f( std::max( _min.x, _max.x), std::max( _min.y, _max.y));

            //check to see if we have one file or multiple files
            //build list of images required
            int lat_needed = 1 + fabs( std::floor(m_max.y) - std::floor(m_min.y) );
            int lon_needed = 1 + fabs( std::floor(m_max.x) - std::floor(m_min.x) );
           
            GEO::DTED_adjust_needed_tiles( lat_needed, lon_needed, m_min, m_max );

            //create 2d array of final images
            vector<vector<Mat> > image_set(lon_needed);
            for( int i=0; i<lon_needed; i++)
                image_set[i] = vector<Mat>(lat_needed);

            
            //begin loading sub tiles
            for( int j=0; j<lat_needed; j++ ){
                for( int i=0; i<lon_needed; i++ ){
                    
                    //compute the required filename
                    string exp_filename = DTED_coordinate2filename( 
                            /** Lat */std::floor(m_min.y)+(lat_needed - j - 1)+0.0001, 
                            /** Lon */std::floor(m_min.x)+(i)+0.0001 
                            );
                    
                    string act_filename = params.dted_root_dir + "/" + exp_filename;
                    
                    //make sure the filename exists
                    if( STR::file_exists(act_filename) == false ){
                        throw string("Error: File does not exist");
                    }
                    
                    //load crop
                    Mat subcrop = GEO::GeoImage( act_filename, true ).get_image();
                    
                    //get crop range
                    pair<double,double> lat_ran( std::max( std::floor(m_min.y+j), m_min.y ), std::min( std::ceil(m_max.y-j), m_max.y));
                    pair<double,double> lon_ran( std::max( std::floor(m_min.x+i), m_min.x ), std::min( std::ceil(m_max.x-i), m_max.x));
                    
                    //compute the percentage of the image you are using
                    pair<double,double> lat_pct( lat_ran.first - std::floor(lat_ran.first), 1 - (std::ceil(lat_ran.second) - lat_ran.second));
                    pair<double,double> lon_pct( lon_ran.first - std::floor(lon_ran.first), 1 - (std::ceil(lon_ran.second) - lon_ran.second));
                    
                    //since images have an inverse range, we need to flip values
                    tempD  = 1 - lat_pct.first;
                    lat_pct.first = 1 - lat_pct.second;
                    lat_pct.second= tempD;

                   //given the percentage, compute the pixel range you need
                    pair<int,int> lat_img_ran( lat_pct.first*subcrop.rows, lat_pct.second*subcrop.rows);
                    pair<int,int> lon_img_ran( lon_pct.first*subcrop.cols, lon_pct.second*subcrop.cols);
                    
                    //create the actual final image
                    Mat crop( lat_img_ran.second-lat_img_ran.first, 
                              lon_img_ran.second-lon_img_ran.first, 
                              subcrop.type());

                    //load pixels
                    for( int x=lon_img_ran.first; x<lon_img_ran.second; x++ ){
                    for( int y=lat_img_ran.first; y<lat_img_ran.second; y++ )
                            
                            if( subcrop.type() == CV_8UC1 ){
                                crop.at<uchar>(y-lat_img_ran.first, x-lon_img_ran.first) = subcrop.at<uchar>(y,x);
                            }
                            else if( subcrop.type() == CV_16UC1 ){
                                crop.at<ushort>(y-lat_img_ran.first, x-lon_img_ran.first) = subcrop.at<ushort>(y,x);
                            }
                            else if( subcrop.type() == CV_16SC1 ){
                                crop.at<short>(y-lat_img_ran.first, x-lon_img_ran.first) = subcrop.at<short>(y,x);
                            }
                            else
                                throw string("TYPE NOT SUPPORTED");
                        }
                    
                    //add final image to image set
                    image_set[i][j] = crop.clone();
                
                }//end i
            }//end j

            // merge all bands
            if( image_set.size() > 1 ) throw string("MULTI-BAND DTED NOT SUPPORTED YET");
            if( image_set[0].size() > 1 ) throw string("MULTI-BAND DTED NOT SUPPORTED YET");
            
            tile = image_set[0][0];

        }// end of dted module load
    }


    /**
     * Destructor
     */
    DEM::~DEM(){
    
    }

    /**
     * Pull the elevation data as an OpenCV Mat
     */
    cv::Mat DEM::get_tile()const{

        if( !tile.data )
            return Mat();
        return tile.clone();
    } //end of get_raw function
    
    /**
     * Set the tile data
    */
    void DEM::set_tile( const Mat& data ){
        tile = data.clone();
    }
    
    /**
     * Return the value of the highest elevation
     * in the tile.
     */
    double DEM::max_elevation( )const{
        
        double _max = 0;
        for( size_t i=0; i<tile.cols; i++)
            for( size_t j=0; j<tile.rows; j++ )
                if( cvGetPixel( tile, Point(i,j), 0 ) > _max ){
                        _max = cvGetPixel( tile, Point(i, j), 0);
                    }
        return _max;
    }

    /**
     * Return the value and location of the highest elevation
     * in the tile.
     */
    double DEM::max_elevation( cv::Point2f& coord )const{

        double _max = 0;
        int I = 0, J = 0;
        for( size_t i=0; i<tile.cols; i++)
            for( size_t j=0; j<tile.rows; j++ )
                if( cvGetPixel( tile, Point(i,j), 0 ) > _max ){
                        _max = cvGetPixel( tile, Point(i,j), 0 );
                        I = i;
                        J = j;
                }
        coord.y = (1 - J/(double)tile.rows) * (m_max.y - m_min.y) + m_min.y;
        coord.x = (I/(double)tile.cols) * (m_max.x - m_min.x) + m_min.x;

        return _max;
    }


    /** 
     * Return the elevation at the specified coordinate.
    */
    double DEM::query_elevation( const Point2f& coordinate )const{
        int realX = math_round((coordinate.x - m_min.x)/(m_max.x-m_min.x) * tile.cols);
        int realY = math_round((1 - (coordinate.y - m_min.y)/(m_max.y-m_min.y)) * tile.rows);
        
        return cvGetPixel( tile, Point(realX, realY), 0 );
    }
    
    /**
     * Same as before, but with a LatLon Coordinate
    */
    double DEM::query_elevation( const CoordinateLatLon& coordinate )const{
        int realX = math_round((coordinate.lon - m_min.x)/(m_max.x-m_min.x) * tile.cols);
        int realY = math_round((1 - (coordinate.lat - m_min.y)/(m_max.y-m_min.y)) * tile.rows);
        
        return cvGetPixel( tile, Point(realX, realY), 0 );
    }

/*
Vec3b color_relief( double elevation, double minC, double maxC ){

    double maxR = maxC;
    double minR = minC;

    double x = ((maxR - minR)-(elevation - minR))/(maxR - minR)*255;
    double y = 255 - x;
    double z = 0;

    if( x > 255 ) x = 255;
    if( y > 255 ) y = 255;
    if( z > 255 ) z = 255;
    if( x < 0 )   x = 0;
    if( y < 0 )   y = 0;
    if( z < 0 )   z = 0;
    
    return Vec3b( x, y, z);
}



Mat DEM::relief_map()const{

    //create a 3 channel image
    Mat output( tile.size(), CV_8UC3);

    //iterate over the image applying the new pixel values
    for( size_t i=0; i<tile.cols; i++)
    for( size_t j=0; j<tile.rows; j++)
        output.at<Vec3b>(j,i) = color_relief( tile.at<short>(j,i), 1000, 4450 );

    return output;
    
}

Point DEM::get_pixel_coordinate( Point2f const& coordinate ){

    //find the percentage of the image from the tl to br
    double pctX = tile.cols*(coordinate.x - tl.x )/(br.x - tl.x);
    double pctY = tile.rows*(1.0-(coordinate.y - br.y )/(tl.y - br.y));

    return Point( pctX, pctY);

}

*/
    
    Point2f DEM::ne()const{  return Point2f( m_max.x, m_max.y ); }
    Point2f DEM::nw()const{  return Point2f( m_min.x, m_max.y ); }
    Point2f DEM::se()const{  return Point2f( m_max.x, m_min.y ); }
    Point2f DEM::sw()const{  return Point2f( m_min.x, m_min.y ); }

}//end of GEO namespace

