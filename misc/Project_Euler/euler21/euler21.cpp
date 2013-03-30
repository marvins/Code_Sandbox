#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

long int sum(int);

int main(int argc,char* argv[])
{
    int max = atoi(argv[1]);
    long long tally=0;
    vector<long int> vec(max+1);

    for(int i=2;i<max+1;i++)
      vec[i] = sum(i);
    
    for(int i=2;i<max+1;i++)
      if(i==vec[vec[i]]&&vec[i]!=0&&i!=vec[i])
        tally += i;
      
    cout << tally << endl;
    return 0;
}


long int sum(int val)
{
  int sum = 0;
  for(int i=2;i<val/2+1;i++)
    if(val%i==0)
      sum += i;
  return sum+1;
}
