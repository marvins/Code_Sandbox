#include <iostream>
#include <stdio.h>
#include <Primesieve.hpp>

using namespace std;

int main()
{
    /// Create a prime sieve
    Primes list(2000000, true);
    bool var = false;
  
    long long sum = 0;;
    long int pos = 0;
    long int max = 2000000;
    int i=0;

    /// Start building prime values
    list.prime_start();
  
    while( pos < max && !var) {

        // pull the next prime
        pos = list.prime_next(var);
        sum += pos;
    }
    cout << endl;
  
  cout << "sum:  " << sum << endl;
  return 0;
}
