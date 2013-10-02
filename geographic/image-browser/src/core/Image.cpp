/**
 * @file    Image.cpp
 * @author  Marvin Smith
 * @date    10/1/2013
*/
#include "Image.hpp"

#include <src/core/FilesystemUtilities.hpp>

using namespace std;


Image::Image( const string& filename ){

    cout << "CONSTRUCTOR START" << endl;
    // set filename 
    this->filename = filename;

    // open up gdal and pull bounding box
    GDALLoader driver( filename );
    bbox = driver.get_bbox();
    cout << "BBox: " << bbox << endl;

    // get the short and long names
    short_name = driver.getShortName();

    long_name  = driver.getLongName();
    
    cout << "CONSTRUCTOR END" << endl;

}

string Image::basename()const{
    return file_basename(filename);
}

string Image::formatShortString()const{

    return short_name;
}

string Image::formatLongString()const{

    return long_name;
}

string Image::canonical()const{
    return file_canonical(filename);
}

Rect Image::getBBox()const{
    return bbox;
}

