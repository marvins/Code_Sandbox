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
        
        cout << "Inside DEM Constructor" << endl;
        /** need to start looking at how many files we need
         * 
         * 1.  DTED uses 1 deg x 1 deg grids
         */
        if( params.filetype == DTED ){



            //compute range
            br.x = max( tl_lon, br_lon);  
            br.y = min( tl_lat, br_lat);  
            
            tl.x = min( tl_lon, br_lon);
            tl.y = max( tl_lat, br_lat);
            
            //check to see if we have one file or multiple files
            //build list of images required
            int lat_needed = 1 + fabs( std::floor(tl.y) - std::floor(br.y) );
            int lon_needed = 1 + fabs( std::floor(tl.x) - std::floor(br.x) );
            vector<Mat> crops;
            

            //begin loading sub tiles
            for( int j=0; j<lat_needed; j++ ){
                for( int i=0; i<lon_needed; i++ ){

                    //compute the required filename
                    string exp_filename = DTEDUtils::coordinate2filename( 
                            /** Lat */std::floor(br.y)+(lat_needed - j - 1)+0.0001, 
                            /** Lon */std::floor(br.x)+(lon_needed - i - 1)+0.0001 
                            );

                    string act_filename = params.dted_root_dir + "/" + exp_filename;

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
                    pair<double,double> lon_ran( std::max( std::floor(br.x)+i, br.x ), std::min( std::ceil(br.x)+i, tl.x));
                    
                    //compute the percentage of the image you are using
                    pair<double,double> lat_pct( lat_ran.first - std::floor(lat_ran.first), 1 - (std::ceil(lat_ran.second) - lat_ran.second));
                    pair<double,double> lon_pct( lon_ran.first - std::floor(lon_ran.first), 1 - (std::ceil(lon_ran.second) - lon_ran.second));
                    
                    cout << "next" << endl;
                    cout << lat_pct.first << ", " << lat_pct.second << endl;
                    cout << lon_pct.first << ", " << lon_pct.second << endl;

                    pair<int,int> lat_img_ran( lat_pct.first*subcrop.rows, lat_pct.second*subcrop.rows);
                    pair<int,int> lon_img_ran( lon_pct.first*subcrop.cols, lon_pct.second*subcrop.cols);

                    // Compute image range
                    cout << "here" << endl;
                    cout << lat_img_ran.first << ", " << lat_img_ran.second << endl;
                    cout << lon_img_ran.first << ", " << lon_img_ran.second << endl;
                    exit(0);

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
            
            //compute the expected width of the image
            vector<int> xlens;  xlens.push_back(0);
            for( size_t xx=0; xx<lon_needed; xx++ )
                xlens.push_back( crops[xx].cols + xlens[xx]);

            //compute the expected height of the image
            vector<int> ylens;  ylens.push_back(0);
            for( size_t yy=0; yy<lat_needed; yy++ )
                ylens.push_back( crops[yy*lon_needed].rows + ylens[yy]);

            int cx = 0;
            int cy = 0;
            tile = Mat( Size( xlens.back(), ylens.back()), crops[0].type() );
            tile = Scalar(0);

            for( size_t ii=0; ii<crops.size(); ii++){

                cx = xlens[ii%lon_needed];
                cy = ylens[ii/lon_needed];
                
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
            return;

        }
        else{
            throw std::string("Error: unsupported DEM format");
        }



            /*
        
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
                            ** Lat *std::floor(br.y)+(lat_needed - j - 1)+0.0001, 
                            ** Lon *std::floor(br.x)+(lon_needed - i - 1)+0.0001 
                            );

                    string act_filename = params.dted_root_dir + "/" + exp_filename;

                    //make sure the filename exists
                    if( bf::exists(bf::path(act_filename)) == false ){
                        cout << act_filename << endl;
                        throw string("Error: File does not exist");
                    }

                    //load crop
                    Mat subcrop = GEO::GeoImage( act_filename, true ).get_image();
                    
                    if( subcrop.type() != CV_16SC1 )
                        throw string("Image must be CV_16SC1" );
                    //find largest
                    int mx = 0;
                    int I=0, J=0;
                    for( size_t a=0; a<subcrop.cols; a++)
                        for( size_t b=0; b<subcrop.rows; b++)

                            if( subcrop.at<short>(b,a) > mx ){
                                mx = subcrop.at<short>(b,a);
                                I = a;
                                J = b;
                            }
                     cout << "image max is " << mx << " at pos " << I << ", " << J << endl;

                    //get crop range
                    pair<double,double> lat_ran( std::max( std::floor(br.y)+j, br.y ), std::min( std::ceil(br.y)+j, tl.y));
                    pair<double,double> lon_ran( std::max( std::floor(br.x)+i, br.x ), std::min( std::ceil(br.x)+i, tl.x));

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
            
            //compute the expected width of the image
            vector<int> xlens;  xlens.push_back(0);
            for( size_t xx=0; xx<lon_needed; xx++ )
                xlens.push_back( crops[xx].cols + xlens[xx]);

            //compute the expected height of the image
            vector<int> ylens;  ylens.push_back(0);
            for( size_t yy=0; yy<lat_needed; yy++ )
                ylens.push_back( crops[yy*lon_needed].rows + ylens[yy]);

            int cx = 0;
            int cy = 0;
            tile = Mat( Size( xlens.back(), ylens.back()), crops[0].type() );
            tile = Scalar(0);

            for( size_t ii=0; ii<crops.size(); ii++){

                cx = xlens[ii%lon_needed];
                cy = ylens[ii/lon_needed];
                
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
            return;

        }
        else{
            throw std::string("Error: unsupported DEM format");
        }
        */

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

