#include "NITFHeader.h"

#include <iostream>
using namespace std;

NITFHeader_Info::NITFHeader_Info(){ 
    image_filename = "_NO_IMAGE_SELECTED_";    
}

NITFHeader_Info::~NITFHeader_Info(){ }

void NITFHeader_Info::copy_header_info( GeoHeader_Info*  other ){

   image_filename = other->get_image_filename();

}

GeoHeader_Info*& NITFHeader_Info::clone() const{

    GeoHeader_Info* output = new NITFHeader_Info();
    
    //move over pixel type
    output->set_pixel_type( get_pixel_type());

    //move over image name
    output->set_image_filename(get_image_filename());
    
    return output;
}
   

std::string NITFHeader_Info::get_driver_format() const{
    return "NITF";
}
