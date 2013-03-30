#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;


int main(int argc,char* argv[])
{
  long long last=1,current;
  const long long row = atoi(argv[1])*2+1;
  
  for(int i=0;i<row;i++){
    if(i==0)
      last=1;
    else{
      current = last*((row-i)/(float)i);
      if(current<last){
        cout << last << endl;
        return 0;}
      last = current;}}
  return 0;
}
