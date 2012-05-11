#include "TEST_structures.h"

#include <fstream>
#include <iostream>
#include <string> 

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <fstream>
#include <iostream>

#include "gdal_priv.h"
#include "cpl_conv.h"

//GeoImage includes
#include <geoimage/PixelType.h> //GeoImage.h>

using namespace cv;
using namespace std;

bool showImg = false;   /*< Flag for viewing the images */

#include "../unit_test/src/Assert.h"
#include "../unit_test/src/Logger.h"
#include "../useful_stuff/string_functions/string_utilities.h"

int TEST_PixelType_types( string& note );
int TEST_PixelType_accessors( string& note );
int TEST_PixelType_convert( string& note );

/** 
 * Beginning of the structures module test
 *
 * Modules covered
 * - PixelType
*/
void TEST_structures_module(){
    
    //if( fabs(GEOVERSION - 1.0) > 0.00001 ){
    //    cout << "BAD VERSION" << endl;
    //}

    /** Framework variables */
    int result;
    string note;

    /** Print the Test Header for the Structures module */
    print_module_header("structures");

    /**  Test the PixelType Module  */
    print_module_header("PixelType");


    result = TEST_PixelType_types(     note );
    print_test_results( "PixelType  types", result, note );

    result = TEST_PixelType_accessors( note );
    print_test_results( "PixelType  accessors", result, note );
    
    result = TEST_PixelType_convert(   note );
    print_test_results( "PixelType  conversions", result, note );

    
    /** Print the Test Footer for the Structures module */
    print_module_footer("structures");

}

/**
 * Test the PixelType internal types
*/
int TEST_PixelType_types( string& note ){
    
    if( PixelType::UNKNOWN != 0 ){
        note = "UNKNOWN Type Failed";
        return 0;
    }
    if( PixelType::UInt8C1 != 1 ){
        note = "UInt8C1 Failed";
        return 0;
    }
    if( PixelType::UInt16C1 != 2 ){
        note = "UInt16C1 Failed";
        return 0;
    }
    if( PixelType::UInt32C1 != 3 ){
        note = "UInt32C1 Failed";
        return 0;
    }

    note = "Successful Operation";
    return true;
}

/** 
 * Test the getters and setters of the PixelType module
*/
int TEST_PixelType_accessors( string& note ){

    //create a couple of pixeltypes 
    PixelType tp01;
    PixelType tp02( PixelType::UNKNOWN  );
    PixelType tp03( PixelType::UInt8C1  );
    PixelType tp04( PixelType::UInt16C1 );
    PixelType tp05( PixelType::UInt32C1 );

    //test get and set
    note = "Get Operation Failed";
    if( tp01.get() != PixelType::UNKNOWN ) return false;
    if( tp02.get() != PixelType::UNKNOWN ) return false;
    if( tp03.get() != PixelType::UInt8C1 ) return false;
    if( tp04.get() != PixelType::UInt16C1) return false;
    if( tp05.get() != PixelType::UInt32C1) return false;

    note = "Set Operation Failed";
    tp01.set( PixelType::UInt32C1 ); if( tp01.get() != PixelType::UInt32C1 ) return false;
    tp02.set( PixelType::UInt16C1 ); if( tp02.get() != PixelType::UInt16C1 ) return false;
    tp03.set( PixelType::UInt8C1  ); if( tp03.get() != PixelType::UInt8C1  ) return false;
    tp04.set( PixelType::UNKNOWN  ); if( tp04.get() != PixelType::UNKNOWN  ) return false;

    note = "Get Name Operation Failed at: ";
    if( tp01.get_name() != "UInt32C1" ){ note += "UInt32C1"; return false; }
    if( tp02.get_name() != "UInt16C1" ){ note += "UInt16C1"; return false; }
    if( tp03.get_name() != "UInt8C1"  ){ note += "UInt8C1";  return false; }
    if( tp04.get_name() != "UNKNOWN"  ){ note += "UNKNOWN";  return false; }

    note = "Successful Operation";
    return true;
}


int TEST_PixelType_convert( string& note ){

    //create a couple of pixeltypes 
    PixelType tp01;
    PixelType tp02( PixelType::UNKNOWN  );
    PixelType tp03( PixelType::UInt8C1  );
    PixelType tp04( PixelType::UInt16C1 );
    PixelType tp05( PixelType::UInt32C1 );
    

    note = "gdal type conversion failed";
    if( tp01.get_gdal_type() != GDT_Unknown ) return false;
    if( tp02.get_gdal_type() != GDT_Unknown ) return false;
    if( tp03.get_gdal_type() != GDT_Byte    ) return false;
    if( tp04.get_gdal_type() != GDT_UInt16  ) return false;
    if( tp05.get_gdal_type() != GDT_UInt32  ) return false;


    note = "Successful Operation";
    return true;
}

/**
int TEST_NITF_Constructor(string& note);
int TEST_NITF_initialization(string& note);
int TEST_NITF_get_image(string& note);
int TEST_NITF_write_image(string& note);
int TEST_GeoHeader( string& note);
int TEST_PixelType(string& note);

** Test the Geographic Structure Module
 *
void TEST_structure_module() {

    int result;
    string note;

    ** Print the Test Header for the NITF Image Library *
    print_module_header("NITF Image");

    ** Test the GeoImage Constructor for NITF *
    result = TEST_NITF_Constructor(note);
    print_test_results("GeoImage Constructor", result, note);

    result = TEST_NITF_initialization(note);
    print_test_results("GeoImage Initialization", result, note);

    //result = TEST_NITF_get_image(note);
    //print_test_results("GeoImage Get Image", result, note);

    cout << 0 << endl;
    result = TEST_GeoHeader(note);
    print_test_results("GeoHeader Member Functions", result, note);
    
    cout << 1 << endl;
    result = TEST_PixelType(note);
    print_test_results("PixelType _INTERNALS_", result, note);

    result = TEST_NITF_write_image(note);
    print_test_results("GDAL_Data Write Image", result, note);

    ** Print the Test Footer for the NITF Image Library *
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

    cout << "A" << endl;
    //create an image
    string fname01 = "data/24FEB129Z0200702ZXGEO000GS0000004F482008.ntf";
    string fname02 = "data/U_1001A.NTF";
    
    GeoImage img1( fname01, true);
    GeoImage img2( fname02, true);
    
    cout << "pulling header" << endl;

    GeoHeader_Info *hdr = img1.get_header();

    cout << "okay" << endl;

    cout << hdr->get_image_filename() << endl;
    cout << "working" << endl;
    exit(0);

    //query the pixeltype
    PixelType* pix01   = img1.get_header()->get_pixel_type();
    cout << 0 << endl;
    cout << "  " << pix01->get_name() << endl;
    
    PixelType* pix02   = img2.get_header()->get_pixel_type();
    cout << 0 << flush << "  " << pix02->get_name() << endl;
    
    string  filename01 = img1.get_header()->get_image_filename();
    string  filename02 = img2.get_header()->get_image_filename();
    
    cout << "C" << endl;
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

    cout << "A" << endl;
    //create an image
    GeoImage img1("data/24FEB129Z0200702ZXGEO000GS0000004F482008.ntf", true);

    cout << "B" << endl;
    //query the pixeltype
    PixelType* pix = img1.get_header()->get_pixel_type();
    
    cout << "C" << endl;
    
    note = "Not implemented";
    return false;
}

*/
