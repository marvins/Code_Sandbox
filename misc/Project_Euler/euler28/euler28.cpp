#include <iostream>
#include <stdlib.h>

using namespace std;

int main(int argc,char* argv[])
{
    if(atoi(argv[1])%2==0){cout << "ERROR: arg must be odd" << endl; return 0;}

    const int range = atoi(argv[1]);
    long int sum = 1;
    long long total=1;

    for(int diameter = 3;diameter<=1001;diameter+=2)
      for(int corners = 0;corners<4;corners++){
        sum += diameter-1;
        total += sum;}
     cout << total << endl;
    return 0;
}
