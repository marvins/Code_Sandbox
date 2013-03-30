#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;


int letter_val(char);
void load_file(vector<string>& list);

int main()
{
    string name;
    vector<string> list;
    long long run_sum = 0;
    int letter;

    load_file(list);
    sort ( list.begin() , list.end() );


    for(int i=0;i<list.size();i++){
      name = list[i];
      letter = 0;
      for(int j=0;j<name.length();j++)
        letter += letter_val(name[j]);
      run_sum += letter * (i+1);
    }

    cout << "RUNNING SUM = " << run_sum << endl;
    return 0;
}  
int letter_val(char c)
{
  char letters[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZ"};
  for(int i=1;i<=26;i++)
    if( c == letters[i-1] )
      return i;
  return 0;
}

void load_file(vector<string>& list)
{
    string name;
    ifstream fin;
    fin.open("euler22.txt");
    
    fin.get();
    getline(fin,name,'\"');
    fin.get();
    
    while(fin.good()){
      fin.get();
      list.push_back(name);
      getline(fin,name,'\"');
      fin.get();
    }
    fin.close();
}
