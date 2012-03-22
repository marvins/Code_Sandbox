#include "GeoImage.h"

/**
 * Default Constructor
*/
GeoImage::GeoImage():filename(""), initialize(false), openCVCompat(false), gdalLoadFailed(false){}


/**
 * Parameterized Constructor
 * @param[in] fname Filename to use
 * @param[in] Init Whether or not to initialize image
*/
GeoImage::GeoImage(const string& fname, const bool& Init ): 
    filename(fname), initialize(Init), openCVCompat(false), gdalLoadFailed(false){  init();  }

/** 
 * Initialize image
*/
void GeoImage::init(){
    if( initialize == true )
        load_image();
}

/**
  * Retrief the init state
  * @return initialization state
*/
bool GeoImage::get_init()const{
    return initialize;
}

/**
 * Set the initialization state
 * @param[in] val Value to set state to
*/
void GeoImage::set_init( const bool& val ){
    initialize = val;
    if( initialize == true && filename != "")
        init();
    else
        initialize = false;
}


/**
 * Set the image filename
 * @param[in] fname filename to load 
 */
void GeoImage::set_filename( const string& fname ){
    filename = fname;
}

/**
 * Extract the image filename
 * @return filename of image
 */
string GeoImage::get_filename( )const{
    return filename;
}

/**
 * Load image using GDAL Drivers
 */
void GeoImage::load_image(){
    
    if( initialize == false ){
        throw string("Error: image not initialized");
    }

    //initialize GDAL
    gdalLoadFailed = false;
    GDALAllRegister();

    //open dataset
    try{
        poDataset = (GDALDataset*)GDALOpen(filename.c_str(), GA_ReadOnly);
    
        if( poDataset == NULL ){
            openCVCompat = false;
            gdalLoadFailed = true;
            return;
        }

        if( poDataset->GetRasterCount() <= 0 ){
            openCVCompat = false;
        }
    }
    catch(...){
        openCVCompat = false;
        gdalLoadFailed = true;
    }

    if( gdalLoadFailed == true )
        return;

    //check for pixel data and halt action if it is not present
    if( poDataset->GetRasterCount() <= 0 ){
        return;
    }
    else
        openCVCompat = true;

    //check to make sure its open
    if( poDataset == NULL ){
        throw string("Dataset did not load");
    }
    
    //get the dataset information
    poDataset->GetGeoTransform( adfGeoTransform );
    
    //get the driver infomation
    driver = poDataset->GetDriver();

}

/**
 * Get Driver
*/
GDALDriver* GeoImage::getDriver()const{
    
    if( gdalLoadFailed == true )
        throw string("Image not loaded");

    return driver;
}

bool GeoImage::gdal_load()const{
    return gdalLoadFailed;
}

        
void GeoImage::get_image()const{


}

bool GeoImage::isOpenCVValid()const{
    return openCVCompat;
}

