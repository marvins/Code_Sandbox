#include <boost/filesystem.hpp>

#include "DEM.h"
#include "../core/Enumerations.h"
#include <GeoImage.h>
#include "../utilities/DTEDUtils.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>

namespace bf = boost::filesystem;

using namespace cv;
using namespace std;

namespace GEO{

    DEM_Params::DEM_Params( const int& ftype, std::string const& root_dir ){

        filetype      = ftype;
        dted_root_dir = root_dir;
    }// end of DEM_Params constructor


    //DEM Constructor
    DEM::DEM( double const& tl_lat, double const& tl_lon, double const& br_lat, double const& br_lon, DEM_Params const& params ){
        
        /** need to start looking at how many files we need
         * 
         * 1.  DTED uses 1 deg x 1 deg grids
         */
        if( params.filetype == DTED ){


            //compute range
            br.x = max( tl_lon, br_lon);  br.y = min( tl_lat, br_lat);  
            tl.x = min( tl_lon, br_lon);  tl.y = max( tl_lat, br_lat);
            
            //check to see if we have one file or multiple files
            //build list of images required
            int lat_needed = 1 + fabs( std::floor(tl.y) - std::floor(br.y) );
            int lon_needed = 1 + fabs( std::floor(tl.x) - std::floor(br.x) );
            
            //create 2d array of final images
            vector<vector<Mat> > image_set(lon_needed);
            for( int i=0; i<lon_needed; i++)
                image_set[i] = vector<Mat>(lat_needed);

            
            //begin loading sub tiles
            for( int j=0; j<lat_needed; j++ ){
                for( int i=0; i<lon_needed; i++ ){
                    
                    //compute the required filename
                    string exp_filename = DTEDUtils::coordinate2filename( 
                            /** Lat */std::floor(br.y)+(lat_needed - j - 1)+0.0001, 
                            /** Lon */std::floor(br.x)+(lon_needed - i - 1)+0.0001 
                            );
                    
                    string act_filename = params.dted_root_dir + "/" + exp_filename;
                    cout << "loading : " << act_filename << endl;

                    //make sure the filename exists
                    if( bf::exists(bf::path(act_filename)) == false ){
                        cout << act_filename << endl;
                        throw string("Error: File does not exist");
                    }

                    //load crop
                    Mat subcrop = GEO::GeoImage( act_filename, true ).get_image();
                   
                    //make sure that pixel type is proper
                    if( subcrop.type() != CV_16SC1 )
                        throw string("Image must be CV_16SC1" );
                   
                   
                    //get crop range
                    pair<double,double> lat_ran( std::max( std::floor(br.y)+j, br.y ), std::min( std::ceil(br.y)+j, tl.y));
                    pair<double,double> lon_ran( std::max( std::floor(tl.x)+i, tl.x ), std::min( std::ceil(tl.x)+i, br.x));
                   
                    //compute the percentage of the image you are using
                    pair<double,double> lat_pct( lat_ran.first - std::floor(lat_ran.first), 1 - (std::ceil(lat_ran.second) - lat_ran.second));
                    pair<double,double> lon_pct( lon_ran.first - std::floor(lon_ran.first), 1 - (std::ceil(lon_ran.second) - lon_ran.second));
                    
                    //given the percentage, compute the pixel range you need
                    pair<int,int> lat_img_ran( lat_pct.first*subcrop.rows, lat_pct.second*subcrop.rows);
                    pair<int,int> lon_img_ran( lon_pct.first*subcrop.cols, lon_pct.second*subcrop.cols);
                    
                    cout << "pixel range x: " << lon_img_ran.first << ", " << lon_img_ran.second << endl;
                    cout << "pixel range y: " << lat_img_ran.first << ", " << lat_img_ran.second << endl;
                    
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

            /** We have now compiled a list of required tiles 
             *  - Now we need to merge them into a single image
            */
            
            cout << "HERE" << endl;
            //compute the expected width and height of the image
            int final_x = 0;
            int final_y = 0;
            for( int i=0; i<lon_needed; i++ )
                final_x += image_set[i][0].cols;
            for( int i=0; i<lat_needed; i++ )
                final_y += image_set[0][i].rows;
            
            cout << "Final Image Statistics" << endl;
            cout << "Image Width : " << final_x << endl;
            cout << "Image Height: " << final_y << endl;
            cout << endl;
            cout << "Individual image sizes" << endl;
            for( size_t i=0; i<image_set.size(); i++)
                for( size_t j=0; j<image_set[i].size(); j++)
                    cout << "image (" << i << ", " << j << ") is " << image_set[i][j].cols << " by " << image_set[i][j].rows << endl;
            exit(0);

            //create final tile
            tile = Mat( Size( final_x, final_y), image_set[0][0].type() );
            tile = Scalar(0);

            int cx = 0;
            int cy = 0;

            //start loading each image in image set into final tile
            for( size_t i=0; i<image_set.size(); i++)
            for( size_t j=0; j<image_set[i].size(); j++){

                //iterate over specific image
                for( int x =0; x < image_set[i][j].cols; x++ )
                for( int y =0; y < image_set[i][j].rows; y++ ){

                    if( image_set[0][0].type() == CV_16UC1 )
                        tile.at<ushort>( y+cy, x+cx) = image_set[i][j].at<ushort>( y, x);
                    else if( image_set[0][0].type() == CV_16SC1 )
                        tile.at<short>(  y+cy, x+cx) = image_set[i][j].at<short>(  y, x);
                    else
                        throw string("ERROR: unsupported dem format");
                }

                //update cx, cy
                cx += image_set[i][j].cols;
                cy += image_set[i][j].rows;
            }


            return;

        }
        else{
            throw std::string("Error: unsupported DEM format");
        }

}

/**
 * Destructor
 */
DEM::~DEM(){
}

/**
 * Pull the elevation data as an OpenCV Mat
 */
cv::Mat DEM::get_raw()const{

    if( !tile.data )
        throw string("Error: tile data uninitialized");
    return tile.clone();
} //end of get_raw function


/**
 * Return the value and location of the highest elevation
 * in the tile.
 */
double DEM::max_elevation( double& lat, double& lon )const{

    double _max = 0;
    int I = 0, J = 0;
    for( size_t i=0; i<tile.cols; i++)
        for( size_t j=0; j<tile.rows; j++ )
            if( tile.type() == CV_16SC1 ){
                if( tile.at<short>(j,i) > _max ){
                    _max = tile.at<short>(j,i);
                    I = i;
                    J = j;
                }
            }
    lat = J;
    lon = I;

    return _max;
}

}//end of GEO namespace

