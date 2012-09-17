#include "TEST_structures.h"

#include "GeoImage/TEST_geoimage.h"
#include "HeaderData/TEST_headers.h"

#include <iostream>

/** 
 * Beginning of the structures module test
 *
 * Modules covered
 * - PixelType
*/
void TEST_structures_module(){
   

    /** test header data */
    TEST_headers();
    
    /** test geo image data */
    TEST_geoimage();

}

