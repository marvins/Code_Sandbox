#ifndef __SRC_STRUCTURES_DEM_H__
#define __SRC_STRUCTURES_DEM_H__

#include <opencv2/core/core.hpp>

#include <string>

namespace GEO{

/**
  * @class DEM_Params
  *
  * Typical parameters for the Digital Elevation Model
  * object.
  *
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
        //DEM( double const& tl_lat, double const& tl_lon, double const& br_lat, double const& br_lon, DEM_Params const&  params );
        //DEM( cv::Point2f const& pnt, DEM_Params const& params );
        
        /**
         * DEM Default Constructor
        */
        DEM( );
    
        /**
         * DEM Parameterized Constructor
         *
         * @param[in] _tl Top Left Corner
         * @param[in] _br Bottom Right Corner
         * @param[in] data DEM Data to set the tile to.
        */
        DEM( const cv::Point2f& _tl, const cv::Point2f& _br, const cv::Mat& data );

        /**
         * DEM Destructor
        */
        ~DEM();

        cv::Mat get_tile()const;
    
        //double max_elevation( double& lat, double& lon )const;
        
        //double get_elevation()const;

        //cv::Mat relief_map()const;
        
        //cv::Point get_pixel_coordinate( cv::Point2f const& coordinate );
        
        // get and set functions
        cv::Point2f ne()const;
        cv::Point2f nw()const;
        cv::Point2f se()const;
        cv::Point2f sw()const;

    private:
        cv::Mat tile;   /*< Loaded Image Tile            */
        cv::Point2f m_tl; /*< Top Left Corner of Image     */
        cv::Point2f m_br; /*< Bottom Right Corner of Image */
        

}; //end of DEM class

} //end of GEO namespace 

#endif

