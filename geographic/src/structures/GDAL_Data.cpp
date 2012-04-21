#include "GDAL_Data.h"

/** Create the GDAL Dataset
 *
 * @brief This initializes all GDAL Dataset objects to NULL
*/
GDAL_Data::GDAL_Data(){

    driver = NULL;
    dataset = NULL;
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
