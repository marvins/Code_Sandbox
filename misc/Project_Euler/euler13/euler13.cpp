#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

int main () 
{
  double value = 0;
  double a;
  ifstream fin;
  
  fin.open("euler13.txt");
  for(int i=0;i<100;i++){
    fin >> a;
    value += a;
   }
   cout << setprecision(11) << value << endl;
  
  return 0;
}



