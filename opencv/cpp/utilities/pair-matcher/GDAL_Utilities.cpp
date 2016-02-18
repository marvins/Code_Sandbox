/**
 * @file    GDAL_Utilities.cpp
 * @author  Marvin Smith
 * @date    2/18/2016
 */
#include "GDAL_Utilities.hpp"

// C++ Libraries
#include <iostream>

// GDAL Libraries
#include <gdal.h>
#include <gdal_priv.h>


/***************************************************/
/*          Load Image from GDAL to OpenCV         */
/***************************************************/
cv::Mat Load_Image_GDAL_To_OpenCV( const std::string& pathname,
                                   double*&           geo_transform )
{
     // Open memory buffer for read.
     GDALDataset* dataset = (GDALDataset*)GDALOpen( pathname.c_str(), 
                                                    GA_ReadOnly ); 

    // Set the parameters
    if( dataset == NULL ){
        std::cout << "Unable to parse GDAL raster" << std::endl;
        return cv::Mat();
    }
    
    // Get the geo-transform
    dataset->GetGeoTransform( geo_transform );
    
    // Update outputs
    int output_cols     = dataset->GetRasterXSize();
    int output_rows     = dataset->GetRasterYSize();
    int output_channels = dataset->GetRasterCount();
    cv::Mat output_image( output_rows,
                          output_cols,
                          CV_8UC(output_channels));


    // Iterate over the bands
    uint8_t* scanline = new uint8_t[output_cols];
    int channel;
    int X;
    for( int bid=1; bid <= output_channels; bid++ )
    {
        // Get the band
        GDALRasterBand* band = dataset->GetRasterBand(bid);
        channel = bid-1;

        // Iterate over each row
        for( int r=0; r<output_rows; r++ )
        {

            // Fetch the buffer
            band->RasterIO( GF_Read, 0, r, 
                            output_cols,
                            1, 
                            scanline,
                            output_cols,
                            1, 
                            GDT_Byte,
                            0,
                            0);
        
            // Copy
            for( int c=0; c<output_cols; c++ )
            {
                
                X=channel;
                //if( X == 0 ){ X=2; }
                //if( X == 2 ){ X=0; }

                if( output_channels == 1 ){
                    output_image.at<uchar>(r,c) = scanline[c];
                }
                else if( output_channels == 3){
                    output_image.at<cv::Vec3b>(r,c)[X] = scanline[c];
                }
                else{
                    output_image.at<cv::Vec4b>(r,c)[X] = scanline[c];
                }
            }
        }
    }

    // Clean up
    delete [] scanline;
 
    return output_image;
}

