/**
 * @file euler53.cpp
 * @author  Marvin Smith
 * @date    9/24/2013
 */
#include <iostream>

#include <BigIntegerLibrary.hh>

using namespace std;


BigInteger nCr( const BigInteger& n, const BigInteger& r ){
    
    BigInteger numerator   = factorial(n);

    BigInteger denominator = factorial(r)*factorial(n-r);

    return BigInteger(numerator/denominator);


}

/**
 *  Main Function
 */
int main( int argc, char* argv[] ){

    try{

    // iterate from 1 to 100 for n
    BigInteger result;
    int count = 0;
    for( size_t n=1; n<=100; n++ ){

        // for r, iterate from 0 to n
        for( size_t r=0; r<=n; r++ ){

            result = nCr( n, r);
            
            if( result > 1000000 )
                count++;
        }
    }

    cout << count << endl;
    
    } catch (char const* str_error ){
        cout << str_error << endl;
        return 1;
    }
    
    return 0;

}

