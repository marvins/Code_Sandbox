/**
 * @file    GDAL_Driver.hpp
 * @author  Marvin Smith
 * @date    3/3/2014
 */
#ifndef __SRC_IO_GDALDRIVER_HPP__
#define __SRC_IO_GDALDRIVER_HPP__

/// C++
#include <iostream>
#include <vector>

/// Boost
#include <boost/filesystem.hpp>
#include <boost/shared_ptr.hpp>

/// GIS Library
#include <src/image/BaseImage.hpp>
#include <src/image/ChannelType.hpp>

/// Geo-Spatial Data-Abstraction Library
#include <gdal/cpl_conv.h>
#include <gdal/gdal_priv.h>


namespace GIS{
namespace IO{
namespace GDAL{

/**
 * @class GDAL_Driver
 */
class GDAL_Driver{

    public:
    
        /// Pointer type for sharing Driver objects
        typedef boost::shared_ptr<GDAL_Driver> ptr_t;
        
        /**
         * Create a driver for loading GDAL Data
         */
        GDAL_Driver( const boost::filesystem::path& image_filename );
        
        /**
         * Destructor
         */
        ~GDAL_Driver();

        /**
         * Check file validity
         */
        bool isValid()const;

        /**
         * Get number of rows
         */
        int getRowCount()const;

        /**
         * Get number of columns
         */
        int getColumnCount()const;

        /**
         * Get image data
         */
        template<typename PIXELTYPE>
        void getImageData( std::vector<PIXELTYPE>& image_data ){
            
            // if the dataset is not open, then do nothing
            if( isOpen() == false ){
                std::cout << "warning: dataset is not open." << std::endl;
                return;
            }

            // make sure the vector is properly sized
            if( image_data.size() != (getRowCount()*getColumnCount())){
                image_data.resize( getRowCount()*getColumnCount());
            }
        
        
            // get image dimensions
            int xsize = getColumnCount();
            int ysize = getRowCount();

            // start iterating through each band
            float* pafScanline;
            double value;

            // get driver type
            std::string driverType;
            if( m_driver->GetDescription() != NULL ){
                driverType = m_driver->GetDescription();
            }

            // set the bit depth if we are nitf
            int NITF_ABPP=16;
            if( driverType == "NITF" ){
                if( m_dataset->GetMetadataItem("NITF_ABPP") != NULL ){
                    std::string abpp = m_dataset->GetMetadataItem("NITF_ABPP");
                    if( abpp == "12" ){
                        NITF_ABPP=12;
                    }
                    else if( abpp == "14" ){
                        NITF_ABPP=14;
                    }
                }
            }


            for( int i=0; i<m_dataset->GetRasterCount(); i++ ){
                
                // create raster band
                GDALRasterBand* band = m_dataset->GetRasterBand(i+1);
                
                // get raster datatype
                int gdalDataType = band->GetRasterDataType();
                
                // iterate over every row
                for( int r=0; r<ysize; r++ ){

                    // initialize data
                    pafScanline = (float*) CPLMalloc(sizeof(float) * xsize);
                    
                    // read data
                    band->RasterIO( GF_Read, 0, r, xsize, 1, pafScanline, xsize, 1, GDT_Float32, 0, 0);

                    

                    //add data to vector
                    for( size_t c=0; c<xsize; c++ ){
                        
                        /// Convert datatypes
                        if( gdalDataType == GDT_Byte ){
                            value = channel_convert<ChannelTypeUInt8,typename PIXELTYPE::channeltype>( pafScanline[c] );
                        } else if( (gdalDataType == GDT_Int16 || gdalDataType == GDT_UInt16 ) || (NITF_ABPP == 12 )){
                            value = channel_convert<ChannelTypeUInt12,typename PIXELTYPE::channeltype>( pafScanline[c] );
                        } else if( gdalDataType == GDT_Int16 || gdalDataType == GDT_UInt16 ){
                            value = channel_convert<ChannelTypeUInt16,typename PIXELTYPE::channeltype>( pafScanline[c] );                        
                        } else {
                            value = pafScanline[c];
                        }


                        if( m_dataset->GetRasterCount() == 1 && image_data.front().dims() == 3 ){
                            image_data[r*xsize + c] = value;
                        }
                        else {
                            image_data[r*xsize + c][i] = value;
                        }
                        

                    }

                    // clear data
                    CPLFree( pafScanline );


                }
            }
        
        }

        /**
         * Open image data
         */
        void open( );
        
        /**
         * Close Dataset
         */
        void close();

        /**
         * Check if the dataset is open
         */
        bool isOpen()const;

    private:

        /// Location of file
        boost::filesystem::path m_path;
        
        /// Valid flag
        bool m_valid;

        /// GDAL Driver
        GDALDriver* m_driver;

        /// GDAL Dataset
        GDALDataset* m_dataset;

};


/**
 * Load an image
 */
template < typename PIXELTYPE >
typename GIS::BaseImage<PIXELTYPE>::ptr_t load_image( boost::filesystem::path const& image_filename ){

    // create the GDAL Object
    GDAL_Driver::ptr_t driver( new GDAL_Driver(image_filename));

    // load the dataset
    driver->open();

    // make sure there were no problems
    if( driver->isValid() == false ){
        std::cout << "error:  Problem loading file." << std::endl;
        return typename BaseImage<PIXELTYPE>::ptr_t( new BaseImage<PIXELTYPE>());

    }

    // Create output object
    typename BaseImage<PIXELTYPE>::ptr_t output( new BaseImage<PIXELTYPE>());
    
    // query file dimensions
    int rows = driver->getRowCount();
    int cols = driver->getColumnCount();
    output->resize( rows, cols);

    // query image data
    driver->getImageData( output->data() );

    
    return output;
}

/**
 * Load an image
 */
template < typename PIXELTYPE >
std::vector<PIXELTYPE>  load_image_data( boost::filesystem::path const& image_filename, int& rows, int& cols ){

    // create the GDAL Object
    GDAL_Driver::ptr_t driver( new GDAL_Driver(image_filename));

    // load the dataset
    driver->open();

    // make sure there were no problems
    if( driver->isValid() == false ){
        std::cout << "error:  Problem loading file." << std::endl;
        rows = 0;
        cols = 0;
        return std::vector<PIXELTYPE>();

    }

    // Create output object
    std::vector<PIXELTYPE> output;

    // query file dimensions
    rows = driver->getRowCount();
    cols = driver->getColumnCount();

    // query image data
    driver->getImageData( output );
    
    return output;
}


/**
 * Write an image
 */
template<typename PIXELTYPE>
void write_image( typename GIS::BaseImage<PIXELTYPE>::ptr_t const& image, const boost::filesystem::path& filename ){

    


}

/**
 * Check if the file has a valid extension
 */
bool isValidExtension( const boost::filesystem::path& filename );

} /// End of GDAL Namespace
} /// End of IO Namespace
} /// End of GIS Namespace


#endif
