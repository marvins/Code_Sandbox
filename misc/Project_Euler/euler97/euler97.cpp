#include <iostream>

using namespace std;

int main()
{
    long long number = 2;

    for(long long i = 0 ; i < 7830457-1 ; i++ ){
      number *= 2;
      number = number%10000000000;
    }
    cout << number*28433+1 << endl;
    return 0;
}
