#include "DTEDHeader.h"

#include <iostream>
using namespace std;

namespace GEO{

DTEDHeader_Info::DTEDHeader_Info(){ 
    image_filename = "_NO_IMAGE_SELECTED_";    
}

DTEDHeader_Info::~DTEDHeader_Info(){ }

void DTEDHeader_Info::copy_header_info( GeoHeader_Info*  other ){

    image_filename = other->get_image_filename();

    setValid( other->isValid() );

}

GeoHeader_Info*& DTEDHeader_Info::clone() const{

    GeoHeader_Info* output = new DTEDHeader_Info();

    //set valid flag
    output->setValid( isValid() );

    //move over pixel type
    output->set_pixel_type( get_pixel_type());

    //move over image name
    output->set_image_filename(get_image_filename());
    
    return output;
}
   

std::string DTEDHeader_Info::get_driver_format() const{
    return "DEM.DTED";
}

std::string DTEDHeader_Info::get_gdal_driver_format() const{
    return "DTED";
}

}
