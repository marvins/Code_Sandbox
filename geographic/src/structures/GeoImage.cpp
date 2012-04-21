#include "GeoImage.h"
#include "../utilities/GDAL2OpenCV.h"
#include "../utilities/OpenCVUtils.h"

using namespace cv;
using namespace std;

/**
 * Default Constructor
*/
GeoImage::GeoImage(): initialize(false), openCVCompatible(false){

    //initialize the header data
    header_data = NULL;

}

/** Parameterized Constructor
 *
 * @param[in] fname image filename
 * @param[in] init initialization flag. If you want to load the image information, 
 * then you can set this equal to true. 
 */
GeoImage::GeoImage(const std::string& fname, const bool& Init ): 
    initialize(Init), openCVCompatible(false)
{  

    //create new header object
    header_data = new NITFHeader_Info();
    
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
GeoImage::GeoImage( const GeoImage& rhs ){

    //create new data object
    header_data = new NITFHeader_Info( );
    header_data->copy_header_info( rhs.header_data );

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
GeoImage::~GeoImage(){
    
    if( header_data == NULL )
        delete header_data;
    
}

/** 
 * Assignment operator (DEEP COPY)
 *
 * @param[in] rhs image to be copied
 */
GeoImage& GeoImage::operator = (const GeoImage& rhs ){

    throw std::string("NOT IMPLMENTED");
    return (*this);
}

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
    if( initialize == true )
        init();
    else
        initialize = false;
}


/** Assign a new image filename
 *
 * @param[in] fname new image filename (Note that the image will not be loaded
 * until you reapply init())
 */
void GeoImage::set_filename( const std::string& fname ){
    header_data->set_image_filename( fname );
}

/**
 * Extract the image filename
 * @return filename of image
 */
std::string GeoImage::get_filename( )const{
    return header_data->get_image_filename( );
}

/** Load The Image Into Memory
 *
 * @brief if you are using GDAL, then it will open, do sanity
 * checking, then load the image dataset.
 */
void GeoImage::load_image(){

    //make sure the image is initialize. This is more of a debugging
    //test as if this false, then I allowed something bad
    if( initialize == false ){
        throw std::string("Error: image not initialized");
    }

    //make sure that the file exists
    if( !header_data->image_filename_exists())
        throw std::string(std::string("Error: Image <") + header_data->get_image_filename() + std::string("> does not exist"));

   
    //initialize GDAL
    GDALAllRegister();

    //open dataset
    try{
        gdal_data.dataset = (GDALDataset*)GDALOpen( header_data->get_image_filename().c_str(), GA_ReadOnly);

        if( gdal_data.dataset == NULL ){
            openCVCompatible = false;
            gdal_data.gdalLoadFailed = true;
            return;
        }

        if( gdal_data.dataset->GetRasterCount() <= 0 ){
            openCVCompatible = false;
        }
    }
    catch(...){
        openCVCompatible = false;
        gdal_data.gdalLoadFailed = true;
    }

    if( gdal_data.gdalLoadFailed == true )
        return;

    //check for pixel data and halt action if it is not present
    if( gdal_data.dataset->GetRasterCount() <= 0 ){
        return;
    }

    openCVCompatible = true;

    //check to make sure its open
    if( gdal_data.dataset == NULL ){
        throw std::string("Dataset did not load");
    }

    //get the driver infomation
    gdal_data.driver = gdal_data.dataset->GetDriver();
}

cv::Size GeoImage::getMatSize()const{

    if( isOpenCVValid() ){
        return cv::Size( gdal_data.dataset->GetRasterBand(1)->GetXSize(), gdal_data.dataset->GetRasterBand(1)->GetYSize());
    }
    else
        return cv::Size(0,0);

}

/** Check if the status of the loading operation was valid */
bool GeoImage::gdal_load()const{
    return gdal_data.gdalLoadFailed;
}


Mat GeoImage::get_image(){

    vector<Mat> imgStack(  gdal_data.dataset->GetRasterCount());
    vector<int> colors(    gdal_data.dataset->GetRasterCount());
    vector<int> depths(    gdal_data.dataset->GetRasterCount());


    for( size_t i=0; i<gdal_data.dataset->GetRasterCount(); i++){

        //create objects
        GDALRasterBand *band;
        band = gdal_data.dataset->GetRasterBand(i+1);

        //get datatype
        depths[i]  = gdal2opencvPixelType( band->GetRasterDataType());

        //get pixeltype
        colors[i] = band->GetColorInterpretation();

        //load pixels
        int cols = band->GetXSize();
        int rows = band->GetYSize();
        Mat timg(Size(cols, rows), cvDepthChannel2Type(depths[i],1));

        double minP = 0;
        double maxP = 0;
        for(size_t r=0; r<rows; r++){

            float* pafScanline;
            pafScanline = (float*)CPLMalloc(sizeof(float)*cols);
            band->RasterIO( GF_Read, 0, r, cols, 1, pafScanline, cols, 1, GDT_Float32, 0, 0);

            for( size_t c=0; c<cols; c++){

                if( r == 0 && c == 0 ){
                    minP = pafScanline[c];
                    maxP = pafScanline[c];
                }
                if( pafScanline[c] > maxP ){
                    maxP = pafScanline[c];
                }
                if( pafScanline[c] < minP ){
                    minP = pafScanline[c];
                }

                if( depths[i] == CV_8U )
                    timg.at<uchar>(r,c) = pafScanline[c];
                else if( depths[i] == CV_16U )
                    timg.at<ushort>(r,c) = pafScanline[c];
                else if( depths[i] == CV_32S )
                    timg.at<int>(r,c) = pafScanline[c];
                else
                    throw std::string("Invalid pixel type");
            }
        }
        if( depths[i] == CV_16U && maxP < 4096 )
            timg = timg.clone()*16;
        else if( depths[i] == CV_16U && maxP < 16384 )
            timg = timg.clone()*4;

        gdal_data.adfMinMax[0] = minP;
        gdal_data.adfMinMax[1] = maxP;
        imgStack[i] = timg.clone();
    }

    //merge channels into single image
    Mat img = merge_bands( imgStack, colors, depths );

    return img;
}

bool GeoImage::isOpenCVValid()const{
    return openCVCompatible;
}

Mat GeoImage::merge_bands( vector<Mat>const& imgStack, vector<int> colors, vector<int> depths )const{

    int nCh = imgStack.size(); 
    Mat image;

    if(imgStack.size() <= 0 )
        throw std::string("Error: imgStack must have at least 1 image");

    int d0 = depths[0];
    for(size_t i=1; i<depths.size(); i++)
        if( d0 != depths[i] ){
            cout << "Error: Depths are not uniform" << endl;
            throw std::string("Error: Depths are not uniform");
        }


    if(nCh == 1)
        return imgStack[0].clone();

    if(nCh == 3){
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

double GeoImage::getMin()const{ return gdal_data.adfMinMax[0]; }
double GeoImage::getMax()const{ return gdal_data.adfMinMax[1]; }


void GeoImage::write_image( const std::string& imgFilename ){


    throw std::string("ERROR: not implemented");

}

std::string GeoImage::getImageTypeName()const{

   if( gdal_data.driver != NULL )
      return gdal_data.driver->GetDescription();
   return "NONE";
}
