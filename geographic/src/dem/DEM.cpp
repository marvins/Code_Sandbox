#include "DEM.h"
#include "../core/Enumerations.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <utility>

using namespace std;

namespace GEO{
        
DEM_Params::DEM_Params( const int& ftype, std::string const& root_dir ){

    filetype      = ftype;
    dted_root_dir = root_dir;
}

DEM::DEM( double const& tl_lat, double const& tl_lon, double const& br_lat, double const& br_lon, DEM_Params const& params ){

    cout << "Created DEM" << endl;
    
    /** need to start looking at how many files we need
     * 
     * 1.  DTED uses 1 deg x 1 deg grids
    */
    if( params.filetype == DTED ){
        cout << "loading dted" << endl;

        /**  there are a few circumstances where we need to consider 
         *   1.  The region is completely inside a tile
         *      - Just pull a sub-region of the image
         *   2.  The region straddles the edge of multiple tiles
         *      - find intersection boundaries
        */
        pair<double,double> tl_lat_range( floor(tl_lat), ceil(tl_lat));
        pair<double,double> tl_lon_range( floor(tl_lon), ceil(tl_lon));
        pair<double,double> br_lat_range( floor(br_lat), ceil(br_lat));
        pair<double,double> br_lon_range( floor(br_lon), ceil(br_lon));
        
        //compute spreads of the latitude
        double lat_min = std::min( tl_lat_range.first, br_lat_range.first);
        double lat_max = std::max( tl_lat_range.second, br_lat_range.second);
        double lon_min = std::min( tl_lon_range.first, br_lon_range.first);
        double lon_max = std::max( tl_lon_range.second, br_lon_range.second);
        int lat_dir, lon_dir;
        
        cout << "lat: " << lat_max << ", " << lat_min << endl;
        cout << "lon: " << lon_max << ", " << lon_min << endl;
        if( lat_max - lat_min  > 1 ){  // we have multiple files
            throw string("Currently not supported");
        }
        else if( lat_max - lat_min < 0 )
            throw string("ERROR: invalid option");
        else{  //lat covers one file
            //get the proper file name
            lat_dir = lat_min;
        }

        if( lon_max - lon_min > 1){ // we have multiple files
            throw string("Currently not supported");
        }
        else if( lon_max - lon_min < 0 ) 
            throw string("ERROR: invalid option");
        else{
            lon_dir = lon_min;
        }
        cout << "File should be " << lat_dir << ", " << lon_dir << endl;
    }
    else{
        throw std::string("Error: unsupported DEM format");
    }


}




}//end of GEO namespace
