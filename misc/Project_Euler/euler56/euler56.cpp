/**
 * @file    euler56.cpp
 * @author  Marvin Smith
 * @date    9/25/2013
 */
#include <iostream>

#include <BigIntegerLibrary.hh>
#include <StringUtilities.hpp>

using namespace std;

int main( int argc, char* argv[] ){
    
    BigInteger maxVal = 0;

    // iterate through the a
    for( size_t a=2; a<100; a++ ){

        // iterate through b
        for( size_t b=2; b<100; b++ ){
            
            // compute power
            BigInteger value = power( a, b );
            
            // convert to string
            string value_string = bigIntegerToString( value );
            
            // compute the sum of digits
            BigInteger sum = 0;
            for( size_t i=0; i<value_string.size(); i++ ){
                sum += str2num<int>(value_string.substr(i,1));
            }

            // compare sum 
            if( maxVal < sum ){
                maxVal = sum;
            }

        }
    }

    cout << "Max: " << maxVal << endl;

    return 0;
}

