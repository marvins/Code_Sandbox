/**
 * @file   euler61.cpp
 * @author Marvin Smith
 * @date   10/15/2013
*/
#include <cmath>
#include <iostream>
#include <utility>
#include <vector>

#include <StringUtilities.hpp>

using namespace std;

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



int main( int argc, char* argv[] ){
   
    try{
       
        // find the indeces where numbers become 4 digits
        vector<pair<int,int> > indeces(9);
        bool break1 = false;
        bool break2 = false;
        for( size_t i=3; i<=8; i++ ){
        
            // cound indeces
            size_t x=0;
            break1 = false;
            break2 = false;
            while( break2 != true ){
            
                // convert value to string
                string val=num2str(Pjn(i,x));

                // check if 4 digits
                if( break1 == false && val.size() >= 4 ){
                    break1 = true;
                    indeces[i].first = x;
                } else if( break1 == true && val.size() >= 5 ){
                    break2 = true;
                    indeces[i].second = x;
                } else {
                    x++;
                }
            }
        }

        // start building the sets
        vector<vector<string> > values(9);
        for( size_t i=3; i<=8; i++ ){
            
            // push all numbers to the list
            for( size_t j=indeces[i].first; j<indeces[i].second; j++ )
                values[i].push_back( num2str(Pjn( j, i)) );
        }

        // iterate through all items
        while( true ){
            
            // create list of items
            for( size_t a=0; a<values[3].size(); a++ )
            for( size_t b=0; b<values[4].size(); b++ )
            for( size_t c=0; c<values[5].size(); c++ )
            for( size_t d=0; d<values[6].size(); d++ )
            for( size_t e=0; e<values[7].size(); e++ )
            for( size_t f=0; f<values[8].size(); f++ ){
                
                // add everything to the list
                vector<string> tempList;
                tempList.push_back( values[3][a] );
                tempList.push_back( values[4][b] );
                tempList.push_back( values[5][c] );
                tempList.push_back( values[6][d] );
                tempList.push_back( values[7][e] );
                tempList.push_back( values[8][f] );
            
                // sort permutations
                std::sort( tempList.begin(), tempList.end() );

                // test permutations
                do{
                    // make sure none of the values are the same
                    for( size_t v=0; v<tempList.size()-1; v++ ){
                        if( tempList[v] == tempList[v-1] ){
                            break;
                        }
                    }

                    // check front and back 

                } while( std::next_permutations( tempList.begin(), tempList.end() ))
            }
        }


    } catch ( string e ){
        cout << e << endl;
    }
    

    return 0;
}

