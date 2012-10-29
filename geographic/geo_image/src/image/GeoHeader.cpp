#include "GeoHeader.hpp"

#include <iostream>
using namespace std;


namespace GEO{

GeoHeader::GeoHeader(){ 
    valid = false;
    pixeltype =  -1;
}

GeoHeader::~GeoHeader(){

}

std::string GeoHeader::get_image_filename()const{
    return image_filename;
}

void GeoHeader::set_image_filename( std::string const& filename ){
    image_filename = filename;
}

void GeoHeader::set_pixel_type( int const& pix){
   
    //copy pixel type
    pixeltype = pix;
    
}


int GeoHeader::get_pixel_type()const{
    
    return pixeltype;
}
      
bool GeoHeader::isValid()const{
    return valid;
}

void GeoHeader::setValid( const bool& flag ){
    valid = flag;
}

/*
void GeoHeader::set_header_data( std::vector<std::pair<std::string, std::string> >const& hdata ){
    header_data = hdata;
}

std::vector<std::pair<std::string,std::string> > GeoHeader::get_header_data()const{
    return header_data;
}

bool GeoHeader::get_header_item( std::string const& key, std::string& result ){
    
    for( size_t i=0; i<header_data.size(); i++ ){
        if( header_data[i].first == key ){
            result = header_data[i].second;
            return true;
        }
    }

    result = "";
    return false;
}

bool GeoHeader::set_header_item( std::string const& key, std::string const& value ){

    bool fndKey = false;
    for( size_t i=0; i<header_data.size(); i++){
        if( header_data[i].first == key ){
            header_data[i].second = value;
            fndKey = true;
        }
    }
    if( fndKey == false )
        header_data.push_back( pair<string,string>(key, value));
    return true;
}*/

}//end of GEO namespace
