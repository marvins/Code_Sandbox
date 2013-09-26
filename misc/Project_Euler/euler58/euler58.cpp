/**
 * @file euler58.cpp
 * @author Marvin Smith
 * @date   9/25/2013
 */
#include <iostream>

#include <Primesieve.hpp>

using namespace std;

int main( int argc, char* argv[] ){

    // create prime sieve
    Primes primesieve( 9999999 );

    // iterate for n sides until you get less than 10% primes
    int maxIterations = 100;
    int start_sum = 1;
    int end_sum = 1;
    for( int i=1; i<maxIterations; i++ ){

        // find the number of values which make up the entire ring
        int boxCount = pow(2*i+1,2);

        // subtract the inner ring
        boxCount = boxCount - pow(2*(i-1)+1,2);
        
        // compute the starting value for the box
        start_sum = end_sum + 1;
        end_sum   = start_sum + (boxCount-1);

        // find the diagonal values

        cout << "Size: " << boxCount << endl;
        cout << "Start: " << start_sum << ", End: " << end_sum << endl;
        cin.get();

    }

    return 0;
}

