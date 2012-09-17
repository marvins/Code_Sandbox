#include "ImageHeader.h"

#include <iostream>
using namespace std;

namespace GEO{

ImageHeader_Info::ImageHeader_Info(){ 
    image_filename = "_NO_IMAGE_SELECTED_";    
}

ImageHeader_Info::~ImageHeader_Info(){ }

void ImageHeader_Info::copy_header_info( GeoHeader_Info*  other ){

    image_filename = other->get_image_filename();

    setValid( other->isValid() );

}

GeoHeader_Info*& ImageHeader_Info::clone() const{

    GeoHeader_Info* output = new ImageHeader_Info();

    //set valid flag
    output->setValid( isValid() );

    //move over pixel type
    output->set_pixel_type( get_pixel_type());

    //move over image name
    output->set_image_filename(get_image_filename());
    
    return output;
}
   

std::string ImageHeader_Info::get_driver_format() const{
    return "IMG";
}
std::string ImageHeader_Info::get_gdal_driver_format() const{
    return "IMG";
}

}
