#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

void print(vector<int>const&);

int main()
{
    vector<int> array;
    long long sum = 0;
    array.push_back(2); 
    for(int i=0;i<500;i++)
      array.push_back(0);
cout << "HERE" << endl;

    for(int i=1;i<1000;i++){
        array[0] *= 2;
        for(int j=1;j<array.size();j++){
            array[j] *= 2;
            if(array[j-1]>=10){
              array[j] += array[j-1]/10;
              array[j-1] = array[j-1]%10;
            }
        } 
    }
    for(int i=0;i<array.size();i++)
      sum += array[i];

    cout << sum << endl;

    return 0;
}

