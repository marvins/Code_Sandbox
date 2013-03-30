#include <iostream>
#include <deque>
#include <cmath>

#include "BigIntegerLibrary.hh"

using namespace std;

int main( int argc, char* argv[] ){

  BigInteger num;
  deque<BigInteger> data;
  deque<BigInteger> data_new;
  deque<BigInteger>::iterator it;

  for(int i=2; i<=100; i++)
    for(int j=2; j<=100; j++){
      
     num = i; 
     for(int power = 1; power < j; power++)
        num *= i;
 
     data.push_back( num );
  }
  
  sort(data.begin(), data.end());
  
  data_new.resize(data.size());
  cout << "BEFORE: " << data.size() << endl;
  it = unique_copy(data.begin(), data.end(), data_new.begin());
  data_new.resize( it - data_new.begin());
  cout << "AFTER : " << data_new.size() << endl;

  return 0;
}


