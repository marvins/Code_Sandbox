#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char* argv[])
{
    ifstream fin;
    string data;
    int a0=0,a1=2;
    int ptr1,ptr2,val;
    long long max;
    vector<vector<long long> > array;
    vector<long long> row;

    fin.open(argv[1]);
    fin >> val;
    row.push_back(val);

    array.push_back(row);
    row.clear();
    while(fin.good()){
        while(a0<a1){
          fin >> val;
          row.push_back(val);
          a0++;
        }
        array.push_back(row);
        row.clear();
        a0=0;
        a1++;
    }
    fin.close();
    array.pop_back();

    //////////////START OF PROGRAM///////////////
    a0=0;a1=1;
    for(int i=1;i<array.size();i++){
      max = array[i].size()-1;
      for(int j=0;j<array[i].size();j++){
        if(i==0);
        else{
          ptr1 = j;
          ptr2 = j-1;
          if(ptr2<0)
            array[i][j]+=array[i-1][ptr1];
          else if(ptr1>=max)
            array[i][j]+=array[i-1][ptr2];
          else
            array[i][j]+= (array[i-1][ptr1]>array[i-1][ptr2])?array[i-1][ptr1]:array[i-1][ptr2];
        }
      }
    }
     
    for(int j=0;j<array[array.size()-1].size();j++)
      if(array[array.size()-1][j]>max)
        max = array[array.size()-1][j];
    cout << max << endl;
}
