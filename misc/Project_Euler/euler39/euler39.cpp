#include <cmath>
#include <iostream>
#include <vector>

using namespace std;

int main(){

  vector<int> totals(1000);
  for(int i=0; i<totals.size(); i++)
    totals[i] = 0;

  double c;
  int p;
  for(int a=0; a<500; a++){
    for(int b=a; b<500; b++){
      c = sqrt( a*a + b*b );
      p = a + b + c;
      if( c == floor(c) && p < 1000)
        totals[p]++;
    }
  }

  int max = 0;
  int val = totals[0];
  for(int i=1; i<totals.size(); i++){

    if( totals[i] > val ){
      val = totals[i];
      max = i;
    }
   }

   cout << val << endl;
   cout << max << endl;

   return 0;
}

