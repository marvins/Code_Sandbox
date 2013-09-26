#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <vector>

#include <Primesieve.hpp>
#include <StringUtilities.hpp>

using namespace std;

const int MIN_DIGITS=5;
const int MAX_DIGITS=7;

/// Number of primes to look for
const int PRIME_FAMILY_SIZE=8;

vector<int> create_family( const int& prime, const vector<bool>& binarray ){
    
    // convert the prime to a string
    string pstring = num2str(prime);
    
    // create output object
    vector<int> output;

    // for each number from 0 to 9
    for( size_t i=0; i<10; i++ ){
        
        // we dont want the first bit to be zero ever
        if( binarray[0] == true && i == 0 ) continue; 

        // create a temporary entry
        string tempString = pstring;

        // for each digit which is true, overwrite the char
        for( size_t j=0; j<binarray.size(); j++ ){
            if( binarray[j] == true){
                tempString[j] = (char)(i+48);
            }
        }

        // add to string list
        output.push_back( str2num<int>(tempString));

    }
    return output;
}


/**
 *  Main Function
 */
int main( int argc, char* argv[] ){
    
    try{

    // create prime sieve
    Primes primesieve( 9999999, true );
    cout << "Prime sieve built" << endl;

    /// List of primes for each power of 10
    deque<int> prime_list;


    // start iterating from min to max
    for( int x=MIN_DIGITS; x<=MAX_DIGITS; x++ ){
        
        // for each digit range, find the min and max value
        int min_value = pow(10, x-1);
        int max_value = pow(10, x) - 1;
        
        // clear our list of primes
        prime_list.clear();

        
        // iterate from min value to max value, adding all primes found
        for( int j=min_value; j<=max_value; j++ ){

            // check if the number is prime
            if( primesieve.is_prime( j ) == true ){
                prime_list.push_back(j);
            }

        }
        

        /**
         * Now that we have a list of primes to search, begin tearing apart primes
         */
        int prime;
        while( prime_list.size() > 0 ){
            
            // grab the next prime
            prime = prime_list.front();
         
            // create a large list of patterns to match against
            // for the number of digits, find the max value in binary
            unsigned int maxVal = pow(2,x)-1;
        
            for( size_t a=2; a<maxVal; a++ ){
            
                // find the indeces this number gives as binary 1
                // convert number to bool binary array
                vector<bool> binarray(x, false);
                for( size_t b=0; b<binarray.size(); b++ ){
                    binarray[b] = ( ((unsigned int)a & (unsigned int)pow(2,b)) == (unsigned int)pow(2,b)) ? true : false;
                }
                
                // prune even numbers
                if( binarray[0] == true ){ continue; }
               
                // swap bits to match with strings
                std::reverse( binarray.begin(), binarray.end());
                
                /**
                 * Now we have a prime as well as a list of indeces to test against
                 */
                // for each binary 1, add to array
                // create a list of 10 numbers
                vector<int> test_family = create_family( prime, binarray );
                
                // count how many in the family are primes
                int numPrimes = 0;
                for( size_t b=0; b<test_family.size(); b++ )
                    if( primesieve.is_prime(test_family[b]) )
                        numPrimes++;

                if( numPrimes >= PRIME_FAMILY_SIZE ){
                    
                    cout << "NumPrimes: " << numPrimes << endl;
                    cout << "Permutations" << endl;
                    for( size_t b=0; b<test_family.size(); b++ )
                        if( primesieve.is_prime( test_family[b] ) == true )
                            cout << test_family[b] << endl;
                    cin.get();
                }


            }


            // pop off the next prime
            prime_list.pop_front();
        }


    }

    } catch ( string e ){
        cout << e << endl;
    }

    return 0;
}
