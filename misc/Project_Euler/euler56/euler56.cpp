/**
 * @file    euler56.cpp
 * @author  Marvin Smith
 * @date    9/25/2013
 */
#include <iostream>

#include <BigIntegerLibrary.hh>

using namespace std;

int main( int argc, char* argv[] ){


    // iterate through the a
    for( size_t a=2; a<100; a++ ){

        // iterate through b
        for( size_t b=2; b<100; b++ ){
            
            BigInteger base(a);
            BigInteger exp(b);

            BigInteger value = power( base, exp );
            
            // iterate through digits, computing the sum of digits
            BigUnsigned mag = value.getMagnitude();
            
            cout << value << endl;
            cout << mag.getLength() << endl;
            //NumberlikeArray* data = mag.getBlk(); 
            cin.get();

        }
    }

    return 0;
}

