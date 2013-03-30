#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "primesieve.h"

using namespace std;

int main(int argc, char* argv[])
{
    long int max = atoi(argv[1]);
    long int counter = 5;
    long int test;
    bool flag = true;

    Primes library(max,true);

    while(counter < max){
      if(!library.prime_check(counter)){
          for(long long i = 0;(flag&&(i<counter/2));i++){
              test = counter - pow(i,2)*2;
              if(library.prime_check(test)){
                flag = false;}}
        if(flag){
          cout << "FOUND PROOF AT " << counter << endl;
          return true;}
        flag = true;}
      counter += 2;}
   return 0;
}
