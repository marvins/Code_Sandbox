/**
 * @file    ImageDatabase.cpp
 * @author  Marvin Smith
 * @date    10/1/2013
*/
#include <src/core/ImageDatabase.hpp>

using namespace std;

/**
 * Default Constructor
*/
ImageDatabase::ImageDatabase(){


}


/**
 * Clear all images from the database
*/
void ImageDatabase::clear(){
    data.clear();
}

/**
 * Return the size of the database
*/
size_t ImageDatabase::size()const{
    return data.size();
}

/**
 * Add an image to the database
*/
void ImageDatabase::addItem( const string& filename ){
    data.push_back(Image(filename));
}

/**
 * Return a pointer to the image
*/
Image& ImageDatabase::operator[]( const int& idx ){
    return data[idx];
}

/**
 * Return a copy of the image
*/
Image ImageDatabase::operator[]( const int& idx )const{
    return data[idx];
}

