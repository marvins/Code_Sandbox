#include "DEM.h"
#include "../core/Enumerations.h"

#include <iostream>
#include <string>

using namespace std;

namespace GEO{

DEM::DEM( cv::Rect_<float> region, DEM_Params params ){

    cout << "Created DEM" << endl;
    
    /** need to start looking at how many files we need
     * 
     * 1.  DTED uses 1 deg x 1 deg grids
    */
    if( params.filetype == DTED ){
        cout << "loading dted" << endl;

    }
    else{
        throw std::string("Error: unsupported DEM format");
    }


}




}//end of GEO namespace
