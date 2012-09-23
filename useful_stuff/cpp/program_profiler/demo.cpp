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

    profiler.tick("functionA");
    usleep(microsecondTimerLength);
    
    profiler.tick("functionA");
    usleep(microsecondTimerLength);
    
    profiler.tick("functionA");
    usleep(microsecondTimerLength);
    
    profiler.tick("functionA");
    usleep(microsecondTimerLength);
    
    profiler.tick("functionA");

}

int main( int argc, char* argv[] ){

    //register the intervals
    profiler.add_interval("functionA");
    profiler.add_interval("functionB");

    // show in single case
    //profiler.start("functionA");
    functionA( );
    //profiler.stop("functionA");
    
    // show in stepped case
    for( int i=0; i<10; i++){
        functionB( );
        profiler.restart_interval("functionA");
    }
    //profiler.end_interval("functionA");
    
    profiler.print_all();

    return 0;
}
