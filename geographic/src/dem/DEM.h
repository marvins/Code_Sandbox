#ifndef __SRC_STRUCTURES_DEM_H__
#define __SRC_STRUCTURES_DEM_H__

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <string>

namespace GEO{

/**
  * @class DEM_Params
  *
  * Typical parameters for the Digital Elevation Model
  * object.
*/
class DEM_Params{
    
    public:
        DEM_Params( const int& ftype, std::string const& root_dir );

        int filetype;
        std::string dted_root_dir;

};

/**
 * @class DEM 
 *
 * Digital Elevation Model object
 */
class DEM{

    public:
        DEM( double const& tl_lat, double const& tl_lon, double const& br_lat, double const& br_lon, DEM_Params const&  params );
        DEM( double const& lat, double const& lon, DEM_Params const& params );
    
        cv::Mat get_raw()const;

    private:
        cv::Mat tile;

}; //end of DEM class

} //end of GEO namespace 

#endif

