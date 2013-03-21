/**
 *  A simple abstraction layer which uses GDAL to open 
 *  image formats not supported by OpenCV.
 *
 * @author Marvin Smith
 * @date 21 March 2013
*/

// OpenCV Dependencies
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// GDAL Dependencies
#include <cpl_conv.h>
#include <gdal_priv.h>

//  Boost Dependencies
#include <boost/filesystem.hpp>

// STL Dependencies
#include <string>

using namespace cv;
using namespace std;

/** 
 * Compute the scale factor for the conversion between image depths
 *
 * @param[in] gdalDepth   - GDAL Depth Type
 * @param[in] opencvDepth - OpenCV Depth Type
 * @return Scale factor between depths
 */
double gdal2OpenCVScale( const int& gdalDepth, const int& opencvDepth ){

    if( opencvDepth == CV_8U  && gdalDepth == GDT_Byte   ) return 1;
    if( opencvDepth == CV_8U  && gdalDepth == GDT_Int16  ) return 1/16.0;
    if( opencvDepth == CV_8U  && gdalDepth == GDT_UInt16 ) return 1/16.0;
    if( opencvDepth == CV_16U && gdalDepth == GDT_Byte   ) return 16;
    if( opencvDepth == CV_16U && gdalDepth == GDT_Int16  ) return 16;
    if( opencvDepth == CV_16U && gdalDepth == GDT_UInt16 ) return 1;
    if( opencvDepth == CV_16S && gdalDepth == GDT_Byte   ) return 16;
    if( opencvDepth == CV_16S && gdalDepth == GDT_Int16  ) return 16;
    if( opencvDepth == CV_16S && gdalDepth == GDT_UInt16 ) return 1;

    throw string("Error: Unknown OpenCV Type or Unknown GDAL Type");
}

/** 
 *  Convert and set the value from the gdal raster to the opencv image
 *
 * @param[in] val Pixel value from GDAL dataset
 * @param[in] image OpenCV Image to load
 * @param[in] point Pixel to set the value to
 * @param[in] scale Scale factor to convert with
*/
void GDAL2OpenCV( double const& val, Mat& image, Point const& point, const double& scale ){
    
    // get the required depth
    int cvDepth = image.depth();

    // get the value for the image
    double pixVal = val * scale;
    
    // place the value
    if( image.depth() == CV_8U ){
        image.at<uchar>(point) = (uchar)pixVal;
    }
    else if( image.depth() == CV_16U ){
        image.at<ushort>(point) = (ushort)pixVal;
    }
    else if( image.depth() == CV_16S ){
        image.at<short>(point) = (short)pixVal;
    }
    else if( image.depth() == CV_32S ){
        image.at<int>(point) = (int)pixVal;
    }
    else if( image.depth() == CV_32F ){
        image.at<float>(point) = (float)pixVal;
    }
    else if( image.depth() == CV_64F ){
        image.at<double>(point) = (double)pixVal;
    }
    else{
        throw string("Error: Unknown Depth");
    }

}

/**
 * Convert Type 2 Depth 
 * 
 * @param[in] type OpenCV Type to convert 
 * @return Associated depth
*/
int cvType2Depth( const int& type ){
    
    switch( type ){

        case CV_8UC1:
            return CV_8U;
        case CV_8UC2:
            return CV_8U;
        case CV_8UC3:
            return CV_8U;
        case CV_16UC1:
            return CV_16U;
        case CV_16UC2:
            return CV_16U;
        case CV_16UC3:
            return CV_16U;
        case CV_16SC1:
            return CV_16S;
        case CV_16SC2:
            return CV_16S;
        case CV_16SC3:
            return CV_16S;
        case CV_32SC1:
            return CV_32S;
        case CV_32SC2:
            return CV_32S;
        case CV_32SC3:
            return CV_32S;
        case CV_32FC1:
            return CV_32F;
        case CV_32FC2:
            return CV_32F;
        case CV_32FC3:
            return CV_32F;
        case CV_64FC1:
            return CV_64F;
        case CV_64FC2:
            return CV_64F;
        case CV_64FC3:
            return CV_64F;
        default:
            return -1;
    }
}


/**
 * Extract the number of channels from the type
 *
 * @param[in] type
 * @return Number of channels
*/
int cvType2Channels( const int& type ){
    
    switch( type ){

        case CV_8UC1:
        case CV_16UC1:
        case CV_16SC1:
        case CV_32SC1:
        case CV_32FC1:
        case CV_64FC1:
            return 1;
        case CV_8UC2:
        case CV_16UC2:
        case CV_16SC2:
        case CV_32SC2:
        case CV_32FC2:
        case CV_64FC2:
            return 2;
        case CV_8UC3:
        case CV_16UC3:
        case CV_16SC3:
        case CV_32SC3:
        case CV_32FC3:
        case CV_64FC3:
            return 3;
    
        default:
            return 0;
    }
}

