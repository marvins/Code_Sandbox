#include "TEST_structures.h"

#include "GeoImage/TEST_geoimage.h"
#include "HeaderData/TEST_headers.h"

#include <fstream>
#include <iostream>
#include <string> 

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <fstream>
#include <iostream>

#include <gdal_priv.h>
#include <cpl_conv.h>

//GeoImage includes
#include <GeoImage.h> 


bool showImg = false;   /*< Flag for viewing the images */

#include <Assert.h>
#include <Logger.h>
#include <string_utilities.h>

using namespace cv;
using namespace std;

/** 
 * Beginning of the structures module test
 *
 * Modules covered
 * - PixelType
*/
void TEST_structures_module(){
   

    if( fabs(GEOVERSION - 1.0) > 0.00001 ){
        cout << "BAD VERSION" << endl;
    }

    /** test header data */
    TEST_headers();
    
    /** test geo image data */
    TEST_geoimage();

}

