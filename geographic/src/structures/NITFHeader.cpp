#include "NITFHeader.h"

NITFHeader_Info::NITFHeader_Info(){ 
    image_filename = "_NO_IMAGE_SELECTED_";    
}

NITFHeader_Info::~NITFHeader_Info(){ }

void NITFHeader_Info::copy_header_info( GeoHeader_Info*  other ){

   image_filename = other->get_image_filename();

}
