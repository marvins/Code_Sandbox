#ifndef __SRC_OPTIONS_HPP__
#define __SRC_OPTIONS_HPP__

///STL Libraries
#include <iostream>

///useful includes
using namespace std;


/** 
 * Useful enumerations
*/
const int CAMERA_EO = 0;
const int CAMERA_IR = 1;


/**
 * @class Options
 *
 * The required configuration parameters for the program.
*/
class Options{

    public:

        Options( const int argc, char** argv ){

            cout << "Hello World" << endl;

        }
        

        void print()const{

            cout << "print" << endl;
        }

};


#endif
