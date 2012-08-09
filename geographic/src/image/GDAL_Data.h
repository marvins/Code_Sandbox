#ifndef __GDAL_DATA_H__
#define __GDAL_DATA_H__

#include <opencv2/core/core.hpp>

#include "NITFHeader.h"
#include "GeoHeader.h"

#include "gdal_priv.h"
#include "cpl_conv.h"

#include <string>
#include <utility>
#include <vector>

namespace GEO{

/** 
 * @class GDAL_Data
 *
 * @brief Container for GDAL Header information
*/
class GDAL_Data{

   public:

       /** Default Constructor */
       GDAL_Data();

       /** Destructor */
        ~GDAL_Data();

        /** Write to output */
        void write( std::string const& outputFilename, std::string const& output_format );
        static void write( std::string const& outputFilename, cv::Mat const& image, GeoHeader_Info* header_data);
        
        void get_corner_coordinates( double& ul_lat, double& ul_lon, double& br_lat, double& br_lon )const;

        std::vector<std::pair<std::string,std::string> >  retrieve_header_data()const;
        
        void set_img_data( const cv::Mat& img );

        bool get_status()const;

        GDALDriver  *driver;    /*<  Driver Object */
        GDALDataset *dataset;   /*<  Dataset Object */
        bool  gdalLoadFailed;   /*<  Whether or not the load operation failed */

        double adfMinMax[2];

};

} //end of GEO namespace 

#endif
