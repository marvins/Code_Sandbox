#ifndef __GDAL_DATA_H__
#define __GDAL_DATA_H__

#include "gdal_priv.h"
#include "cpl_conv.h"

#include <string>

enum PIXELTYPE{
   NITF = 123 
};

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

        GDALDriver  *driver;    /*<  Driver Object */
        GDALDataset *dataset;   /*<  Dataset Object */
        bool  gdalLoadFailed;   /*<  Whether or not the load operation failed */

        double adfMinMax[2];

};

#endif
