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
}

DEM::DEM( double const& tl_lat, double const& tl_lon, double const& br_lat, double const& br_lon, DEM_Params const& params ){

    /** need to start looking at how many files we need
     * 
     * 1.  DTED uses 1 deg x 1 deg grids
    */
    if( params.filetype == DTED ){

        /**  there are a few circumstances where we need to consider 
         *   1.  The region is completely inside a tile
         *      - Just pull a sub-region of the image
         *   2.  The region straddles the edge of multiple tiles
         *      - find intersection boundaries
        */

        //compute the lower ranges of the corners
        int tl_lat_floor = floor(tl_lat);
        int tl_lon_floor = floor(tl_lon);
        int br_lat_floor = floor(br_lat);
        int br_lon_floor = floor(br_lon);

        //compute range
        double minx = min( tl_lon, br_lon);  double maxx = max( tl_lon, br_lon);
        double miny = min( tl_lat, br_lat);  double maxy = max( tl_lat, br_lat);

        //check to see if we have one file or multiple files
        if( tl_lat_floor == br_lat_floor && tl_lon_floor == br_lon_floor ){

            //retrieve the filename
            string exp_filename = DTEDUtils::coordinate2filename( tl_lat_floor, tl_lon_floor );
            string act_filename = params.dted_root_dir + "/" + exp_filename;

            //make sure the filename exists
            if( bf::exists(bf::path(act_filename)) == false ){
                cout << act_filename << endl;
                throw string("Error: File does not exist");
            }

            //load file
            GeoImage elevation_tile(act_filename, true);
            cv::Mat cv_tile = elevation_tile.get_image();

            int range_minx = (minx - std::floor(tl_lon))*cv_tile.cols;
            int range_maxx = (1-(std::ceil(tl_lon)  - maxx))*cv_tile.cols;
            int range_miny = (miny - std::floor(tl_lat))*cv_tile.rows;
            int range_maxy = (1-(std::ceil(tl_lat)  - maxy))*cv_tile.rows;

            cv::Mat crop( cv::Size( range_maxx - range_minx, range_maxy - range_miny ), cv_tile.type());
            for( int x=range_minx; x< range_maxx; x++ )
                for( int y=range_miny; y< range_maxy; y++ ){
                    // CV_8UC1
                    if( cv_tile.type() == CV_8UC1 )
                        crop.at<uchar>(y-range_miny, x-range_minx) = cv_tile.at<uchar>(y, x);
                    // CV_16UC1
                    else if( cv_tile.type() == CV_16UC1 )
                        crop.at<unsigned short>(y-range_miny, x-range_minx) = cv_tile.at<unsigned short>(y, x);
                    // CV_16SC1
                    else if( cv_tile.type() == CV_16SC1 ){
                        crop.at<short>(y-range_miny, x-range_minx) = cv_tile.at<short>(y, x);
                    }
                    // CV_8UC3
                    else if( cv_tile.type() == CV_8UC3 )
                        crop.at<Vec3b>(y-range_miny, x-range_minx) = cv_tile.at<Vec3b>(y, x);
                    else 
                        throw string("ERROR: unsupported pixel type");
                }

            tile = crop;
        }
        else{

            //build list of images required
            int lat_needed = 1 + fabs( std::floor(maxy) - std::floor(miny) );
            int lon_needed = 1 + fabs( std::floor(maxx) - std::floor(minx) );

            int img_width = 0;
            int img_height= 0;
            int maxX = 0, maxY = 0, curX = -1, curY = -1;
            vector<Mat> crops;
            
            //create size table
            vector<vector<pair<int,int> > > pos_list;
            pos_list.resize(lon_needed);
            for( size_t i=0; i<lon_needed; i++)
                pos_list[i].resize(lat_needed);

            
            //begin loading sub tiles
            for( int j=0; j<lon_needed; j++ ){
            for( int i=0; i<lat_needed; i++ ){

                //compute the required filename
                string exp_filename = DTEDUtils::coordinate2filename( std::floor(miny)+i+0.0001, std::floor(minx)+j+0.0001 );
                string act_filename = params.dted_root_dir + "/" + exp_filename;

                //make sure the filename exists
                if( bf::exists(bf::path(act_filename)) == false ){
                    cout << act_filename << endl;
                    throw string("Error: File does not exist");
                }

                //load crop
                Mat subcrop = GEO::GeoImage( act_filename, true ).get_image();

                //ensure crop tile is the same size as previous tile sizes
                if(      current_height == 0 ) current_height = subcrop.rows;
                else if( current_height != subcrop.rows )
                    throw string("ERROR: tile is not the same size as the adjacent tile");

                //get crop range
                pair<double,double> lat_ran( std::max( std::floor(miny)+i, miny ), std::min( std::ceil(miny)+j, maxy));
                pair<double,double> lon_ran( std::max( std::floor(minx)+i, minx ), std::min( std::ceil(minx)+j, maxx));

                pair<double,double> lat_pct( lat_ran.first - std::floor(lat_ran.first), 1 - (std::ceil(lat_ran.second) - lat_ran.second));
                pair<double,double> lon_pct( lon_ran.first - std::floor(lon_ran.first), 1 - (std::ceil(lon_ran.second) - lon_ran.second));

                pair<int,int> lat_img_ran( lat_pct.first*subcrop.rows, lat_pct.second*subcrop.rows);
                pair<int,int> lon_img_ran( lon_pct.first*subcrop.cols, lon_pct.second*subcrop.cols);

                cout << "ran: " << lat_ran.first << ", " << lat_ran.second << "  x  " << lon_ran.first << ", " << lon_ran.second << endl;
                cout << "prop img size: " << lat_img_ran.first << ", " << lat_img_ran.second << "  vs  " << lon_img_ran.first << ", " << lon_img_ran.second << endl;
                Mat crop( lat_img_ran.second-lat_img_ran.first, lon_img_ran.second-lon_img_ran.first, subcrop.type());

                for( int ii=lat_img_ran.first; ii<lat_img_ran.second; ii++ )
                    for( int jj=lon_img_ran.first; jj<lon_img_ran.second; jj++ ){
                        if( subcrop.type() == CV_8UC1 ){
                            crop.at<uchar>(ii-lat_img_ran.first, jj-lon_img_ran.first) = subcrop.at<uchar>(ii,jj);
                        }
                        else if( subcrop.type() == CV_16UC1 ){
                            crop.at<ushort>(ii-lat_img_ran.first, jj-lon_img_ran.first) = subcrop.at<ushort>(ii,jj);
                        }
                        else if( subcrop.type() == CV_16SC1 ){
                            crop.at<short>(ii-lat_img_ran.first, jj-lon_img_ran.first) = subcrop.at<short>(ii,jj);
                        }
                        else
                            throw string("TYPE NOT SUPPORTED");
                    }

                crops.push_back(crop.clone());

                if( i == 0 )
                    pos_list[i][j].first = crop.cols;
                else
                    pos_list[i][j].first = crop.cols + pos_list[i-1][j].first;

                if( j == 0 )
                    pos_list[i][j].second = crop.rows;
                else
                    pos_list[i][j].second = crop.rows + pos_list[i][j-1].second;

                if( j > maxY )
                    maxY = j;
                if( i > maxX )
                    maxX = i;

                if( i > curX ){
                    curX = i;
                    img_width += crop.cols;
                }
                if( j > curY ){
                    curY = j;
                    img_height += crop.rows;
                }

            }
            }


            int cX = 0;
            int cY = 0;
            int cRow = 0;
            tile = Mat( img_width, img_height, crops[0].type() );

            for( size_t ii=0; ii<crops.size(); ii++){
                
                cx = pos_list[ii/lon_needed][ii%lat_needed].first;
                cy = pos_list[ii/lon_needed][ii%lat_needed].second;
                for( int xx =0; xx < crops[ii].cols; xx++ )
                    for( int yy =0; yy < crops[ii].rows; yy++ ){
                        if( crops[0].type() == CV_16UC1 )
                            tile.at<ushort>( yy+cy, xx+cx) = crops[ii].at<ushort>( yy, xx);
                        else if( crops[0].type() == CV_16SC1 )
                            tile.at<short>( yy+cy, xx+cx) = crops[ii].at<short>( yy, xx);
                        else
                            throw string("ERROR: unsupported dem format");
                    }
             }



                throw string("ERROR: Multiple files unsupported");

            }

        }
        else{
            throw std::string("Error: unsupported DEM format");
        }


    }


    cv::Mat DEM::get_raw()const{

        if( !tile.data )
            throw string("Error: tile data uninitialized");
        return tile.clone();
    }

}//end of GEO namespace

