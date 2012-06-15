#include <boost/filesystem.hpp>

#include "DEM.h"
#include "../core/Enumerations.h"
#include "../structures/GeoImage.h"
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

            //compute range
            double minx = min( tl_lon, br_lon);  double maxx = max( tl_lon, br_lon);
            double miny = min( tl_lat, br_lat);  double maxy = max( tl_lat, br_lat);
            
            int range_minx = (minx - std::floor(tl_lon))*cv_tile.cols;
            int range_maxx = (1-(std::ceil(tl_lon)  - maxx))*cv_tile.cols;
            int range_miny = (miny - std::floor(tl_lat))*cv_tile.rows;
            int range_maxy = (1-(std::ceil(tl_lat)  - maxy))*cv_tile.rows;
            
            cv::Mat crop( cv::Size( range_maxx - range_minx, range_maxy - range_miny ), cv_tile.type());
            if( cv_tile.type() == CV_8UC1  ) cout << "CV_8UC1"  << endl;
            if( cv_tile.type() == CV_8UC3  ) cout << "CV_8UC3"  << endl;
            if( cv_tile.type() == CV_16UC1 ) cout << "CV_16UC1" << endl;
            if( cv_tile.type() == CV_16SC1 ) cout << "CV_16SC1" << endl;
            if( cv_tile.type() == CV_32SC1 ) cout << "CV_32SC1" << endl;
            for( int x=range_minx; x< range_maxx; x++ )
                for( int y=range_miny; y< range_maxy; y++ ){
                    if( cv_tile.type() == CV_8UC1 )
                        crop.at<uchar>(y-range_miny, x-range_minx) = cv_tile.at<uchar>(y, x);
                    else if( cv_tile.type() == CV_16UC1 )
                        crop.at<unsigned short>(y-range_miny, x-range_minx) = cv_tile.at<unsigned short>(y, x);
                    else if( cv_tile.type() == CV_16SC1 ){
                        crop.at<short>(y-range_miny, x-range_minx) = cv_tile.at<short>(y, x);
                    }
                    else if( cv_tile.type() == CV_8UC3 )
                        crop.at<Vec3b>(y-range_miny, x-range_minx) = cv_tile.at<Vec3b>(y, x);
                    else 
                        throw string("ERROR: unsupported pixel type");
                }
            
            tile = crop;
        }
        else{

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
