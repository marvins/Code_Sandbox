/**
 * @file   euler61.cpp
 * @author Marvin Smith
 * @date   10/15/2013
 */
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <set>
#include <utility>
#include <vector>

#include <BigIntegerLibrary.hh>
#include <StringUtilities.hpp>

using namespace std;

/// Dictionary of Polygonal Numbers
vector<set<string> > figurateList(9);

/// Current List
vector<string>  currentList;

/**
 * 
 */
bool is_cyclic( vector<string> const& slist ){

    for( size_t i=0; i<slist.size()-1; i++ ){
        if( slist[i][slist[i].size()-2] != slist[i+1][0] ) return false;
        if( slist[i][slist[i].size()-1] != slist[i+1][1] ) return false;
    }

    if( slist[slist.size()-1][slist[slist.size()-1].size()-2] != slist[0][0] ) return false;
    if( slist[slist.size()-1][slist[slist.size()-1].size()-1] != slist[0][1] ) return false;

    return true;
}

/**
 * 
 */
int Pjn( int const& j, int const& n ){

    switch( j ){

        case 3:
            return (n*(n+1)/2);

        case 4:
            return (n*n);

        case 5:
            return (n*(3*n-1)/2);

        case 6:
            return (n*(2*n-1));

        case 7:
            return (n*(5*n-3)/2);

        case 8:
            return (n*(3*n-2));

        default:
            throw string("Error: Unknown value");
            return -1;

    }
    return -1;
}


/**
 * Main Function
 */
int main( int argc, char* argv[] ){

    try{
            
        // iterate through the list, building the sets
        for( size_t i=3; i<=8; i++ ){

            // counter
            int x = 0;

            // add values
            while( true ){

                // compute number
                string val = num2str(Pjn(i,x));

                // if the value is less than 4, continue
                if( val.size() < 4 ){ x++; continue; }

                // if the value is greater than 4, break
                if( val.size() > 4 ){ break; }

                // add to dictionary
                figurateList[i].insert(val);

                // add to the current list
                currentList.push_back(val);

                x++;
            }

        }

        // compute permutations
        BigInteger _c = 0;
        BigInteger _max = (factorial( currentList.size() )/(factorial( currentList.size() - 6 )*factorial(6)));

        // sort all values
        std::sort(   currentList.begin(), currentList.end() );

        // remove all duplicates
        std::unique( currentList.begin(), currentList.end() );
        std::cout.precision(6);
        
        // begin iterating over permutations of the list
        vector<string> tempList(6);
        int x = 0;
        do{

            // build list
            for( size_t i=0; i<6; i++ )
                tempList[i] = currentList[i];

            // sort permutations
            std::sort( tempList.begin(), tempList.end() );

            // test permutations
            do{

                // check front and back
                if( is_cyclic( tempList ) == true ){
                    cout << "Output" << endl;
                    for( size_t v=0; v<tempList.size()-1; v++ )
                        cout << tempList[v] << endl;
                    return 0;
                }

            } while( std::next_permutation( tempList.begin(), tempList.end() ));
            
            // increment
            _c++;

            if( _c % 50000 == 0 ){
                cout << std::fixed  << ((double)_c.toLong()/_max.toLong())*100 << endl;
            }

        } while ( std::next_permutation( currentList.begin(), currentList.end() ));


    } catch ( string e ){
        cout << e << endl;
    }


    return 0;
}

