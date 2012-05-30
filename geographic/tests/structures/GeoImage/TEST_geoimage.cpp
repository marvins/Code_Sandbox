#include "TEST_geoimage.h"

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <Assert.h>
#include <Logger.h>
#include <string_utilities.h>

#include <GeoImage.h>

using namespace cv;
using namespace std;

int TEST_dted_geoimage( string& note );

void TEST_geoimage(){
    
    int result;
    string note;

    print_module_header("GeoImage");

    result = TEST_dted_geoimage( note );
    print_test_results( "GeoImage    DTED", result, note );
    
    
    print_module_footer("GeoImage");

}


int TEST_dted_geoimage( string& note ){

    //create a geoimage object
    GEO::GeoImage img_dted01("data/n37.dt2", true);
    GEO::GeoImage img_dted02("data/n38.dt2", true);
    GEO::GeoImage img_dted03("data/n39.dt2", true);

    //make sure that the file exists
    if( GEO::GeoHeader_Info::file_exists( img_dted01.get_filename() ) == false ){
        note = "img_dted01 doesn't exist";
        return false;
    }
    if( GEO::GeoHeader_Info::file_exists( img_dted02.get_filename() ) == false ){
        note = "img_dted02 doesn't exist";
        return false;
    }
    if( GEO::GeoHeader_Info::file_exists( img_dted03.get_filename() ) == false ){
        note = "img_dted03 doesn't exist";
        return false;
    }

    //make sure that the images are opencv compatible
    if( img_dted01.isOpenCVValid() == false ){
        note = "img_dted01 failed";
        return false;
    }
    if( img_dted02.isOpenCVValid() == false ){
        note = "img_dted02 failed";
        return false;
    }
    if( img_dted03.isOpenCVValid() == false ){
        note = "img_dted03 failed";
        return false;
    }

    //pull images
    Mat img01 = img_dted01.get_image();
    Mat img02 = img_dted02.get_image();
    Mat img03 = img_dted03.get_image();
    
    note = "Successful Operation";
    return true;
}
