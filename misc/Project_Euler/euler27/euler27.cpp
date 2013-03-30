#include <iostream>

#include "primesieve.h"

using namespace std;

long int quadratic( const int a, const int b, const int n ){
   return (n*n + a*n + b);
}

int main( int argc, char* argv[] ){

  
  Primes primes( 1000000, true );

  bool some_bool;
  for(int i=0; i<10; i++)
    cout << primes.prime_next(some_bool) << endl;
    
  int count;
  int max_cnt = 0;
  int max_a = -1000;
  int max_b = -1000;

  for(int i = -999; i<1000; i++)
    for(int j= -999; j<1000; j++){

      count = 0;
      while( primes.is_prime( quadratic( i, j, count)))
        count++;

      if( count > max_cnt ){
        max_a = i;
        max_b = j;
        max_cnt = count;
        cout << "i: " << i << ", j: " << j << ", count: " << count << endl;
        cin.get();
      }
    
    }

    cout << "A: " << max_a << ", B: " << max_b << ", product: " << max_a * max_b << endl;

  return 0;
}

