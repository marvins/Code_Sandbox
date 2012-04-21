#include "GeoHeader.h"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

GeoHeader_Info::GeoHeader_Info(){ }

GeoHeader_Info::~GeoHeader_Info(){ }

std::string GeoHeader_Info::get_image_filename()const{
    return image_filename;
}

void GeoHeader_Info::set_image_filename( std::string const& filename ){
    image_filename = filename;
}

bool GeoHeader_Info::image_filename_exists()const{

   if(fs::exists(fs::path(image_filename)) == true )
      return true;

   return false;
}
