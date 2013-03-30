#include <iostream>
#include <vector>
#include <string.h>

using namespace std;

void ex26(){ 
  int n, i, len, maxlen, maxn; 
  maxlen = 0; 
  for( n=2 ; n<=1000 ; n++ ){ 
    int rest = 1; 
    int r0; 
    for( i=0 ; i<n ; i++ ) rest = (rest*10)%n; 
    r0 = rest; 
    len = 0; 
    do { 
      rest = (rest*10)%n; 
      len++; 
    } while( rest!=r0 ); 
    if( len>maxlen ){ 
      maxn = n; 
      maxlen = len; 
    } 
  } 
  printf("ex26: %d: %d\n", maxn, maxlen); 
} 

vector<unsigned long> get_primes(unsigned long max){
  vector<unsigned long> primes;
  char *sieve;
  sieve = new char[max/8+1];
  // Fill sieve with 1  
  memset(sieve, 0xFF, (max/8+1) * sizeof(char));
  for(unsigned long x = 2; x <= max; x++)
    if(sieve[x/8] & (0x01 << (x % 8))){
      primes.push_back(x);
      // Is prime. Mark multiplicates.
      for(unsigned long j = 2*x; j <= max; j += x)
        sieve[j/8] &= ~(0x01 << (j % 8));
    }
  delete[] sieve;
  return primes;
}

int main(void){
  vector<unsigned long> primes;
  primes = get_primes(1000);

  int c = 1;
  vector<unsigned long>::iterator it;

  //look only at primes
  for(it=primes.begin(); it < primes.end(); it++){

  }
  ex26();


  cout << endl;
  return 0;
}

