/**
 * @file   euler57.cpp
 * @author Marvin Smith
 * @date   9/25/2013
 */
#include <iostream>

#include <BigIntegerLibrary.hh>
#include <StringUtilities.hpp>

using namespace std;


void solve( BigInteger& numerator, BigInteger& denominator, int const& iterations ){
    
    /**
     *  Exit Condition: First value of the series
     */
    if( iterations <= 0 ){
        numerator = 3;
        denominator = 2;
    }

    /**
     * Otherwise, follow parameterized functions
     */
    else{
        // get the previous numerator
        solve( numerator, denominator, iterations-1);

        // create temp variable
        BigInteger temp_numerator = numerator;

        // apply the numerator function
        numerator   = temp_numerator + (denominator*2);
        denominator = temp_numerator + (denominator);
    }

}

int main( int argc, char* argv[] ){
    
    int count = 0;

    // iterate 1000 times
    BigInteger numerator, denominator;
    for( size_t i=0; i<1000; i++ ){

        // compute the numerator and denominator using recursion
        solve( numerator, denominator, i );

        if( num2str(bigIntegerToString(numerator)).size() > num2str(bigIntegerToString(denominator)).size())
            count++;
    }
    
    cout << count << endl;

    return 0;
}
