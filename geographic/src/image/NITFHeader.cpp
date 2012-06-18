#include "NITFHeader.h"

#include <iostream>
using namespace std;

namespace GEO{

NITFHeader_Info::NITFHeader_Info(){ 
    image_filename = "_NO_IMAGE_SELECTED_";    
}

NITFHeader_Info::~NITFHeader_Info(){ }

void NITFHeader_Info::copy_header_info( GeoHeader_Info*  other ){

    image_filename = other->get_image_filename();

    setValid( other->isValid() );

    //move over header data
    set_header_data( other->get_header_data());

}

GeoHeader_Info*& NITFHeader_Info::clone() const{

    GeoHeader_Info* output = new NITFHeader_Info();

    //set valid flag
    output->setValid( isValid() );

    //move over pixel type
    output->set_pixel_type( get_pixel_type());

    //move over image name
    output->set_image_filename(get_image_filename());

    //move over header data
    output->set_header_data( get_header_data());

    return output;
}
   

std::string NITFHeader_Info::get_driver_format() const{
    return "IMG.NITF";
}

}
