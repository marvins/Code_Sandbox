#include "NITF_Image.h"

/**
 * Default Constructor
*/
NITF_Image::NITF_Image():filename(""), initialize(false){}


/**
 * Parameterized Constructor
 * @param[in] fname Filename to use
 * @param[in] init Whether or not to initialize image
*/
NITF_Image::NITF_Image(const string& fname, const bool& init ): 
    filename(fname), initialize(init){  }



/**
 * Set the image filename
 * @param[in] fname filename to load 
*/
void NITF_Image::set_filename( const string& fname ){
    filename = fname;
}

/**
 * Extract the image filename
 * @return filename of image
*/
string NITF_Image::get_filename( )const{
    return filename;
}

