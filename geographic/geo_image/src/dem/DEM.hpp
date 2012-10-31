#ifndef __SRC_STRUCTURES_DEM_H__
#define __SRC_STRUCTURES_DEM_H__

#include <opencv2/core/core.hpp>

#include <string>

#ifdef DEPLOYED
#include <geoimage/core/Enumerations.hpp>
#include <geoimage/coordinates/CoordinateLatLon.hpp>
#else
#include "../core/Enumerations.hpp"
#include "../coordinates/CoordinateLatLon.hpp"
#endif

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
         * Construct a DEM Model given the bounds as well as the parameters required to load data.
        */
        DEM( const cv::Point2f& _min, const cv::Point2f& _max, DEM_Params const& params );

        /**
         * DEM Destructor
        */
        ~DEM();

        cv::Mat get_tile()const;
        void set_tile( const cv::Mat& data );
    
        // return the max elevation
        double max_elevation( cv::Point2f& coord )const;
        double max_elevation( )const;

        // return the specific elevation
        double query_elevation( const cv::Point2f& location )const;
        double query_elevation( const GEO::CoordinateLatLon& coord )const;
        
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
        cv::Point2f m_min; /*< Top Right Corner of Image     */
        cv::Point2f m_max; /*< Bottom Left Corner of Image */
        

}; //end of DEM class

} //end of GEO namespace 

#endif

