#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "primesieve.h"

using namespace std;

int prime_parts(long int);

int main(int argc, char* argv[])
{
      long int max = 0;
      long int count = 1;
      long int sum = 0,SUM;
      long int I,J;
      long int result = 0;
      long int range = atoi(argv[1]);

      Primes list(range,false);

      for(long int i=2;i<sqrt(range);i++){
        if(i>2&&(i%2)==0)
          i++;
        I = i;

        if(list.prime_check(I)){
          sum = i;
          count = 1;
          for(long int j=i+1;j<range/2;j++){
            J = j;
            if(list.prime_check(J))
            {
              if(((long long)sum + (long long)j)<range){
                sum += j;
                SUM = sum;
                count++;
                if(list.prime_check(SUM)){
                  if(count > max){
                    max = count;
                    result = sum;
                  }
                }
              }
            }
          }
        }
      }
      cout << result << "   " << max << endl;
      return 0;
}

