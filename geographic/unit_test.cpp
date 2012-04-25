/**
   Unit test showing how to use and implement the Geographic Utilities
 */

#include <fstream>
#include <iostream>
#include <string> 

#include "../unit_test/src/Assert.h"
#include "../unit_test/src/Logger.h"
#include "../useful_stuff/string_functions/string_utilities.h"

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <fstream>
#include <iostream>

#include "src/structures/GeoImage.h"
#include "src/structures/GDAL_Data.h"
#include "src/structures/NITFHeader.h"
#include "src/structures/GeoHeader.h"

using namespace cv;
using namespace std;

bool showImg = false;

void TEST_structure_module();

int TEST_NITF_Constructor(string& note);
int TEST_NITF_initialization(string& note);
int TEST_NITF_get_image(string& note);
int TEST_NITF_write_image(string& note);
int TEST_GeoHeader( string& note);
int TEST_PixelType(string& note);

/** Main Driver 
 *
 * @brief Entrance to the Unit Test
 */
int main(int argc, char* argv[]) {

    try {
        //begin structure module test
        TEST_structure_module();
    }    catch (string e) {
        cout << e << endl;
    }
    //end of unit tests
    return 0;
}

/** Test the Geographic Structure Module
 */
void TEST_structure_module() {

    int result;
    string note;

    /** Print the Test Header for the NITF Image Library */
    print_module_header("NITF Image");

    /** Test the GeoImage Constructor for NITF */
    result = TEST_NITF_Constructor(note);
    print_test_results("GeoImage Constructor", result, note);

    result = TEST_NITF_initialization(note);
    print_test_results("GeoImage Initialization", result, note);

    //result = TEST_NITF_get_image(note);
    //print_test_results("GeoImage Get Image", result, note);

    result = TEST_GeoHeader(note);
    print_test_results("GeoHeader Member Functions", result, note);
    
    result = TEST_PixelType(note);
    print_test_results("PixelType _INTERNALS_", result, note);

    result = TEST_NITF_write_image(note);
    print_test_results("GDAL_Data Write Image", result, note);

    /** Print the Test Footer for the NITF Image Library */
    print_module_footer("NITF Image");

}

int TEST_NITF_Constructor(string& note) {

    GeoImage img1("data/U_1001A.NTF");
    GeoImage img2("data/U_1005A.NTF", false);
    GeoImage img3("data/U_1025A.NTF", true);
    GeoImage img4("data/U_1033A.NTF", true);
    GeoImage img5("data/U_1034A.NTF", true);
    GeoImage img6;

    //test init function 
    note = "Initialization string improperly initialized on img";
    if (img1.get_init() == true) {
        note += "1";
        return false;
    }
    if (img2.get_init() == true) {
        note += "2";
        return false;
    }
    if (img3.get_init() == false) {
        note += "3";
        return false;
    }
    if (img4.get_init() == false) {
        note += "4";
        return false;
    }
    if (img5.get_init() == false) {
        note += "5";
        return false;
    }
    if (img6.get_init() == true) {
        note += "6";
        return false;
    }

    //test set init function
    note = "Set init fails on set_init call";
    img6.set_init(true);
    if (img6.get_init() == true || img6.get_filename() != "_NO_IMAGE_SELECTED_") {
        return false;
    }

    //give it valid filename and try again
    img6.set_filename("data/U_1036A.NTF");
    img6.set_init(true);
    if (img6.get_init() != true || img6.get_filename() != "data/U_1036A.NTF")
        return false;

    note = "Test successful";
    return true;
}

int TEST_NITF_initialization(string& note) {

    GeoImage img1("data/U_1001A.NTF", true);
    GeoImage img2("data/U_1005A.NTF", true);
    GeoImage img3("data/U_1025A.NTF", true);
    GeoImage img4("data/U_1033A.NTF", true);
    GeoImage img5("data/U_1034A.NTF", true);
    GeoImage img6("data/U_1036A.NTF", true);


    //test set init
    note = "Driver short description not NITF, actual: ";
    if (string(img1.getImageTypeName()) != "NITF") {
        note += img1.getImageTypeName();
        return false;
    }

    if (string(img2.getImageTypeName()) != "NITF") {
        note += img2.getImageTypeName();
        return false;
    }

    if (string(img3.getImageTypeName()) != "NITF") {
        note += img3.getImageTypeName();
        return false;
    }

    if (string(img4.getImageTypeName()) != "NITF") {
        note += img4.getImageTypeName();
        return false;
    }

    if (string(img5.getImageTypeName()) != "NITF") {
        note += img5.getImageTypeName();
        return false;
    }

    if (string(img6.getImageTypeName()) != "NITF") {
        note += img6.getImageTypeName();
        return false;
    }

    note = "Test successful";
    return true;
}

