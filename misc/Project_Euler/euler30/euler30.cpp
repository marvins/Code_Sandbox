#include <iostream>
#include <cmath>
#include <string>

#include "BigIntegerLibrary.hh"

using namespace std;

int main( int argc, char* argv[] ){

  int count = 0;
  BigInteger num = 20;
  BigInteger sum;
  BigInteger Total_Sum = 0;
  string value;
  char c;
  int n;

  while(1){

    //increment number
    num++; 
  
    //convert number to string
		value = bigIntegerToString(num);

    //for each letter in string
    sum = 0;
    for(int i=0; i<value.size(); i++){
      //get character
      c = value[i];
      //convert to int
      n = c - 48;
      //multiply by power
      sum += (long int)pow((double)n, 5);
    }

    //check if sum is equal to total
    if( sum == num ){
      count++;
      Total_Sum += sum;
      cout << "MATCH: " << count << "  -> " << sum << endl;
      cout << "TOTAL SUM: " << Total_Sum << endl;
    }
  }
  
  
  return 0;
}


