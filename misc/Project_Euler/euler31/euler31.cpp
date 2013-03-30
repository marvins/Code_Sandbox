#include <iostream>

using namespace std;

int main( int argc, char* argv[] ){

  //start at 1 due to 200p coin
  int count = 1;

  //iterate through all possible 1p
  for(int a=0; a<(200); a++)

    //iterate through all possible 2p
    for(int b=0; b<(100); b++)

      //iterate through all 5p
      for(int c=0; c<(40); c++)

        //iterate through all 10p
        for(int d=0; d<(20); d++)

          //iterate through all 20p
          for(int e=0; e<(10); e++)

            //iterate through all 50p
            for(int f=0; f<(4); f++)

              //iterate through all 100p
              for(int g=0; g<(2); g++)

                if( (a + 2*b + 5*c + 10*d + 20*e + 50*f + 100*g) == 200 ){
                  cout << count << endl;
                  count++;
                }
  cout << count << endl;

  return 0;
}
