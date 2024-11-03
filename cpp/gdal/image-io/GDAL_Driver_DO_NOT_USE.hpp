/**
 *  @file   GDAL_Driver.hpp
 *  @author  Marvin Smith
 *  @date    6/30/2013
 */
#ifndef __SRC_IO_GDAL_DRIVER_HPP__
#define __SRC_IO_GDAL_DRIVER_HPP__

#ifdef DEPLOYED
#include <gis_library/exports.hpp>
#include <gis_library/Enumerations.hpp>
#include <gis_library/GIS_LineString.hpp>
#else
#include <src/exports.hpp>
#include <src/core/Enumerations.hpp>
#include <src/vector/GIS_LineString.hpp>
#endif

/// OpenCV Dependencies
#include <opencv2/core/core.hpp>

/// GDAL Dependencies
#include <cpl_conv.h>
#include <gdal_priv.h>


namespace GIS{

const int GDAL_UNKNOWN = 0;
const int GDAL_GRAY  = 1;
const int GDAL_RED   = 3;
const int GDAL_GREEN = 4;
const int GDAL_BLUE  = 5;
const int GDAL_ALPHA = 6;

/**
 * @class GDAL_Driver
 *
 * Container for extracting image, vector, and elevation data from GIS Containers using GDAL
*/
class  GIS_LIBRARY_API GDAL_Driver{

	public:
		
		/**
		 * Default Constructor
		*/
		GDAL_Driver( );
		
		/**
		 * Standard Constructor. 
		 *
		 * @param[in] filename Input filename to load.
		 * @param[in] init Flag on whether to load imagery yet or not.
		*/
		GDAL_Driver( const std::string& filename, bool const& init = true );
        
        /**
         * Destructor
        */
        ~GDAL_Driver();

		/**
		 * Return the OpenCV Image
		*/
		cv::Mat  getOpenCVMat( const int& pixType );
		cv::Mat  getOpenCVMat( );
        
        
        std::string get_image_format()const;

        /**
         * Set the filename 
        */
        void set_filename( std::string image_filename );
        
        /**
         * Write the image to file
        */
        void write( std::string const& image_filename, int const& format=IMAGE_NONE );

        /**
         * Get the corner coordinates
        */
        GIS_LineString getCornerCoordinates()const;

	//private:
		
		/**
		 * Initialize the driver to use
		*/
		void initialize();
		
		/**
		 * Set the class to its default configuration
		*/
		void set_defaults();
		
		/**
		 * Return the OpenCV Pixel Type
		*/
		int getOpenCVPixelType()const;

        static GDALDataType getGDALPixelType( const int& cvDepth);
		
		/**
		 * Convert the value from a GDAL Pixel Type to the OpenCV Pixel Type
		*/
		double GDAL2OpenCV( double const& val, const int cvtype, const int gdaltype );
		
		/** 
		 * Get the dimensions of the raster object from the driver
		 */
		std::pair<int,int> getRasterDimensions( )const;
		
		/**
		 * Return if the image is available to retrieve
		*/
		bool isValid()const;
		
		/**
		 * Return the number of image channels
		*/
		int get_num_channels()const;
	    

		/**
		 * Flag if the main GDAL Driver has been loaded and initialized
		 * True:  Driver can be used
		 * False:  Driver must be initialize before it can be used.
		*/
		bool driver_initialized;
		
		/**
		 * Filename of the data
		 */
		std::string filename;
		
		/**
		 * GDAL-specific dataset
		 */
		GDALDataset*  dataset;
		
		/**
		 * GDAL-specific driver
		*/
		GDALDriver*  driver;
		
        /**
         * Image Format
        */
        std::string image_format;

};

} // End of Namespace GIS

#endif
