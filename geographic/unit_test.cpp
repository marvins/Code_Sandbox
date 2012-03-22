/**
   Unit test showing how to use and implement the Geographic Utilities
*/

#include <Assert.h>
#include <Color.h>
#include <Logger.h>

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include "src/structures/GeoImage.h"

using namespace cv;
using namespace std;

void TEST_structure_module();
int  TEST_NITF_Constructor( string& note);
int  TEST_NITF_initialization( string& note);
int  TEST_NITF_get_image( string& note );

int main( int argc, char* argv[] ){

    TEST_structure_module();

    return 0;
}

void TEST_structure_module(){

    int result;
    string note;
    
    print_module_header("NITF Image");
    
    result = TEST_NITF_Constructor(note);
    print_test_results("GeoImage Constructor",result,note);
    
    result = TEST_NITF_initialization(note);
    print_test_results("GeoImage Initialization",result,note);

    result = TEST_NITF_get_image(note);
    print_test_results("GeoImage Get Image",result, note);

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

    //test set init
    note = "Set init fails on set_init call";
    img6.set_init(true);
    if( img6.get_init() == true || img6.get_filename() != ""){ return false; }
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
    if( string(img1.getDriver()->GetDescription()) != "NITF" )
    { note += img1.getDriver()->GetDescription();  return false; }
    
    if( string(img2.getDriver()->GetDescription()) != "NITF" )
    { note += img2.getDriver()->GetDescription();  return false; }
    
    if( string(img3.getDriver()->GetDescription()) != "NITF" )
    { note += img3.getDriver()->GetDescription();  return false; }
    
    if( string(img4.getDriver()->GetDescription()) != "NITF" )
    { note += img4.getDriver()->GetDescription();  return false; }
    
    if( string(img5.getDriver()->GetDescription()) != "NITF" )
    { note += img5.getDriver()->GetDescription();  return false; }
    
    if( string(img6.getDriver()->GetDescription()) != "NITF" )
    { note += img6.getDriver()->GetDescription();  return false; }

    note = "Test successful";
    return true;
}

int  TEST_NITF_get_image( string& note ){

    vector<GeoImage> imgArr(24);
    imgArr[0].set_filename("data/U_1001A.NTF");
    imgArr[1].set_filename("data/U_1005A.NTF");
    imgArr[2].set_filename("data/U_1025A.NTF");
    imgArr[3].set_filename("data/U_1033A.NTF");
    imgArr[4].set_filename("data/U_1034A.NTF");
    imgArr[5].set_filename("data/U_1036A.NTF");
    imgArr[6].set_filename("data/U_1050A.NTF");
    imgArr[7].set_filename("data/U_1051E.NTF");
    imgArr[8].set_filename("data/U_1052A.NTF");
    imgArr[9].set_filename("data/U_1056A.NTF");
    imgArr[10].set_filename("data/U_1060A.NTF");
    imgArr[11].set_filename("data/U_1064A.NTF");
    imgArr[12].set_filename("data/U_2001A.NTF");
    imgArr[13].set_filename("data/U_2001D.NTF");
    imgArr[14].set_filename("data/U_2001E.NTF");
    imgArr[15].set_filename("data/U_2005A.NTF");
    imgArr[16].set_filename("data/U_2010A.NTF");
    imgArr[17].set_filename("data/U_2015A.NTF");
    imgArr[18].set_filename("data/U_2020A.NTF");
    imgArr[19].set_filename("data/U_3001A.NTF");
    imgArr[20].set_filename("data/U_3002A.NTF");
    imgArr[21].set_filename("data/U_3010A.NTF");
    imgArr[22].set_filename("data/U_4003B.NTF");
    imgArr[23].set_filename("data/U_4017A.NTF");

    for(size_t i=0; i<imgArr.size(); i++)
        imgArr[i].init();

    for(size_t i=0; i<imgArr.size(); i++)
        imgArr[i].get_image();

    note = "TEST NOT INITIALIZED";
    return false;

    note = "Test successful";
    return true;
}
