#include <cmath>
#include <iostream>
#include <vector>

#include "Primesieve.hpp"

using namespace std;

const int MIN_DIGITS=5;
const int MAX_DIGITS=8;

/// List of solutions
vector<int> prime_list;

/// Number of primes to look for
const int PRIME_FAMILY_SIZE=7;

template <typename TP>
TP str2num( std::string const& value ){
    
    std::stringstream sin;
    TP result;
    sin << value;
    sin >> result;
    return result;
}

template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}


bool is_family( vector<int> temp_list, const int& num_primes ){
    
    if( temp_list.size() != num_primes ){
        throw string("ERROR: bad input");
    }

    // number list converted to strings
    vector<string>  str_list;
    for( size_t i=0; i<temp_list.size(); i++ )
        str_list.push_back( num2str(temp_list[i]));
    
    
    // list of indeces matching between sets
    vector<bool> idx_match_list(temp_list[0].size());

    // check each pair for a set of matching characters
    // - first, iterate over each character
    for( size_t i=0; i<str_list[0].size(); i++ ){
        
        // - next, iterate over each string
        bool matching = true;
        for( size_t j=1; j<str_list.size(); j++ ){
            if( str_list[0][i] != str_list[j][i] ){
                matching = false;
                break;
            }
        }
        // if they are not matching, then skip
        if( matching == true ){
            idx_match_list[i] = true;
        } else{
            idx_match_list[i] = false;
        }

    }

    // for the indeces of non matching characters, make sure they are all the same in each number

    
    return false;
}

int main( int argc, char* argv[] ){
    
    // create prime sieve
    Primes primesieve( 99999999, true );
    cout << "Prime sieve built" << endl;

    // start iterating from min to max
    for( int i=MIN_DIGITS; i<=MAX_DIGITS; i++ ){
        
        // for each digit range, find the min and max value
        int min_value = pow(10, i-1);
        int max_value = pow(10, i) - 1;
        
        // clear our list of primes
        prime_list.clear();

        // iterate from min value to max value
        for( int j=min_value; j<=max_value; j++ ){

            // check if the number is prime
            if( primesieve.is_prime( j ) == true ){
                prime_list.push_back(j);
            }

        }

        
        // create a container which stores indeces of items we are looking at
        vector<int> idxs(PRIME_FAMILY_SIZE);
        for( int j=0; j<PRIME_FAMILY_SIZE; j++ )
            idxs[j] = j;
        
        // current index with the max idx
        int cmax = PRIME_FAMILY_SIZE - 1;

        int idxx=0;

        // iterate through our list and look for members with matching digits
        bool break_loop = false;
        while( break_loop == false ){
            
            // compare the primes in the group
            vector<int> temp_prime_list;
            for( int j=0; j<idxs.size(); j++ ){
                temp_prime_list.push_back( prime_list[idxs[j]] );
            }

            // test if this is a valid family
            if( is_family( temp_prime_list, PRIME_FAMILY_SIZE ) == true ){
                break;
            }
                
            // increment the indeces
            if( idxs[idxs.size()-1] >= (prime_list.size()-1) ){
                 
                // if we max the furthest right index, then we need to increment the previous idxs
                int cmaxx = idxs.size()-2;
                while(true){
                    if( idxs[cmaxx] >= idxs[cmaxx+1]-1){
                        cmaxx--;
                        if( cmaxx < 0 ){ break_loop = true; break; }
                    }
                    else{
                        idxs[cmaxx]++;
                        for( size_t k=cmaxx+1; k<idxs.size(); k++ ){
                            idxs[k] = idxs[k-1]+1;
                        }
                        break;
                    }
                }
                
            }
            else{
                idxs[idxs.size()-1]++;
            }

        }

    }



    return 0;
}
