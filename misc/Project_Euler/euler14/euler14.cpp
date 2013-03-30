#include <iostream>
#include <cassert>

using namespace std;

void count( long long& value , long int& counter )
{
    counter++;
    if( value == 1 )
      return;
    else if( value%2 == 0 )
      value = value/2;
    else
      value = 3*value+1;
    count( value , counter );
}

int main()
{
   long long value = 13;
   long int counter = 0;
   long long temp;
   long long max = 0;
   long int max_counter = 0;

   for(long long i=4;i<1000000;i++){
     temp = i;
     counter = 0;
     count(temp,counter);
     if(counter > max_counter){
        max_counter = counter;
        max = i;
     }
   }
   cout << max << endl;
   
   return 0;
}