/**
 * Convert Depth and Channels into Type
 * 
 * @param[in] Depth OpenCV Depth value
 * @param[in] Channels Number of channels
 * @return  OpenCV Type
*/
int cvDepthChannel2Type( const int Depth, const int Channels ){

    if( Depth == CV_8U  && Channels == 3 )  return CV_8UC3;
    if( Depth == CV_8U  && Channels == 2 )  return CV_8UC2;
    if( Depth == CV_8U  && Channels == 1 )  return CV_8UC1;
    if( Depth == CV_16U && Channels == 3 ) return CV_16UC3;
    if( Depth == CV_16U && Channels == 2 ) return CV_16UC2;
    if( Depth == CV_16U && Channels == 1 ) return CV_16UC1;
    if( Depth == CV_16S && Channels == 3 ) return CV_16SC3;
    if( Depth == CV_16S && Channels == 2 ) return CV_16SC2;
    if( Depth == CV_16S && Channels == 1 ) return CV_16SC1;
    if( Depth == CV_32S && Channels == 3 ) return CV_32SC3;
    if( Depth == CV_32S && Channels == 2 ) return CV_32SC2;
    if( Depth == CV_32S && Channels == 1 ) return CV_32SC1;
    if( Depth == CV_64F && Channels == 1 ) return CV_64FC1;

    throw string("Error: combo not supported");

    return 0;
}


/**
 * Checks if the OpenCV Image Type is supported for this. 
 * 
 * 2 channel images are not supported only because I am not sure what
 * types of images these are. 
*/
bool validOpenCVImageType( const int& imageType ){

    switch( imageType ){

        case CV_8UC1:
        case CV_8UC3:
        case CV_16UC1:
        case CV_16UC2:
        case CV_16UC3:
        case CV_16SC1:
        case CV_16SC2:
        case CV_16SC3:
        case CV_32SC1:
        case CV_32SC2:
        case CV_32SC3:
        case CV_32FC1:
        case CV_32FC2:
        case CV_32FC3:
        case CV_64FC1:
        case CV_64FC2:
        case CV_64FC3:
            return true;
        default:
            return false;
    }
}

/**
 * Read an image format using GDAL as the toolset.
 *
 * @param[in] filename Image filename
 * @param[in] flags Type of image you want to read
 */
Mat  imread_geo( const string& filename, const int& imageType ){

    // parse the flags to determine which color type we want to read
    if( validOpenCVImageType(imageType) == false ) throw string("Invalid Image Type");

    // ensure the file exists
    if( boost::filesystem::exists( filename ) == false ){
        throw string( "Error: File ") + filename + string(" does not exist" );
    }
    
    // register the gdal driver
    GDALAllRegister();
    
    // load the dataset 
    GDALDataset*  dataset = (GDALDataset*) GDALOpen( filename.c_str(), GA_ReadOnly);

    // if the dataset returns null, then the file does not exist, or there was a read error
    if( dataset == NULL ){ throw string("Error:  GDAL Dataset returned null from read"); }
    
    // check if pixel data even exists
    if( dataset->GetRasterCount() <= 0 ) throw string("Error: File does not contain pixel data");

    //get the driver infomation
    GDALDriver*  driver = dataset->GetDriver();

    // get raster image size
    Size imgSize( dataset->GetRasterXSize(), dataset->GetRasterYSize() );
    
    // create mats for each raster layer
    vector<Mat> layers( dataset->GetRasterCount() );
    for( size_t i=0; i<layers.size(); i++ )
        layers[i] = Mat( imgSize, cvDepthChannel2Type(cvType2Depth( imageType), 1));

    // iterate through each band
    for (int i = 0; i < dataset->GetRasterCount(); i++) {

        //create the band object
        GDALRasterBand *band = dataset->GetRasterBand(i + 1);

        // get the gdal band type
        int datatype = band->GetRasterDataType();
        
        // compute the scale factor
        double scale = gdal2OpenCVScale( datatype, layers[i].depth());

        //read each image row
        for ( int r = 0; r < layers[i].rows; r++) {

            float* pafScanline;
            pafScanline = (float*) CPLMalloc(sizeof (float) *layers[i].cols);
            band->RasterIO(GF_Read, 0, r, layers[i].cols, 1, pafScanline, layers[i].cols, 1, GDT_Float32, 0, 0);
            
            // iterate through each column
            for ( int c = 0; c < layers[i].cols; c++) {
                GDAL2OpenCV( pafScanline[c], layers[i], Point(c,r), scale );
            }

        }

    }
    
    // close our GDAL Dataset and clean up
    GDALClose( dataset );

    //merge channels into single image
    Mat output;
    if( layers.size() > 1 )
        cv::merge( layers, output );
    else
        output = layers[0].clone();
    
    // do channel conversions if necessary
    if( cvType2Channels(imageType) == output.channels() )
        return output;

    if( cvType2Channels(imageType) == 1 && output.channels() == 3 ){
        cvtColor( output, output, CV_BGR2GRAY );
        return output;
    }
    if( cvType2Channels(imageType) == 3 && output.channels() == 1 ){
        cvtColor( output, output, CV_GRAY2BGR);
        return output;
    }

    // Return the output image
    return output;
}



/**
 * Test Program
*/
int main( int argc, char* argv[] ){

    if( argc < 2 ){
        cerr << "Error: must provide one image name" << endl;
        cerr << "usage:  " << argv[0] << " [input image] [optional: output image]" << endl;
        return 1;
    }

    try{
        
        //  Read the image
        Mat image = imread_geo( argv[1], CV_8UC1 );
        
        //  Save the image
        if( argc > 2 )
            imwrite( argv[2], image );


        namedWindow("IMAGE");
        imshow("IMAGE", image);
        waitKey(0);

    } catch( string e ){
        cout << e << endl;
    }

    return 0;
}


