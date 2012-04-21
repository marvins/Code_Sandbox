#include "GeoHeader_Info.h"

GeoHeader_Info::GeoHeader_Info(){ }

GeoHeader_Info::~GeoHeader_Info(){ }

std::string GeoHeader_Info::get_image_filename()const{
    return image_filename;
}

void GeoHeader_Info::set_image_filename( std::string const& filename ){
    image_filename = filename;
}
