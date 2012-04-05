#include <iostream>
#include <ctime>

#include "ProgressBar.h"

using namespace std;

void wait ( int seconds )
{
   clock_t endwait;
   endwait = clock () + seconds * CLOCKS_PER_SEC ;
   while (clock() < endwait) {}
}

int main( int argc, char** argv){

   ProgressBar bar;
   
   bar.min = 0;
   bar.max = 1000;

   for(int i=0; i<1000; i++){
      bar.current = i;
      cout << bar << endl;
   }
   return 0;
}
