/**
   Unit test showing how to use and implement the Geographic Utilities
*/

#include <fstream>
#include <iostream>
#include <string> 

#include "../unit_test/src/Assert.h"
#include "../unit_test/src/Logger.h"

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <fstream>
#include <iostream>

#include "src/structures/GeoImage.h"


using namespace cv;
using namespace std;

void TEST_structure_module();
int  TEST_NITF_Constructor( string& note);
int  TEST_NITF_initialization( string& note);
int  TEST_NITF_get_image( string& note );
int TEST_NITF_write_image( string& note );

/** Main Driver 
 *
 * @brief Entrance to the Unit Test
*/
int main( int argc, char* argv[] ){

    //begin structure module test
    TEST_structure_module();

    //end of unit tests
    return 0;
}

/** Test the Geographic Structure Module
*/
void TEST_structure_module(){

    int result;
    string note;
  
    /** Print the Test Header for the NITF Image Library */
    print_module_header("NITF Image");
    
    /** Test the GeoImage Constructor for NITF */
    result = TEST_NITF_Constructor(note);
    print_test_results("GeoImage Constructor",result,note);
    
    result = TEST_NITF_initialization(note);
    print_test_results("GeoImage Initialization",result,note);

    result = TEST_NITF_get_image(note);
    print_test_results("GeoImage Get Image",result, note);
    
    result = TEST_NITF_write_image(note);
    print_test_results("GeoImage Write Image",result, note);

    /** Print the Test Footer for the NITF Image Library */
    print_module_footer("NITF Image");

}

int  TEST_NITF_Constructor( string& note){
    
    GeoImage img1("data/U_1001A.NTF");
    GeoImage img2("data/U_1005A.NTF", false);
    GeoImage img3("data/U_1025A.NTF", true );
    GeoImage img4("data/U_1033A.NTF", true );
    GeoImage img5("data/U_1034A.NTF", true );
    GeoImage img6;
   
    //test init function 
    note = "Initialization string improperly initialized on img";
    if( img1.get_init() == true  ){ note += "1"; return false; }
    if( img2.get_init() == true  ){ note += "2"; return false; }
    if( img3.get_init() == false ){ note += "3"; return false; }
    if( img4.get_init() == false ){ note += "4"; return false; }
    if( img5.get_init() == false ){ note += "5"; return false; }
    if( img6.get_init() == true  ){ note += "6"; return false; }
    
    //test set init function
    note = "Set init fails on set_init call";
    img6.set_init(true);
    if( img6.get_init() == true || img6.get_filename() != "_NO_IMAGE_SELECTED_"){ return false; }
  
    //give it valid filename and try again
    img6.set_filename("data/U_1036A.NTF");
    img6.set_init(true);
    if( img6.get_init() != true || img6.get_filename() != "data/U_1036A.NTF" )
        return false;
    
    note = "Test successful";
    return true;
}

int  TEST_NITF_initialization( string& note){

    GeoImage img1("data/U_1001A.NTF", true );
    GeoImage img2("data/U_1005A.NTF", true );
    GeoImage img3("data/U_1025A.NTF", true );
    GeoImage img4("data/U_1033A.NTF", true );
    GeoImage img5("data/U_1034A.NTF", true );
    GeoImage img6("data/U_1036A.NTF", true );

    
    //test set init
    note = "Driver short description not NITF, actual: ";
    if( string(img1.getImageTypeName()) != "NITF" )
    { note += img1.getImageTypeName();  return false; }
    
    if( string(img2.getImageTypeName()) != "NITF" )
    { note += img2.getImageTypeName();  return false; }
    
    if( string(img3.getImageTypeName()) != "NITF" )
    { note += img3.getImageTypeName();  return false; }
    
    if( string(img4.getImageTypeName()) != "NITF" )
    { note += img4.getImageTypeName();  return false; }
    
    if( string(img5.getImageTypeName()) != "NITF" )
    { note += img5.getImageTypeName();  return false; }
    
    if( string(img6.getImageTypeName()) != "NITF" )
    { note += img6.getImageTypeName();  return false; }

    note = "Test successful";
    return true;
}

void show_image(const string& fname, int i){

    //create opencv structures
    Mat currentImg;
    namedWindow(fname.c_str(),0);
    GeoImage img(fname, true);


    bool showImg = true;
    cout << "Image " << i << endl;
    
    if( img.isOpenCVValid() ){    
        currentImg = img.get_image();

        Size sz = img.getMatSize();
        cout << "min: " << img.getMin() << ", max: " << img.getMax() << endl;
        cout << "sz: " << sz.width << ", " << sz.height << endl;

        if( showImg == true ){
            imshow(fname.c_str(),currentImg);
            cvResizeWindow(fname.c_str(), 500, 500);
            waitKey(0);
        }

    }

    destroyAllWindows();
}


int  TEST_NITF_get_image( string& note ){

    try{

        //open run list
        ifstream fin;
        fin.open("data/run_file.txt");
        string fname;
        fin >> fname;
        int i=0;
        while( !fin.eof()){
            fin >> fname;
            show_image(fname, i++);
        }
        fin.close();

        note = "test success";
        return true;
    }
    catch(string e){
        note = "test failed, threw exception: ";
        note += e;
        return false;
    }
}

int TEST_NITF_write_image( string& note ){


    note = "TEST NOT INITIALIZED";
    return false;


}
