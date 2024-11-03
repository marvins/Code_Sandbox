/**
 * @file  GDAL_Driver.cpp
 * @author Marvin Smith
 * @date  7/4/2013
*/
#include "GDAL_Driver.hpp"

#include <src/core/Enumerations.hpp>
#include <src/utilities/Filesystem_Utilities.hpp>
#include <src/utilities/OpenCV_Utilities.hpp>

#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>

// Boost Libraries
#include <boost/algorithm/string.hpp>

using namespace cv;
using namespace std;

namespace GIS{

/**
 * Default Constructor
*/
GDAL_Driver::GDAL_Driver( ){
	set_defaults();
}

/** 
* Destructor
*/
GDAL_Driver::~GDAL_Driver(){
    
    // if dataset is not null
    if( dataset != NULL ){
        GDALClose(dataset);
        dataset = NULL;
    }
}


/**
 * Create the driver and initialize if necessary
*/
GDAL_Driver::GDAL_Driver( const string& filename, bool const& init ){

	// set the defaults for the class
	set_defaults();
	
	// set the filename
	this->filename = filename;
	
	// initialize the driver if requested
	if( init == true ){
		initialize();
	} 

}
        
/**
 * Set image filename
*/
void GDAL_Driver::set_filename( std::string image_filename ){
    
    // set the filename
    filename = image_filename;

}

/**
 * Initialize dataset
*/
void GDAL_Driver::initialize(){

	// make sure the file actually exists
	if( file_exists( filename ) == false ){
		throw string("Error:  File does not exist");
	}
    
	// register the gdal driver
    GDALAllRegister();

	// load the dataset 
    dataset = (GDALDataset*) GDALOpen( filename.c_str(), GA_ReadOnly);
	
    driver_initialized=true;
	
    // if dataset is null, then there was a problem
	if( dataset == NULL ){
        cout << "Driver Null" << endl;
		driver_initialized = false;
		return;
	}
	
	// make sure we have pixel data inside the raster
	if( dataset->GetRasterCount() <= 0 ){
        cout << "No Pixel Data inside Image" << endl;
		driver_initialized = false;
		return;
	}
	
	//extract the driver infomation
    driver = dataset->GetDriver();
	

}

pair<int,int>  GDAL_Driver::getRasterDimensions( )const{
        
    return pair<int,int>( dataset->GetRasterXSize(), dataset->GetRasterYSize() );

}
	
Mat GDAL_Driver::getOpenCVMat( ){
    
    // skip if the driver is not initialized
	if( driver_initialized == false ){
		return Mat();
	}
    
    int ptype;
    if( dataset->GetRasterCount() == 1 ){ ptype = CV_8UC1; }
    if( dataset->GetRasterCount() == 3 ){ ptype = CV_8UC3; }
    if( dataset->GetRasterCount() == 4 ){ ptype = CV_8UC4; }

    // return the image
    return getOpenCVMat( ptype );
}

int compute_pixeltype( const vector<int>& pixels ){
    
    // check for rgb
    bool r=false, g=false, b=false, a=false;
    for( int i=0; i<(int)pixels.size(); i++ ){
        if( pixels[i] == GDAL_RED  ) r = true;
        if( pixels[i] == GDAL_BLUE ) b = true;
        if( pixels[i] == GDAL_GREEN) g = true;
        if( pixels[i] == GDAL_ALPHA) a = true;
    }
    
    // check for rgba
    if( r == true && g == true && b == true ){
        if( a == true ){ return PIXEL_BGRA; }
        else
            return PIXEL_BGR;
    }

   // otherwise grayscale
   return PIXEL_GRAY;

}

std::string GDAL_Driver::get_image_format()const{
    return image_format;
}

Mat GDAL_Driver::getOpenCVMat( const int& pixType ){
    
	// skip if the driver is not initialized
	if( driver_initialized == false ){
		return Mat();
	}
    	
	// get relevant image data
    pair<int,int> dims =  getRasterDimensions();
    
	// create an output image
    vector<Mat> layers( dataset->GetRasterCount() );
    for( size_t i=0; i<layers.size(); i++ )
        layers[i] = Mat( dims.second, dims.first, cvDepthChannel2Type(cvType2Depth( pixType), 1));
        
        
    // determine input color type
    vector<int> color_list;
    image_format = "";
    for( int i=0; i<dataset->GetRasterCount(); i++ ){
        int val = dataset->GetRasterBand(1+i)->GetColorInterpretation();
        //cout << val << endl;

        if( val == GDAL_RED   )      image_format.push_back( 'r' );
        else if( val == GDAL_GREEN ) image_format.push_back( 'g' );
        else if( val == GDAL_BLUE  ) image_format.push_back( 'b' );
        else if( val == GDAL_ALPHA ) image_format.push_back( 'a' );
        else if( val == GDAL_GRAY  ) image_format.push_back( 'G' );
        else if( val == GDAL_UNKNOWN ) image_format.push_back( 'G' );
        else{ cout << "ERROR: Unknown type " << val << endl; throw string(); }
    }

    // iterate through each band
    for (int i = 0; i < dataset->GetRasterCount(); i++) {
        
        //create objects
        GDALRasterBand *band = dataset->GetRasterBand(i + 1);

        int datatype = band->GetRasterDataType();

        //load pixels
        double minP = 0;
        double maxP = 0;
        for ( int r = 0; r < layers[i].rows; r++) {
                
            float* pafScanline;
            pafScanline = (float*) CPLMalloc(sizeof (float) *layers[i].cols);
            band->RasterIO(GF_Read, 0, r, layers[i].cols, 1, pafScanline, layers[i].cols, 1, GDT_Float32, 0, 0);
            
            for ( int c = 0; c < layers[i].cols; c++) {

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
                    
                double value = GDAL2OpenCV( pafScanline[c], layers[i].depth(), datatype );
                // set the pixel value
                cvSetPixel( layers[i], Point(c,r), value );
            }
         
        }
        if (layers[i].depth() == CV_16U && maxP < 4096)
            layers[i] = layers[i].clone()*16;
        
        else if (layers[i].depth() == CV_16U && maxP < 16384)
            layers[i] = layers[i].clone()*4;
    }

    // if needed, swap the alpha channel around
    //if( dataset->GetRasterCount() >= 3 ){
    //    Mat tmp = layers[0].clone();
    //    layers[0] = layers[2].clone();
    //    layers[2] = tmp.clone();
    //}

    //merge channels into single image
    Mat output;
    if( layers.size() > 1 )
        cv::merge( layers, output );
    else
        output = layers[0].clone();
    
    // check if we need a type conversion
    //if( cvType2Channels(pixType) == 3 && output.channels() == 1 && cvType2Depth(pixType) == output.depth()){
    //    cvtColor( output, output, CV_GRAY2BGRA);
    //}
    //else if( cvType2Channels(pixType) == 3 && output.channels() == 1 && cvType2Depth(pixType) == output.depth()){
    //    cvtColor( output, output, CV_GRAY2BGR);
    //}

    return output;

}


void GDAL_Driver::set_defaults(){
	
	driver_initialized = false;
	
}

int GDAL_Driver::getOpenCVPixelType( )const{
        
    int gdalType  = dataset->GetRasterBand(1)->GetRasterDataType();
    int nchannels = get_num_channels( );

    if( gdalType == GDT_Byte   && nchannels == 1 ) return CV_8UC1;
    if( gdalType == GDT_Byte   && nchannels == 2 ) return CV_8UC2;
    if( gdalType == GDT_Byte   && nchannels == 3 ) return CV_8UC3;
    if( gdalType == GDT_UInt16 && nchannels == 1 ) return CV_16UC1;
    if( gdalType == GDT_UInt16 && nchannels == 2 ) return CV_16UC2;
    if( gdalType == GDT_UInt16 && nchannels == 3 ) return CV_16UC3;
    if( gdalType == GDT_Int16  && nchannels == 1 ) return CV_16SC1;
    if( gdalType == GDT_Int16  && nchannels == 2 ) return CV_16SC2;
    if( gdalType == GDT_Int16  && nchannels == 3 ) return CV_16SC3;
    if( gdalType == GDT_UInt32 && nchannels == 1 ) return CV_32SC1;
    if( gdalType == GDT_UInt32 && nchannels == 2 ) return CV_32SC2;
    if( gdalType == GDT_UInt32 && nchannels == 3 ) return CV_32SC3;
    if( gdalType == GDT_Int32  && nchannels == 1 ) return CV_32SC1;
    if( gdalType == GDT_Int32  && nchannels == 2 ) return CV_32SC2;
    if( gdalType == GDT_Int32  && nchannels == 3 ) return CV_32SC3;

    throw std::string("TYPE NOT SUPPORTED");

}

GDALDataType GDAL_Driver::getGDALPixelType( const int& cvDepth ){

    if( cvDepth == CV_8U  ){  return GDT_Byte;   }
    if( cvDepth == CV_16U ){  return GDT_UInt16; }
    if( cvDepth == CV_16S ){  return GDT_Int16;  }
    if( cvDepth == CV_32S ){  return GDT_Int32;  }


    throw std::string("TYPE NOT SUPPORTED");

}

