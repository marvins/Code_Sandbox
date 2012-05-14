#include "GDAL_Data.h"

#include <iostream>

using namespace std;

namespace GEO{

/** Create the GDAL Dataset
 *
 * @brief This initializes all GDAL Dataset objects to NULL
*/
GDAL_Data::GDAL_Data(){

    driver = NULL;
    dataset = NULL;
    gdalLoadFailed = false;
}

/** Destroy the GDAL Data object
 *
 * @brief This will close the dataset and delete all allocated
 * gdal-specific memory
*/
GDAL_Data::~GDAL_Data(){

    if( dataset != NULL )
        GDALClose( dataset );

}

/** Write an image to a NITF Format
 *
*/
void GDAL_Data::write( std::string const& image_filename, std::string const& image_format ){

    //currently the only supported format is NITF
    if( image_format != "NITF" ){
        cout << "invalid format, exiting" << endl;
        return;
    }
    
    /// This is where we should set any internal flags
    char ** papszOptions = NULL;

    //create an output dataset
    //GDALDataset *outputData = driver->Create( image_filename.c_str(), 
    //        dataset->GetRasterXSize(), dataset->GetRasterYSize(),
    //        dataset->GetRasterCount(), GDALGetRasterDataType(dataset->GetRasterBand(1)),
    //        papszOptions);
    
    //copy an output dataset
    GDALDataset *outputData = driver->CreateCopy( image_filename.c_str(), 
            dataset, FALSE, papszOptions, NULL, NULL);
    
    //load dataset with appropriate metadata
    
    
    
    //close dataset and flush data to file
    GDALClose((GDALDatasetH) outputData);
    

}

void GDAL_Data::write( std::string const& image_filename, cv::Mat const& image, GeoHeader_Info* header_data){
    
    /// This is where we should set any internal flags
    char ** papszOptions = NULL;
    GDALDriver* oDriver = GetGDALDriverManager()->GetDriverByName(header_data->get_driver_format().c_str());
    
    //create an output dataset
    GDALDataset *outputData = oDriver->Create( image_filename.c_str(), 
            image.cols, image.rows, image.channels(), header_data->get_pixel_type().get_gdal_type(),
            papszOptions);
    
    GDALRasterBand* band;


    //load dataset with appropriate metadata
    for( int c=0; c<image.channels(); c++){
        
        //fetch band
        band = outputData->GetRasterBand(c+1);
        
        for( int y=0; y<image.rows; y++){
            for( int x=0; x<image.cols; x++){
                int* value = new int;
                
                *value  = header_data->get_pixel_type().convert( image, x, y, c);

                band->RasterIO( GF_Write, x, y, 1, 1, value, 
                        1, 1, header_data->get_pixel_type().get_gdal_type(), 0, 0);
                
                delete value;
            }
        }
        
    }
    
    //close dataset and flush data to file
    GDALClose((GDALDatasetH) outputData);
    

}
} //end of GEO namespace 
