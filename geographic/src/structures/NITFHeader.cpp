#include "NITFHeader_Info.h"

NITFHeader_Info::NITFHeader_Info(){ }

NITFHeader_Info::~NITFHEADER(){ }

void NITFHeader_Info::copy_header_info( GeoHeader_Info*  other ){

   image_filename = other->image_filename;

}