 double GDAL_Driver::GDAL2OpenCV( double const& val, const int cvtype, const int gdaltype ){
        
    if( cvtype == CV_8U  && gdaltype == GDT_Byte ) return val;
    if( cvtype == CV_8U  && gdaltype == GDT_Int16 ) return val/16.0;
    if( cvtype == CV_8U  && gdaltype == GDT_UInt16 ) return val/16.0;

    if( cvtype == CV_16U && gdaltype == GDT_Byte ) return val*8.0;
    if( cvtype == CV_16U && gdaltype == GDT_Int16 ) return val;
    if( cvtype == CV_16U && gdaltype == GDT_UInt16 ) return val;
        
    if( cvtype == CV_16S && gdaltype == GDT_Byte ) return val*8;
    if( cvtype == CV_16S && gdaltype == GDT_Int16 ) return val;
    if( cvtype == CV_16S && gdaltype == GDT_UInt16 ) return val;
        
    if( cvtype == CV_64F && gdaltype == GDT_Byte   ) return val;///255.0;
    if( cvtype == CV_64F && gdaltype == GDT_UInt16 ) return val;///65536.0;
    if( cvtype == CV_64F && gdaltype == GDT_Int16 ) return val;///65536.0;
        
    throw string("UNKNOWN TYPE");
}

int GDAL_Driver::get_num_channels( )const{
        
    //make sure the image is initialized
    if( isValid() == false )
        return -1;
        
    return dataset->GetRasterCount();

}

bool GDAL_Driver::isValid()const{
	return driver_initialized;
}


/**
 * Write an image using GDAL
*/
void GDAL_Driver::write( std::string const& image_filename, int const& format ){
    
    // build the desired output format
    int desired_format = format;

    // determine image format if not given
    if( desired_format == IMAGE_NONE ){
        
        // if not given, then we need to use the extension
        desired_format = compute_imagetype( image_filename );
    }
    if( desired_format == IMAGE_NONE ){
        throw string("Error: Could not detect image format");
    }
    
    // Set all parameters
    //char ** papszOptions = NULL;
    //for( size_t i=0; i<header_info.size(); i++ )
    //    papszOptions = CSLSetNameValue( papszOptions, header_info[i].first.c_str(), header_info[i].second.c_str());

    //Retrieve the driver
    GDALDriver* oDriver = GetGDALDriverManager()->GetDriverByName( compute_gdal_driver_format(desired_format).c_str());
    if( oDriver == NULL ){
        throw string("ERROR: Driver is null" );
    }

    //create an output dataset
    GDALDataset *outputData = oDriver->CreateCopy( image_filename.c_str(), dataset, 0, NULL, NULL, NULL);
   
    //GDALDataType tp = getGDALPixelType(image.depth());

    //Set the metadata
    //outputData->SetMetadata(papszOptions);

    //GDALRasterBand* band;

    //load dataset with appropriate metadata
    //for( int c=0; c<image.channels(); c++){

        //fetch band
    //    band = outputData->GetRasterBand(c+1);

    //    for( int y=0; y<image.rows; y++){
    //        for( int x=0; x<image.cols; x++){
    //            int* value = new int;
    //            cout << "A" << endl;
    //            *value  = cvGetPixel( image, cv::Point(x,y), c ); 
    //            cout << "B" << endl;
    //            band->RasterIO( GF_Write, x, y, 1, 1, value, 1, 1, tp, 0, 0);

    //            delete value;
    //            }
    //        }

    //    }

        //close dataset and flush data to file
        GDALClose((GDALDatasetH) outputData);
    //    CSLDestroy( papszOptions );


}

/**
 * Get the corner coordinates
*/
GIS_LineString GDAL_Driver::getCornerCoordinates()const{
    
    // get the transform
    double gt[6];

    // get raster dimensions
    double width  = getRasterDimensions().first;
    double height = getRasterDimensions().second;
        
    // create output object
    GIS_LineString output;

    // make sure we still have a dataset
    if( dataset == NULL ) return GIS_LineString();
    
    // otherwise get the transform
    if( dataset->GetGeoTransform( gt ) == CE_None ){
        
        // get the lower-left corner (minx, miny)
        output.append_coordinate(CoordinateDD( gt[0], gt[3] + width*gt[4] + height*gt[5]));
        
        // get the upper-left corner (minx, maxy)
        output.append_coordinate(CoordinateDD( gt[0], gt[3] ));
        
        // get the upper-right corner (maxx, maxy)
        output.append_coordinate(CoordinateDD( gt[0] + width*gt[1] + height*gt[2], gt[3] ));
        
        // get the lower-right corner (maxx, miny)
        output.append_coordinate(CoordinateDD( gt[0] + width*gt[1] + height*gt[2], gt[3] + width*gt[4] + height*gt[5]));


    } else{
        return GIS_LineString();
    }

    return output;
}


} /// End of GIS Namespace 
