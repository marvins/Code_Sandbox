#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

int ones[10] = {0,3,3,5,4,4,3,5,5,4};
int tens[10] = {0,3,6,6,5,5,5,7,6,6};
int teens[10]= {0,6,6,8,8,7,7,9,8,8};

int count_digits(int m)
{
  long long result = 0;

  if(m==1000)
    return 11;
  if(m/100>0)
    result += ones[m/100] + 7 + 3; //one hundred and...
  if(m%100>=20||m%100==10)
    result += tens[(m%100)/10]+ones[m%10]; //twenty seven
  else if(((m%100)/10==1)&&((m%100)/10<2)) //fourteen
    result += teens[m%10];
  else
    result += ones[m%10];

   return result;
}

int main()
{
    long long total=0;
    int limit;
    for(int i=1;i<1001;i++)
      total += count_digits(i);
    cout << total-27 << endl;
    return 0;
}

