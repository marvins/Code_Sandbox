#ifndef __GDAL_DATA_H__
#define __GDAL_DATA_H__


#ifdef DEPLOYED
#include <geoimage/io/GeoLoader.hpp>
#include <geoimage/io/IO_Configuration.hpp>
#else
#include "GeoLoader.hpp"
#include "IO_Configuration.hpp"
#endif 

#include <cpl_conv.h>
#include <gdal_priv.h>

#include <string>
#include <utility>
#include <vector>

#include <boost/scoped_ptr.hpp>

// OpenCV Library
#if USE_OPENCV == 1
#include <opencv2/core/core.hpp>
#endif


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
         * Initialize GDAL Dataset and load information
        */
        void initialize( const std::string& fname );

        /**
         * Get Header Metadata
         *
         * @return header metadata list
        */
        std::vector<std::pair<std::string,std::string> >  get_header_data()const;
        
        
        /**
         * Get Header TRE Metadata
        */
        std::string  get_header_tre()const;
        std::string  get_header_tre( const std::string& tag )const;
        std::string  get_raw_tre( )const;

        /**
         * Returns if data is opencv compatible
        */
        bool isOpenCVCompatible( )const;

        /**
         * Returns if data is valid
        */
        bool isValid( )const;

        /**
         * Returns the number of image channels
        */
        int get_image_channels( )const;
        
        /**
         * Get Raster Dimensions
        */
        std::pair<int,int> getRasterDimensions( )const;

#if USE_OPENCV == 1
        /**
         * Check the opencv pixel type
        */
        int getOpenCVPixelType( )const;
        
        /**
         * Pull out the OpenCV image data.
        */
        cv::Mat getOpenCVMat( const int& pixType )const;

#endif
        
        std::string getImageTypeName() const;

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
        
        bool  valid_data;       /*<  Whether or not the data is currently valid. */
        bool  gdalLoadFailed;   /*<  Whether or not the load operation failed */
        bool  openCVCompatible; /*<  Whether or not the data is compatible with OpenCV */


        double adfMinMax[2];

};

} //end of GEO namespace 

#endif
