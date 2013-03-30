#include <iostream>
#include <stdio.h>
#include <limits>

using namespace std;

int divisor_count(long long);

int main()
{
  
  long long value = 1;
  long long prev  = 0;
  long int j = 100;
  int divisors = 1;
  
  divisors = divisor_count(value);
  while(divisors < 500)
  {
    prev += value++;
    while(prev%13!=0){
      prev += value++;
      while(prev%11!=0){
        prev += value++;
        while(prev%7!=0){
          prev += value++;
          while(prev%5!=0){
            prev += value++;
            while(prev%3!=0){
              prev += value++;
              while(prev%2!=0){
                prev += value++;}}}}}}
    
    
    divisors = divisor_count(prev);
   
    if(divisors > j){
      cout << "VAL: " << value << " PREV: " << prev << "  DIV: " << divisors << " j: " << j << endl;
      j += 2;
    }
  }
  cout << endl;
  cout << "Value is " << prev << endl;
  return 0;
}

int divisor_count(long long value)
{
  int count = 0;
  long long lower_limit = value/2+1;

  for(long long i = 2; i < lower_limit; i++ )
    if(( value % i ) == 0 )
      count++;
  return count+1;
}
