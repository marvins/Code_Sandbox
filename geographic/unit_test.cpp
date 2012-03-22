/**
   Unit test showing how to use and implement the Geographic Utilities
*/

#include <Assert.h>
#include <Color.h>
#include <Logger.h>

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include "src/structures/NITF_Image.h"

using namespace cv;
using namespace std;

void TEST_structure_module();
int  TEST_NITF_Constructor( string& note);

int main( int argc, char* argv[] ){

    TEST_structure_module();

    return 0;
}

void TEST_structure_module(){

    int result;
    string note;
    
    print_module_header("NITF Image");
    
    result = TEST_NITF_Constructor(note);
    print_test_results("NITF_Image Constructor",result,note);

    print_module_footer("NITF Image");

}

int  TEST_NITF_Constructor( string& note){

    NITF_Image img1("data/U_1001A.NTF");
    NITF_Image img2("data/U_1005A.NTF");

    note = "Constructor test not implemented";
    return false;
}
