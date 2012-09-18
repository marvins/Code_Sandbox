/**
 * This is a demo of the program profiler
*/
#include "Profiler.hpp"

//create profiler
Profiler profiler;

void functionA( ){

    sleep(1);

}

void functionB( ){

    profiler.tick("functionA");
    sleep(1);
    
    profiler.tick("functionA");
    sleep(1);
    
    profiler.tick("functionA");
    sleep(1);
    
    profiler.tick("functionA");
    sleep(1);
    
    profiler.tick("functionA");

}

int main( int argc, char* argv[] ){

    //register the intervals
    profiler.add_interval("functionA");
    profiler.add_interval("functionB");

    // show in single case
    profiler.start("functionA");
    functionA( );
    profiler.stop("functionA");
    
    // show in stepped case
    for( int i=0; i<10; i++){
        profiler.reset_interval("functionA");
        functionB( );
    }
    profiler.end_interval("functionA");

    return 0;
}
