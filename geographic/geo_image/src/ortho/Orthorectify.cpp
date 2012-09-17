#include "Orthorectify.hpp"

#include <opencv2/core/core.hpp>

using namespace cv;

namespace GEO{
namespace ORTHO{


GEO::GeoImage& orthorectify( GeoImage const& input_image ){

    //create an output GeoImage
    GEO::GeoImage output_image;
    
    //gather some relevant information from the image
    //CameraParameters params = GEO::ORTHO::retrieve_camera_parameters( input_image );
    
    //grab the OpenCV Frame
    //Mat scene = input_image.get_image();

    return output_image;
}


} // End of ORTHO Namespace 
} // End of GEO Namespace 
