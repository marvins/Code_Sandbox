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

            //compute range
            double minx = min( tl_lon, br_lon);  double maxx = max( tl_lon, br_lon);
            double miny = min( tl_lat, br_lat);  double maxy = max( tl_lat, br_lat);

            //check to see if we have one file or multiple files

            //build list of images required
            int lat_needed = 1 + fabs( std::floor(maxy) - std::floor(miny) );
            int lon_needed = 1 + fabs( std::floor(maxx) - std::floor(minx) );

            vector<Mat> crops;

            //create size table
            vector<vector<pair<int,int> > > pos_list;
            pos_list.resize(lon_needed);
            for( size_t i=0; i<lon_needed; i++)
                pos_list[i].resize(lat_needed);


            //begin loading sub tiles
            for( int j=0; j<lat_needed; j++ ){
                for( int i=0; i<lon_needed; i++ ){

                    //compute the required filename
                    string exp_filename = DTEDUtils::coordinate2filename( 
                            /** Lat */std::floor(miny)+j+0.0001, 
                            /** Lon */std::floor(minx)+i+0.0001 
                            );

                    string act_filename = params.dted_root_dir + "/" + exp_filename;

                    //make sure the filename exists
                    if( bf::exists(bf::path(act_filename)) == false ){
                        cout << act_filename << endl;
                        throw string("Error: File does not exist");
                    }

                    //load crop
                    Mat subcrop = GEO::GeoImage( act_filename, true ).get_image();

                    //get crop range
                    pair<double,double> lat_ran( std::max( std::floor(miny)+j, miny ), std::min( std::ceil(miny)+j, maxy));
                    pair<double,double> lon_ran( std::max( std::floor(minx)+i, minx ), std::min( std::ceil(minx)+i, maxx));

                    pair<double,double> lat_pct( lat_ran.first - std::floor(lat_ran.first), 1 - (std::ceil(lat_ran.second) - lat_ran.second));
                    pair<double,double> lon_pct( lon_ran.first - std::floor(lon_ran.first), 1 - (std::ceil(lon_ran.second) - lon_ran.second));

                    pair<int,int> lat_img_ran( lat_pct.first*subcrop.rows, lat_pct.second*subcrop.rows);
                    pair<int,int> lon_img_ran( lon_pct.first*subcrop.cols, lon_pct.second*subcrop.cols);

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
                }
            }

            vector<int> xlens;  xlens.push_back(0);
            for( size_t xx=0; xx<lon_needed; xx++ )
                xlens.push_back( crops[xx].cols + xlens[xx]);

            vector<int> ylens;  ylens.push_back(0);
            for( size_t yy=0; yy<lat_needed; yy++ )
                ylens.push_back( crops[yy].rows + ylens[yy]);

            int cx = 0;
            int cy = 0;
            tile = Mat( Size( xlens.back(), ylens.back()), crops[0].type() );

            for( size_t ii=0; ii<crops.size(); ii++){

                cx = xlens[ii%lon_needed];
                cy = ylens[ii/lon_needed];

                for( int xx =0; xx < crops[ii].cols; xx++ )
                    for( int yy =0; yy < crops[ii].rows; yy++ ){
                        if( crops[0].type() == CV_16UC1 )
                            tile.at<ushort>( yy-cy, xx-cx) = crops[ii].at<ushort>( yy, xx);
                        else if( crops[0].type() == CV_16SC1 )
                            tile.at<short>( yy-cy, xx-cx) = crops[ii].at<short>( yy, xx);
                        else
                            throw string("ERROR: unsupported dem format");
                    }
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
    } //end of get_raw function

}//end of GEO namespace

