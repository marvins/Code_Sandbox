#include <fstream>
#include <iostream>
#include <string>

#include "BigIntegerLibrary.hh"

using namespace std;


int main( int argc, char* argv[] ){

  BigInteger a = 0;
  BigInteger b = 1;
  BigInteger c = 1;

  double fraction;

  string big_ass_string;
  string big;

  big_ass_string.push_back('1');
  for(int i=0; i<999; i++)
    big_ass_string.push_back('0');
  
  
  for(int i=0; ; i++){

    b = a;
    a = c;
    c = a + b;
  
    
		big = bigIntegerToString(c); 
    
    cout << i+2 << "  -> " << big.size() <<  endl;
    if( big.size() == 1000 )
      break;
  }
  
  ofstream fout;
  fout.open("euler25_results.txt");

  fout << c << endl;

  fout.close();


  return 0;
}
