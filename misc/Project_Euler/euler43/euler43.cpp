#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include "Permutation_Engine.h"
#include "Functors.h"

using namespace FNCT;
using namespace std;

int main(int argc , char *argv[])
{
  long long sum = 0;
  int count = atoi(argv[1]);

  euler43_func<int> Euler43;
  vector<int> list;
  vector<vector<int> > results;
 
  for(int i=0;i<count;i++)
    list.push_back(i);

  permute_pandigital(list,results,0,count,false,Euler43);
  
  for(int i=0;i<results.size();i++)
    for(int j=count-1;j>=0;j--)
      sum += results[i][j]*pow(10,9-j);
    
  cout << "SUM IS : " << sum << endl;

  return 0;
}

