#include "NITFHeader.h"

NITFHeader_Info::NITFHeader_Info(){ 
    image_filename = "_NO_IMAGE_SELECTED_";    
}

NITFHeader_Info::~NITFHeader_Info(){ }

void NITFHeader_Info::copy_header_info( GeoHeader_Info*  other ){

   image_filename = other->get_image_filename();

}

GeoHeader_Info*& NITFHeader_Info::clone() const{
    
    GeoHeader_Info* output = new NITFHeader_Info();
    PixelType *pix = get_pixel_type();
    output->set_image_filename(get_image_filename());
    output->set_pixel_type(pix);
    
    return output;
}
                
std::string NITFHeader_Info::get_driver_format() const{
    return "NITF";
}