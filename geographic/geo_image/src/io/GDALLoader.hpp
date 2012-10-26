#ifndef __GDAL_DATA_H__
#define __GDAL_DATA_H__


#ifdef DEPLOYED
#include <geoimage/io/GeoLoader.hpp>
#else
#include "GeoLoader.hpp"
#endif 

#include <cpl_conv.h>
#include <gdal_priv.h>

#include <string>
#include <utility>
#include <vector>

#include <boost/scoped_ptr.hpp>

namespace GEO{



/** 
 * @class GDALLoader
 *
 * @brief Container Loading imagery and other data using GDAL
 */
class GDALLoader : public GeoLoader {

   public:

       /** 
        * Default Constructor 
        *
        * @brief Creates an empty container for usage
        */
       GDALLoader();


        /**
         * Parameterized Constructor
         *
         * @param[in] fname Filename of object to load
        */
        GDALLoader( const std::string& fname );


        /** Destructor */
        virtual ~GDALLoader();


        /**
         * Get Header Metadata
         *
         * @return header metadata list
        */
        std::vector<std::pair<std::string,std::string> >  get_header_data()const;


    private:
        
        /** Write to output */
        //void write( std::string const& outputFilename, std::string const& output_format );
        //static void write( std::string const& outputFilename, cv::Mat const& image, GeoHeader_Info* header_data);
        
        //void get_corner_coordinates( double& ul_lat, double& ul_lon, double& br_lat, double& br_lon )const;

        
        //void set_img_data( const cv::Mat& img );

        //bool get_status()const;

        //void clean();

        GDALDriver*    driver;    /*<  Driver Object */
        GDALDataset*   dataset;   /*<  Dataset Object */
        bool  gdalLoadFailed;   /*<  Whether or not the load operation failed */
        bool  openCVCompatible; /*<  Whether or not the data is compatible with OpenCV */

        //double adfMinMax[2];

};

} //end of GEO namespace 

#endif
