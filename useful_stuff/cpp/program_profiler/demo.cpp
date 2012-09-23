/**
 * This is a demo of the program profiler
*/
#include "Profiler.hpp"

//create profiler
Profiler profiler;

//define a set length for the timer
int microsecondTimerLength = 100000;

void functionA( ){

    usleep(microsecondTimerLength);

}

void functionB( ){

    profiler.tick("functionA", 0);
    usleep(microsecondTimerLength);
    
    profiler.tick("functionA", 1);
    usleep(microsecondTimerLength);
    
    profiler.tick("functionA", 2);
    usleep(microsecondTimerLength);
    
    profiler.tick("functionA", 3);
    usleep(microsecondTimerLength);
    
    profiler.tick("functionA", 5);

}

int main( int argc, char* argv[] ){

    //register the intervals
    profiler.add_interval("functionA");
    profiler.add_interval("functionB");

    // show in stepped case
    for( int i=0; i<10; i++){
        functionB( );
        profiler.restart_interval("functionA");
    }
    //profiler.end_interval("functionA");
    
    profiler.print_all();
    
    profiler.build_chart( PROFILER_SCIPY, "output.py");

    return 0;
}
