#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

long int sum(int);

int main(int argc,char* argv[])
{
    int max =28124;
    vector<long int> vec;
    long long tally = 0;
    bool array[max];


    for(int i=0;i<max;i++)
      array[i]=false;
    for(int i=12;i<max;i++)
      if(i<sum(i))
        vec.push_back(i);

    for(int i=0;i<vec.size();i++)
      for(int j=0;j<vec.size();j++)
        if(vec[i]+vec[j]<max)
          array[vec[i]+vec[j]]=true;

    for(int i=0;i<max;i++)
      if(!array[i])
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
