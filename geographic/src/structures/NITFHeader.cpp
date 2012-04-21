#include "NITFHeader.h"

NITFHeader_Info::NITFHeader_Info(){ }

NITFHeader_Info::~NITFHeader_Info(){ }

void NITFHeader_Info::copy_header_info( GeoHeader_Info*  other ){

   image_filename = other->get_image_filename();

}
