#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;


vector<int> movezeros( vector<int>& number){

    // temp variable
    vector<int> output(number.size(), 0);
    int counter = 0;

    for( size_t i=0; i<number.size()-1; i++ ){
        
        if( number[i] != 0 ){
            output[counter++] = number[i];     
        }
    }
    return output;

}


void print(vector<int>const& number){

    for( size_t i=0; i<number.size(); i++ ){
        cout << number[i];
    }
    cout << endl;
}

int main( int argc, char* argv[] ){
    
    srand(time(NULL));
    
    
    vector<int> number;
    string tempStr = argv[1];
    for( size_t i=0; i<tempStr.size(); i++ ){
        number.push_back((int)tempStr[i] - '0');
    }

    print(number);

    number = movezeros(number);

    print(number);

    return 0;
}

