#ifndef __GDAL_DATA_H__
#define __GDAL_DATA_H__

#include "gdal_priv.h"
#include "cpl_conv.h"

/** 
 * @class GDAL_Data
 *
 * @brief Container for GDAL Header information
*/
class GDAL_Data{
       
       /** Default Constructor */
       GDAL_Data();

       /** Destructor */
        ~GDAL_Data();

        GDALDriver  *driver;    /*<  Driver Object */
        GDALDataset *dataset;   /*<  Dataset Object */

};

#endif
