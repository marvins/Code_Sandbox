
#include <BigIntegerLibrary.hh>

#include <iostream>

using namespace std;

int main( int argc, char* argv[] ){
    
    // test the factorial function
    cout << factorial(10) << endl;
    if( factorial(10) != BigUnsigned(3628800)){
        cout << "Factorial(10) failed" << endl;
    } else{
        cout << "Factorial(10) passed" << endl;
    }


    return 0;
}

