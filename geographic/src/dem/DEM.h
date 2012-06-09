#ifndef __SRC_STRUCTURES_DEM_H__
#define __SRC_STRUCTURES_DEM_H__

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <string>

namespace GEO{

class DEM_Params{
    
    public:
        int filetype;
        std::string dted_root_dir;

};

class DEM{

    public:
        DEM( cv::Rect_<float> region, DEM_Params params );

    private:


}; //end of DEM class

} //end of GEO namespace 

#endif

