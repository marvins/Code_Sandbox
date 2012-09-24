/**
 * This is a demo of the program profiler
*/
#include <ctime>
#include <iostream>

#include "Profiler.hpp"

using namespace std;

//create profiler
Profiler profiler;


// A random sleep function call
void randomSleep( int useconds, int variation ){
    
    int uTime = std::max(rand()%(2*variation) + (useconds - variation), 0);
    usleep(uTime);
}


// main demo function
int main( int argc, char* argv[] ){
    
    // seed the random number generator
    srand( time( NULL ) );

    //register the interval
    profiler.add_interval("function");

    
    //begin a random loop
    for( int i=0; i<100; i++){

        profiler.tick("function", 0);
        
        randomSleep( 10000, 1000 );

        profiler.tick("function", 1);

        randomSleep( 10000, 1000 );

        profiler.tick("function", 2);

        randomSleep( 10000, 1000 );

        profiler.tick("function", 3);
        
        randomSleep( 10000, 1000 );
        
        profiler.tick("function", 4);

        randomSleep( 10000, 1000 );

        profiler.tick("function", 5);

        profiler.restart_interval("function");
    
    }

    // plot our results
    profiler.build_chart( PROFILER_SCIPY, "output.py");

    return 0;
}
