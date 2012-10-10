
///STL Libraries
#include <deque>
#include <iostream>
#include <map>
#include <string>
#include <vector>

///Personal Libraries
#include "Camera.hpp"
#include "Options.hpp"


///useful namespaces
using namespace std;


/** 
 * Main Driver
 *
 * Implements the primary status interface
*/
int main( int argc, char * argv[] ){
    
    try{

        ///load the configuration here
        Options options( argc, argv );
    
        ///print the configuration
        options.print();
        
        ///find the camera directories
        deque<Camera> cameras = find_camera_directories( options );
        
        ///build a list of image bundles
        map<int,ImageBundle> bundles = compute_image_bundles( cameras, options );


        cout << "pause" << endl;
        cin.get();


    } catch (string e){
        cout << e << endl;
    }

    return 0;
}
