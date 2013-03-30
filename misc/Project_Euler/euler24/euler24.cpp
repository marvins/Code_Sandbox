#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void Print( const vector<int>& a ){

  for(int i=0; i<a.size(); i++)
    cout << a[i];
  cout << endl;
}

int main( int argc, char* argv[] ){

  cout << "Permutation Engine" << endl;
  
  vector<int> array(10);
  int count;
  stringstream(string(argv[1])) >> count;

  for(int i=0; i<array.size(); i++)
    array[i] = i;

  Print( array );

  for(int i=0; i< count-1; i++)
    next_permutation( array.begin(), array.end() );

  Print( array );

  return 0;
}