int show_image(const string& fname, int i, string& note) {

    //create opencv structures
    Mat currentImg;
    namedWindow(fname.c_str(), 0);
    GeoImage img(fname, true);

    if (img.isOpenCVValid()) {
        currentImg = img.get_image();

        Size sz = img.getMatSize();
        if (i == 0 || i == 1 || i == 2 || i == 3 || i == 4) {
            if (currentImg.cols != 8192 || currentImg.rows != 8192) {
                note = string("Dimensions are off on image ") + int2str(i);
                return false;
            }
            if (sz.width != 8192 || sz.height != 8192) {
                note = string("Dimensions are off on image ") + int2str(i);
                return false;
            }
            if (img.getMin() != 0 || img.getMax() != 16383) {
                note = string("Pixel range is off on image ") + int2str(i)
                        + string(" should be 0 - 16383 and is ") + int2str(img.getMin())
                        + string(" and ") + int2str(img.getMax());
                return false;
            }
        }


        if (showImg == true) {
            imshow(fname.c_str(), currentImg);
            cvResizeWindow(fname.c_str(), 500, 500);
            waitKey(0);
        }

    }

    destroyAllWindows();
    return true;
}

int TEST_NITF_get_image(string& note) {

    try {

        //open run list
        ifstream fin;
        fin.open("data/run_file.txt");
        string fname;
        fin >> fname;
        int i = 0;
        int result = 1;
        while (!fin.eof()) {
            fin >> fname;
            result = show_image(fname, i++, note);

            if (result != true)
                return result;
        }
        fin.close();

        note = "Test Success";
        return true;
    } catch (string e) {
        note = "test failed, threw exception: ";
        note += e;
        return false;
    }
}

int TEST_NITF_write_image(string& note) {

    cout << "start of write image test" << endl;
    ///create input and output image filename
    string output_filename = "gdaltest.ntf";
    string input_filename = "data/24FEB129Z0200702ZXGEO000GS0000004F482008.ntf";

    //create 
    GeoImage img1(input_filename, true);

    Mat imageTest;
    GaussianBlur( img1.get_image(), imageTest, Size(11,11), 2);
    
    if( img1.isOpenCVValid() == true ){
        
        //write data
        GDAL_Data::write(output_filename,
                 imageTest,//img1.get_image(),
                 img1.get_header());
    }
    else{
        note = "Image load failed";
        return false;
    }

    note = "Successful Operation";
    return true;

}

int TEST_GeoHeader(string& note) {

    //create an image
    string fname01 = "data/24FEB129Z0200702ZXGEO000GS0000004F482008.ntf";
    string fname02 = "data/U_1001A.NTF";
    
    GeoImage img1( fname01, true);
    GeoImage img2( fname02, true);
    
    //query the pixeltype
    PixelType* pix01   = img1.get_header()->get_pixel_type();
    PixelType* pix02   = img2.get_header()->get_pixel_type();
    string  filename01 = img1.get_header()->get_image_filename();
    string  filename02 = img2.get_header()->get_image_filename();
    
    if( filename01 != fname01 || filename02 != fname02 ){
        note = "Filename is not transferrin properly";
        return false;
    }
    if( pix01->get_name() != "UInt16" || pix02->get_name() != "UInt8"){
        note = "Pixeltype is not transferring correctly";
        return false;
    }
    if( img1.get_header()->get_driver_format() != "NITF"){
        note = "Driver format is not transferring correctly";
        return false;
    }
    
    note = "Successful Operation";
    return true;
}

int TEST_PixelType(string& note) {

    //create an image
    GeoImage img1("data/24FEB129Z0200702ZXGEO000GS0000004F482008.ntf", true);

    //query the pixeltype
    PixelType* pix = img1.get_header()->get_pixel_type();
    
    
    note = "Not implemented";
    return false;
}
