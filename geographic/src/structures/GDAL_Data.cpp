#include "GDAL_Data.h"

#include <iostream>

using namespace std;

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

    //create an output dataset
    GDALDataset *outputData;// = driver->Create
    

}
