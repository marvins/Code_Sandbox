#include <GDAL2OpenCV.h>
#include <OpenCVUtils.h>

#include "GeoImage.h"

#include <boost/filesystem.hpp>

using namespace cv;
using namespace std;

namespace bf = boost::filesystem;

namespace GEO{

/**
 * Default Constructor
 */
GeoImage::GeoImage() : initialize(false), openCVCompatible(false) {
    
    //initialize the header data
    header_data = new NITFHeader_Info();

}

/** Parameterized Constructor
 *
 * @param[in] fname image filename
 * @param[in] Init initialization flag. If you want to load the image information, 
 * then you can set this equal to true. 
 */
GeoImage::GeoImage(const std::string& fname, const bool& Init) :
initialize(Init), openCVCompatible(false) {
    
    //figure out which type of image you are
    int imtype = getFileType( fname ); 
    
    if(      imtype == DTED )
        header_data = new DTEDHeader_Info();

    else if( imtype == NITF )
        header_data = new NITFHeader_Info();
    
    else if( imtype == SRTM )
        header_data = new SRTMHeader_Info();
    
    else
        throw string("TYPE FAILED");
    
    //set filename
    header_data->set_image_filename(fname);
    
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
    header_data = new NITFHeader_Info();
    header_data->copy_header_info(rhs.header_data);

    //set initialize flag
    initialize = rhs.initialize;

    //call initialize
    init();
}

/**
 * GeoImage destructor
 * 
 * @brief Deallocates the GDAL datasets
 */
GeoImage::~GeoImage() {

    if (header_data == NULL)
        delete header_data;

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
        load_image();
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
    header_data->set_image_filename(fname);
}

/**
 * Extract the image filename
 * @return filename of image
 */
std::string GeoImage::get_filename()const {
    return header_data->get_image_filename();
}

/** Load The Image Into Memory
 *
 * @brief if you are using GDAL, then it will open, do sanity
 * checking, then load the image dataset.
 */
void GeoImage::load_image() {

    //make sure the image is initialize. This is more of a debugging
    //test as if this false, then I allowed something bad
    if (initialize == false) {
        throw std::string("Error: image not initialized");
    }

    if (header_data->get_image_filename() == "" || header_data->get_image_filename() == "_NO_IMAGE_SELECTED_") {
        initialize = false;
        return;
    }

    //make sure that the file exists
    if (!header_data->image_filename_exists()) {
        cout << "warning: Image <" << header_data->get_image_filename() << "> does not exist" << endl;
        initialize = false;
        return;
    }

    //initialize GDAL
    GDALAllRegister();
    header_data->setValid( true );

    //open dataset
    try {
        gdal_data.dataset = (GDALDataset*) GDALOpen(header_data->get_image_filename().c_str(), GA_ReadOnly);

        if (gdal_data.dataset == NULL) {
            openCVCompatible = false;
            gdal_data.gdalLoadFailed = true;
            return;
        }

        if (gdal_data.dataset->GetRasterCount() <= 0) {
            openCVCompatible = false;
        }
    }    catch (...) {
        openCVCompatible = false;
        gdal_data.gdalLoadFailed = true;
    }

    if (gdal_data.gdalLoadFailed == true)
        return;

    //check for pixel data and halt action if it is not present
    if (gdal_data.dataset->GetRasterCount() <= 0) {
        return;
    }

    openCVCompatible = true;
    gdal_data.gdalLoadFailed = false;

    //check to make sure its open
    if (gdal_data.dataset == NULL) {
        throw std::string("Dataset did not load");
    }

    //get the driver infomation
    gdal_data.driver = gdal_data.dataset->GetDriver();
    
    int depth = gdal2opencvPixelType(gdal_data.dataset->GetRasterBand(1)->GetRasterDataType());
    
    PixelType pixelToSet;

    if ( depth == CV_8U )
        pixelToSet.set( PixelType::UInt8C1 );

    else if ( depth == CV_16U )
        pixelToSet.set( PixelType::UInt16C1 );

    else if ( depth == CV_16S )
        pixelToSet.set( PixelType::Int16C1 );

    else 
        throw string(string("Unknown pixel depth: ") + opencvDepth2string(depth));
   
    header_data->set_pixel_type( pixelToSet );

}

/**
  * Get the Mat image size
  *
  * @return size of GDAL Image
*/
cv::Size GeoImage::getMatSize()const {

    if (isOpenCVValid()) {
        return cv::Size(gdal_data.dataset->GetRasterBand(1)->GetXSize(), gdal_data.dataset->GetRasterBand(1)->GetYSize());
    } else
        return cv::Size(0, 0);

}

/** Check if the status of the loading operation was valid */
bool GeoImage::gdal_load()const {
    return !gdal_data.gdalLoadFailed;
}

/**
  * Get the OpenCV Image from GDAL
  *
  * @return OpenCV Image.
*/
Mat GeoImage::get_image() {

    //make sure the image has been initialized
    if( openCVCompatible == false ){
        return Mat();
    }

    vector<Mat> imgStack(gdal_data.dataset->GetRasterCount());
    vector<int> colors(gdal_data.dataset->GetRasterCount());
    vector<int> depths(gdal_data.dataset->GetRasterCount());


    for (int i = 0; i < gdal_data.dataset->GetRasterCount(); i++) {

        //create objects
        GDALRasterBand *band;
        band = gdal_data.dataset->GetRasterBand(i + 1);

        //get datatype
        depths[i] = gdal2opencvPixelType(band->GetRasterDataType());

        //get pixeltype
        colors[i] = band->GetColorInterpretation();

        //load pixels
        int cols = band->GetXSize();
        int rows = band->GetYSize();
        Mat timg(Size(cols, rows), cvDepthChannel2Type(depths[i], 1));

        double minP = 0;
        double maxP = 0;
        for ( int r = 0; r < rows; r++) {

            float* pafScanline;
            pafScanline = (float*) CPLMalloc(sizeof (float) *cols);
            band->RasterIO(GF_Read, 0, r, cols, 1, pafScanline, cols, 1, GDT_Float32, 0, 0);
            
            for ( int c = 0; c < cols; c++) {

                if (r == 0 && c == 0) {
                    minP = pafScanline[c];
                    maxP = pafScanline[c];
                }
                if (pafScanline[c] > maxP) {
                    maxP = pafScanline[c];
                }
                if (pafScanline[c] < minP) {
                    minP = pafScanline[c];
                }
                
                //8 Bit Unsigned Character
                if (depths[i] == CV_8U)
                    timg.at<uchar > (r, c) = pafScanline[c];
                
                //16 bit Unsigned Character
                else if (depths[i] == CV_16U)
                    timg.at<ushort > (r, c) = pafScanline[c];
                
                //16 bit Signed Character
                else if (depths[i] == CV_16S)
                    timg.at<short>(r,c) = pafScanline[c];
                
                //32 bit signed integer
                else if (depths[i] == CV_32S)
                    timg.at<int>(r, c) = pafScanline[c];
                
                else
                    throw std::string("Invalid pixel type");
                
            }
            
        }

        if (depths[i] == CV_16U && maxP < 4096)
            timg = timg.clone()*16;
        
        else if (depths[i] == CV_16U && maxP < 16384)
            timg = timg.clone()*4;

        gdal_data.adfMinMax[0] = minP;
        gdal_data.adfMinMax[1] = maxP;
        imgStack[i] = timg.clone();
    }

    //merge channels into single image
    Mat img = merge_bands(imgStack, colors, depths);

    return img;
}

/**
  * Check if an image is in a valid OpenCV Format.
  *
  * @return true if image is available for OpenCV
*/
bool GeoImage::isOpenCVValid()const {
    return openCVCompatible;
}

/** Merge multiple GDAL Bands into a multi-channel OpenCV Image
 *
 * @param[in] imgStack Array of images loaded from GDAL
 * @param[in] colors   Array of OpenCV interpreted GDAL Band colors
 * @param[in] depths   Array of OpenCV interpreted GDAL Band pixel depths
 * @return merged image
 */
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
*/
double GeoImage::getMin()const {
    return gdal_data.adfMinMax[0];
}

/**  
  * Return the max pixel value of the image
  *
  * @return max pixel value (double)
*/
double GeoImage::getMax()const {
    return gdal_data.adfMinMax[1];
}

void GeoImage::write_image(const std::string& imgFilename) {


    /** Create output dataset */
    gdal_data.write(imgFilename, "NITF");

}

/** 
 * Get the type of image
 *
 * @return name of driver (ex GDAL)
 */
std::string GeoImage::getImageTypeName()const {

    if (gdal_data.driver != NULL)
        return gdal_data.driver->GetDescription();
    return "NONE";
}

/**
  *  Return a copy of the header data. 
  *
  * @return Header info
*/
GeoHeader_Info*& GeoImage::get_header()const {

    return header_data->clone();
}

/**
 * Get the type of file
*/
int GeoImage::getFileType( const string& fname ){

    //extract the extension
    std::string ext = bf::path(fname).extension().string();

    if( ( ext == ".dt2" ) == true ) return DTED;
    if( ( ext == ".ntf" ) == true ) return NITF;
    if( ( ext == ".NTF" ) == true ) return NITF;
    
    throw string("ERROR: Unknown type");
}

Point2d getElevationMeters( Point2d coordinate ){


    //first we need to solve for which image this coordinate will belong to
    //  - round number to nearest integer
    int iLat = std::floor(coordinate.y + 0.5 );
    int lLon = std::floor(coordinate.x + 0.5 );

    // - check if point is in southern or western hemisphere
    bool northern = true;
    bool eastern  = true;
    if( iLat < 0 ){   
        iLat = (iLat * -1)+1;
        northern = false;
    }
    if( iLon < 0 ){
        iLon = (iLon * -1)+1;
        eastern = false;
    }

    // - compute the appropriate filename for the dted file



    Point2d output;
    return output;

}

} //end of GEO namespace 
