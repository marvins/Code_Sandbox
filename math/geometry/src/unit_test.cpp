#include <vector>

#include "Vector.h"
#include "Quaternion.h"

#include "../../../unit_test/src/Assert.h"
#include "../../../unit_test/src/Logger.h"

using namespace std;

void TEST_Vector_Module( );
int  TEST_Vector_Constructor( string& note );
int  TEST_Vector_Accessors( string& note );

void TEST_Quaternion_Module( );

int main( int argc, char* argv[] ){

    
    TEST_Vector_Module();
    
    TEST_Quaternion_Module();
	
    return 0;
}


void TEST_Vector_Module( ){

    int result;
    string note;

    print_module_header("Vector");

    result = TEST_Vector_Constructor(note);
    print_test_results( "Vector Constructor", result, note);
    
    result = TEST_Vector_Accessors(note);
    print_test_results( "Vector Accessors", result, note);
    
    print_module_footer("Vector");

}

int TEST_Vector_Constructor(string& note){

    vector<double> vB;
    vector<int> vC;
    vector<size_t> vD;

    vB.push_back(1.1); vB.push_back(-2);
    vC.push_back(1.1); vC.push_back(-2); vC.push_back(3);
    vD.push_back(1.1); vD.push_back( 0); vD.push_back(2); vD.push_back(3);

    Vector<double,99> vecA;
    Vector<double,2> vecB(vB);
    Vector<int,3>    vecC(vC);
    Vector<size_t,4> vecD(vD);

    note = "Vector constructor does not set given input parameters.";
    for(size_t i=0; i<99; i++)
        if( ASSERT_EQUAL( vecA[i], 0, 0.00001 ) == false )
            return false;

    for(size_t i=0; i<vB.size(); i++)
        if( ASSERT_EQUAL( vecB[i], vB[i], 0.00001 ) == false )
            return false;
    
    for(size_t i=0; i<vC.size(); i++)
        if( ASSERT_EQUAL( vecC[i], vC[i], 0.00001 ) == false )
            return false;
    
    for(size_t i=0; i<vD.size(); i++)
        if( ASSERT_EQUAL( vecD[i], vD[i], 0.00001 ) == false )
            return false;
     

    note = "results accepted";
    return true;
}

int TEST_Vector_Accessors(string& note){

    note = "Vector Accessor test not implemented";


    return false;
}

void TEST_Quaternion_Module( ){

    int result;
    string note;

    print_module_header("Quaternion");

    
    print_module_footer("Quaternion");


}

