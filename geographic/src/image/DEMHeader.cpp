#include "DEMHeader.h"

#include <iostream>
using namespace std;

namespace GEO{

DEMHeader_Info::DEMHeader_Info(){ 
    image_filename = "_NO_IMAGE_SELECTED_";    
}

DEMHeader_Info::~DEMHeader_Info(){ }

void DEMHeader_Info::copy_header_info( GeoHeader_Info*  other ){

    image_filename = other->get_image_filename();

    setValid( other->isValid() );

}

GeoHeader_Info*& DEMHeader_Info::clone() const{

    GeoHeader_Info* output = new DEMHeader_Info();

    //set valid flag
    output->setValid( isValid() );

    //move over pixel type
    output->set_pixel_type( get_pixel_type());

    //move over image name
    output->set_image_filename(get_image_filename());
    
    return output;
}
   

std::string DEMHeader_Info::get_driver_format() const{
    return "DEM";
}

}
