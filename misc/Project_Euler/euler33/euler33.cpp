#include <iostream>
#include <stdio.h>

using namespace std;

int main()
{
   double a,b,c,d;

   for(int i=2;i<=9;i++)
   {
     for(int j=1;j<i;j++)
     {
       for(int k=1;k<=9;k++)
       {
          a = j*10+k;
          b = k*10+i;
          c = j;
          d = i;
          if((a/b)==(c/d)){
            cout << a << " , " << b << " , " << c << " , " << d << endl;
            cout << a/b << "   -   " << c/d << endl;
            cout << j << " , " << i << " , " << k << endl;

            cin.get();
          }
       }
     }
   }
   return 0;
}
