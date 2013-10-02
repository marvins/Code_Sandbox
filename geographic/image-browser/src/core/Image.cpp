/**
 * @file    Image.cpp
 * @author  Marvin Smith
 * @date    10/1/2013
*/
#include "Image.hpp"

#include <src/core/FilesystemUtilities.hpp>

using namespace std;


Image::Image( const string& filename ){

    // set filename 
    this->filename = filename;

    // open up gdal and pull bounding box
    driver = new GDALLoader( filename );

}

string Image::basename()const{
    return file_basename(filename);
}

string Image::formatShortString()const{

    return driver->getShortName();
}

string Image::formatLongString()const{

    return driver->getLongName();
}

