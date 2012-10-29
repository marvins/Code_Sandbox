/**
 * @file GeoImage.cpp
 * @author Marvin Smith
*/
//#include <GDAL2OpenCV.h>
//#include <OpenCVUtils.h>

#include "GeoImage.hpp"

#include "../utilities/File_Utilities.hpp"

#include "../core/Enumerations.hpp"

//#include "CoordinateLatLon.h"

//#include <boost/filesystem.hpp>

using namespace cv;
using namespace std;

//namespace bf = boost::filesystem;

namespace GEO{

/**
 * Default Constructor
 */
GeoImage::GeoImage() : initialize(false){
    
}



/** Parameterized Constructor
 *
 * @param[in] fname image filename
 * @param[in] Init initialization flag. If you want to load the image information, 
 * then you can set this equal to true. 
 */
GeoImage::GeoImage(const std::string& fname, const bool& Init) :
                              initialize(Init){
    
    //figure out which type of image you are
    int imtype = getFileType( fname ); 
    header_data.image_type = imtype;

    //set filename
    header_data.set_image_filename(fname);
    
    init();

}

/** Copy Constructor
 *
 * @brief Note that this performs a deep copy. In fact,
 * it merely transfers any pre-loading information, then 
 * reloads the image information using GDAL
 *
 * @param[in] rhs Image to be copied
 */
GeoImage::GeoImage(const GeoImage& rhs) {

    //create new data object
    //header_data.copy_header_info(rhs.header_data);

    //set initialize flag
    initialize = rhs.initialize;

    //call initialize
    init();

    throw string( "ERROR: Not fully implemented" );
}

/**
 * GeoImage destructor
 * 
 * @brief Deallocates the GDAL datasets
 */
GeoImage::~GeoImage() {


}

/** 
 * Assignment operator (DEEP COPY)
 *
 * @param[in] rhs image to be copied
 */
GeoImage& GeoImage::operator =(const GeoImage& rhs) {

    throw std::string("NOT IMPLMENTED");
    return (*this);
}

/** 
 * Initialize image
 */
void GeoImage::init() {
    if (initialize == true)
        load_data();
}

/**
 * Retrief the init state
 * @return initialization state
 */
bool GeoImage::get_init()const {
    return initialize;
}

/**
 * Set the initialization state
 * @param[in] val Value to set state to
 */
void GeoImage::set_init(const bool& val) {
    initialize = val;
    if (initialize == true)
        init();
    else
        initialize = false;
}

/** Assign a new image filename
 *
 * @param[in] fname new image filename (Note that the image will not be loaded
 * until you reapply init())
 */
void GeoImage::set_filename(const std::string& fname) {
    header_data.set_image_filename(fname);
}

/**
 * Extract the image filename
 * @return filename of image
 */
std::string GeoImage::get_filename()const {
    return header_data.get_image_filename();
}

/** Load The Image Into Memory
 *
 * @brief if you are using GDAL, then it will open, do sanity
 * checking, then load the image dataset.
 */
void GeoImage::load_data() {

    //make sure the image is initialize. This is more of a debugging
    //test as if this false, then I allowed something bad
    if (initialize == false) {
        throw std::string("Error: image not initialized");
    }

    if (header_data.get_image_filename() == "" || header_data.get_image_filename() == "_NO_IMAGE_SELECTED_") {
        initialize = false;
        return;
    }

    //make sure that the file exists
    if ( !STR::file_exists( header_data.get_image_filename() ) ) {
        cout << "warning: Image <" << header_data.get_image_filename() << "> does not exist" << endl;
        initialize = false;
        return;
    }

    //open dataset
    try {
        
        // initialize the dataset
        gdal_data.initialize( header_data.get_image_filename() );
        
        // ensure the data is valid
        if( gdal_data.isValid( ) == false )
            return;


    }    
    catch (...) {
        return;
    }

    if (gdal_data.isValid() == false )
        return;
    
    if (gdal_data.isOpenCVCompatible() == false )
        return;

    header_data.set_pixel_type( gdal_data.getOpenCVPixelType( ) );
    
}

/**
  * Get the Mat image size
  *
  * @return size of GDAL Image
*
cv::Size GeoImage::getMatSize()const {

    if (isOpenCVValid()) {
        return cv::Size(gdal_data.dataset->GetRasterBand(1)->GetXSize(), gdal_data.dataset->GetRasterBand(1)->GetYSize());
    } else
        return cv::Size(0, 0);

}

/** Check if the status of the loading operation was valid 
 *
bool GeoImage::gdal_load()const {
    return gdal_data.get_status();
}

/**
  * Get the OpenCV Image from GDAL
  *
  * @return OpenCV Image.
*/
Mat GeoImage::get_image() {

    //make sure the image has been initialized
    if( gdal_data.isOpenCVCompatible() == false ){
        return Mat();
    }
    
    //return the image
    return get_image( header_data.get_pixel_type() );

}


cv::Mat GeoImage::get_image( const int& imtype ){
    
    //make sure the image has been initialized
    if( gdal_data.isOpenCVCompatible() == false ){
        return Mat();
    }
    
    return gdal_data.getOpenCVMat( imtype );
}


//void GeoImage::set_image( const Mat& img ){
//    gdal_data.set_img_data( img ); 
//}


/**
  * Check if an image is in a valid OpenCV Format.
  *
  * @return true if image is available for OpenCV
*/
bool GeoImage::isOpenCVValid()const {
    return gdal_data.isOpenCVCompatible( );
}

/**
 * Check if the data loaded and is valid
*/
bool GeoImage::isDataValid( )const{
    return gdal_data.isValid();
}

/** Merge multiple GDAL Bands into a multi-channel OpenCV Image
 *
 * @param[in] imgStack Array of images loaded from GDAL
 * @param[in] colors   Array of OpenCV interpreted GDAL Band colors
 * @param[in] depths   Array of OpenCV interpreted GDAL Band pixel depths
 * @return merged image
 *
Mat GeoImage::merge_bands(vector<Mat>const& imgStack, vector<int> colors, vector<int> depths)const {

    int nCh = imgStack.size();
    Mat image;

    if (imgStack.size() <= 0)
        throw std::string("Error: imgStack must have at least 1 image");

    int d0 = depths[0];
    for (size_t i = 1; i < depths.size(); i++)
        if (d0 != depths[i]) {
            cout << "Error: Depths are not uniform" << endl;
            throw std::string("Error: Depths are not uniform");
        }


    if (nCh == 1)
        return imgStack[0].clone();

    if (nCh == 3) {
        Mat imgOut;

        //assuming RGB
        vector<Mat> trueSet(3);
        trueSet[0] = imgStack[2].clone();
        trueSet[1] = imgStack[1].clone();
        trueSet[2] = imgStack[0].clone();
        merge(trueSet, imgOut);

        return imgOut.clone();
    }

    return imgStack[0].clone();

}

/**
  * Return the min pixel value of the image
  *
  * @return min pixel value (double)
*
double GeoImage::getMin()const {
    return gdal_data.adfMinMax[0];
}

/**  
  * Return the max pixel value of the image
  *
  * @return max pixel value (double)
*
double GeoImage::getMax()const {
    return gdal_data.adfMinMax[1];
}

void GeoImage::write_image(const std::string& imgFilename) {


    /** Create output dataset *
    if( imgFilename == "__NONE__" )
        gdal_data.write( get_filename(), "NITF" );
    else
        gdal_data.write(imgFilename, "NITF");

}

/** 
 * Get the type of image
 *
 * @return name of driver (ex GDAL)
 */
std::string GeoImage::getImageTypeName()const {

    return gdal_data.getImageTypeName( );
}

/**
  *  Return a copy of the header data. 
  *
  * @return Header info
*
GeoHeader_Info*& GeoImage::get_header()const {

    return header_data->clone();
}

/**
 * Get the type of file
*/
int GeoImage::getFileType( const string& fname ){

    //extract the extension
    std::string ext = STR::file_extension(fname);

    if( ( ext == ".dt2" ) == true ) return DTED;
    if( ( ext == ".ntf" ) == true ) return NITF;
    if( ( ext == ".NTF" ) == true ) return NITF;
    
    return UNKNOWN;
}

/** Modify the header of the image 
 *  ACTION PARAMETERS
 *  1.  SEARCH FOR TAG AND REPLACE VALUE
 *  2.  SEARCH FOR VALUE AND REPLACE TAG
 *  3.  ADD TAG AND VALUE PAIR
 *  4.  DELETE FIRST ENTRY WHICH MATCHES TAG
 *  5.  DELETE FIRST ENTRY WHICH MATCHES VALUE
 *
void GeoImage::modify_header_metadata( const string& tag, const string& val, const int& action ){
    
    if( action == 1 ){
        cout << "Setting metadata" << endl;
        gdal_data.driver->SetMetadataItem(tag.c_str(),val.c_str()); 
    }

}
    
void GeoImage::get_corner_coordinates( CoordinateLatLon& ul, CoordinateLatLon& br ){

    gdal_data.get_corner_coordinates( ul.lon, ul.lat, br.lon, br.lat );

}


void GeoImage::get_corner_coordinates( Point2f& ul, Point2f& br )const{
    
    double ulx, uly, brx, bry;
    gdal_data.get_corner_coordinates( uly, ulx, bry, brx );
    ul.y=uly;
    ul.x=ulx;
    br.y=bry;
    br.x=brx;
}

void GeoImage::clean(){
    if (header_data == NULL)
        delete header_data;
    header_data = new NITFHeader_Info();

    gdal_data.clean();
}

string GeoImage::get_tre()const{

    char ** tre_info = NULL;
    
    string tre_output = "";
    char ** results = gdal_data.dataset->GetMetadata("TRE");
    
    int idx = 0;
    if( results != NULL ){
        
        while( results[idx] != NULL ){
            tre_output += results[idx++];
        }
    }

    return tre_output;

}
*/


}
