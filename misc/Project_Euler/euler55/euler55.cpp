/**
 *  Project Euler 55 Solution
 */
#include <algorithm>
#include <iostream>
#include <string>

#include <BigIntegerLibrary.hh>
#include <StringUtilities.hpp>

using namespace std;


bool isPalindrome( const BigInteger& value ){

    // convert value to string
    string value_string   = num2str(value);
    string reverse_string = num2str(value);

    std::reverse( reverse_string.begin(), reverse_string.end());

    if( value_string == reverse_string )
        return true;
    return false;
}

bool isLychrel( const int& value, const int& maxIterations = 50 ){
    
    // current sum
    BigInteger current_sum = value;
    
    // begin iterating from 0 to maxIterations
    for( size_t i=0; i<maxIterations; i++ ){
        
        // convert current sum to string
        string current_sum_string = num2str(current_sum);
        
        // reverse value and add
        string reverse_string = current_sum_string;
        std::reverse( reverse_string.begin(), reverse_string.end() );
        
        BigInteger reverse_sum = str2num<long>(reverse_string);
        
        // current sum update
        current_sum = current_sum + reverse_sum;

        // check if number is palindrome
        if( isPalindrome( current_sum ) == true ){
            return false;
        }
    }
    
    return true;
}

int main( int argc, char* argv[] ){

    // number of lychrel numbers
    int counter = 0;

    // iterate from 10 to 10000
    for( size_t i=10; i<10000; i++ ){
        
        // check if lychrel
        if( isLychrel( i ) == true ){
            counter++;
        }
    }
    
    cout << counter << endl;

    return 0;
}

