
// load the header file
#include "GDALLoader.hpp"
#include "../utilities/File_Utilities.hpp"
#include "../utilities/OpenCV_Utilities.hpp"

// STL Libraries
#include <iostream>

// Boost Libraries
#include <boost/algorithm/string.hpp>

// OpenCV Library
#if USE_OPENCV == 1
using namespace cv;
#endif

using namespace std;


/// start of GEO Namespace
namespace GEO{

    /** Create the GDALLoader
     *
     * @brief This initializes all GDAL Dataset objects to NULL
     */
    GDALLoader::GDALLoader(){
        
        driver = NULL;
        dataset = NULL;
        
        gdalLoadFailed   = false;
        openCVCompatible = false;
        valid_data       = false;
    }

    /**
     * Create a GDALLoader object given a filename
    */
    GDALLoader::GDALLoader( const string& fname ){
        
        initialize( fname );
    }

    /** Destroy the GDAL Data object
     *
     * @brief This will close the dataset and delete all allocated
     * gdal-specific memory
     */
    GDALLoader::~GDALLoader(){

        if( dataset != NULL )
            GDALClose( dataset );

    }
    
    /*-----------------------------------------------------------------------*/ 
    /**
     * Initialize the dataset and load the GDAL Drivers
    */
    void GDALLoader::initialize( const string& fname ){

        // make sure that the file exists
        if( STR::file_exists( fname ) == false ){
            valid_data = false;
            gdalLoadFailed = true;
            throw string( "ERROR: File does not exist" );
        }
        
        // make sure we have an appropriate extension
        string ext = STR::file_extension( fname );
        if( ext != ".ntf" && ext != ".NTF" && ext != ".nitf" && ext != ".NITF" &&
            ext != ".dt2" && ext != ".DT2"                                        ){
            valid_data = false;
            gdalLoadFailed = true;
            throw string(string("ERROR: did not recognize extension (") + ext + string(")"));
        }


        // register the gdal driver
        GDALAllRegister();
    
        // load the dataset 
        dataset = (GDALDataset*) GDALOpen( fname.c_str(), GA_ReadOnly);
        
        // if the file returns null, then exit
        if( dataset == NULL ){
            openCVCompatible = false;
            gdalLoadFailed   = true;
            valid_data = false;
            return;
        }
        else
            valid_data = true;
    
        //check for pixel data and halt action if it is not present
        if ( dataset->GetRasterCount() <= 0 ){
            openCVCompatible = false;
            return;
        }

        openCVCompatible = true;
        gdalLoadFailed = false;

        
        //check to make sure its open
        if ( dataset == NULL ){
            throw std::string("Dataset did not load");
        }

        //get the driver infomation
        driver = dataset->GetDriver();


    }
    
    /*-----------------------------------------------------------------------*/ 
    /** Write an image to a NITF Format
     *
     *
    void GDAL_Data::write( std::string const& image_filename, std::string const& image_format ){

        //currently the only supported format is NITF
        if( image_format != "NITF" ){
            cout << "invalid format, exiting" << endl;
            return;
        }

        /// This is where we should set any internal flags
        char ** papszOptions = NULL;

        //copy an output dataset
        GDALDataset *outputData = driver->CreateCopy( image_filename.c_str(), 
                dataset, FALSE, papszOptions, NULL, NULL);

        *
           for( size_t i=0; i<header_info.size(); i++ )
           papszOptions = CSLSetNameValue( papszOptions, header_info[i].first.c_str(), header_info[i].second.c_str());


        //Retrieve the driver
        GDALDriver* oDriver = GetGDALDriverManager()->GetDriverByName(header_data->get_gdal_driver_format().c_str());

        //create an output dataset
        GDALDataset *outputData = oDriver->Create( image_filename.c_str(), image.cols, image.rows, image.channels(), header_data->get_pixel_type().get_gdal_type(), NULL );
         *

        //Set the metadata
        outputData->SetMetadata( driver->GetMetadata() );



        //close dataset and flush data to file
        GDALClose((GDALDatasetH) outputData);


    }

    void GDAL_Data::write( std::string const& image_filename, cv::Mat const& image, GeoHeader_Info* header_data){

        /// This is where we should set any internal flags
        vector<pair<string,string> > header_info = header_data->get_header_data();

        // Set all parameters
        char ** papszOptions = NULL;
        for( size_t i=0; i<header_info.size(); i++ )
            papszOptions = CSLSetNameValue( papszOptions, header_info[i].first.c_str(), header_info[i].second.c_str());


        //Retrieve the driver
        GDALDriver* oDriver = GetGDALDriverManager()->GetDriverByName(header_data->get_gdal_driver_format().c_str());

        //create an output dataset
        GDALDataset *outputData = oDriver->Create( image_filename.c_str(), image.cols, image.rows, image.channels(), header_data->get_pixel_type().get_gdal_type(), NULL );

        //Set the metadata
        outputData->SetMetadata(papszOptions);

        GDALRasterBand* band;


        //load dataset with appropriate metadata
        for( int c=0; c<image.channels(); c++){

            //fetch band
            band = outputData->GetRasterBand(c+1);

            for( int y=0; y<image.rows; y++){
                for( int x=0; x<image.cols; x++){
                    int* value = new int;

                    *value  = header_data->get_pixel_type().convert( image, x, y, c);

                    band->RasterIO( GF_Write, x, y, 1, 1, value, 1, 1, header_data->get_pixel_type().get_gdal_type(), 0, 0);

                    delete value;
                }
            }

        }

        //close dataset and flush data to file
        GDALClose((GDALDatasetH) outputData);
        CSLDestroy( papszOptions );


    }*/


    /*-----------------------------------------------------------------------*/ 
    /**
     * Query for the header metadata
    */
    vector<pair<string,string> >  GDALLoader::get_header_data()const{
    
        //create output data
        vector<pair<string,string> > headerList;
        
        // grab the metadata
        char ** metadata = dataset->GetMetadata();
        
        // iterate through the metadata, inserting the key/value pairs into
        // the output structure
        int idx = 0;
        string original;
        std::pair<string, string> item;
        while( metadata[idx] != NULL ){
            original = metadata[idx++];
            vector<string> results;
            boost::split(results, original, boost::is_any_of("="));

            if( results.size() < 2 )
                throw string("ERROR: metadata tag is invalid, must contain at least one =");

            item.first = results[0];
            item.second= results[1];

            for(size_t i=2; i<results.size(); i++)
                item.second += string("=") + results[i];

            headerList.push_back(item);
        }


        return headerList;
    }
    
    /*-----------------------------------------------------------------------*/ 
    /**
     * Get header metadata in the TRE module
    */
    string  GDALLoader::get_header_tre()const{
        
        char ** tre_info = NULL;
    
        string tre_output = "";
        char ** results = dataset->GetMetadata("TRE");
    
        int idx = 0;
        if( results != NULL ){
        
            while( results[idx] != NULL ){
                tre_output += results[idx++];
            }
        }

        return tre_output;

    }

    string  GDALLoader::get_header_tre( const string& TAG )const{
        
        char ** tre_info = NULL;
    
        string tre_output = "";
        if( dataset->GetMetadataItem(TAG.c_str(), "TRE" ) != NULL )
            return dataset->GetMetadataItem(TAG.c_str(), "TRE" );
        else
            return "";
    }
    
    string  GDALLoader::get_raw_tre()const{
        
        char ** tre_info = NULL;
    
        string tre_output = "";
        char ** results = dataset->GetMetadata("NITF_METADATA");
    
        int idx = 0;
        if( results != NULL ){
        
            while( results[idx] != NULL ){
                cout << results[idx] << endl;
                tre_output += results[idx++];
            }
        }
        return tre_output;

    }

    /*
    void GDAL_Data::get_corner_coordinates( double& ul_lat, double& ul_lon, double& br_lat, double& br_lon )const{

        double adfGeoTransform[6];
        if( dataset->GetGeoTransform( adfGeoTransform ) == CE_None )
        {
            ul_lat = adfGeoTransform[3];
            ul_lon = adfGeoTransform[0];

            int xS = dataset->GetRasterXSize()-1;
            int yS = dataset->GetRasterYSize()-1;

            br_lon = adfGeoTransform[0] + xS*adfGeoTransform[1] + yS*adfGeoTransform[2];
            br_lat = adfGeoTransform[3] + xS*adfGeoTransform[4] + yS*adfGeoTransform[5];
        }

    }

    void GDAL_Data::set_img_data( cv::Mat const& image ){

        GDALRasterBand *band;    
        PixelType pixelToSet;

        if ( image.depth() == CV_8U )
            pixelToSet.set( PixelType::UInt8C1 );

        else if ( image.depth() == CV_16U )
            pixelToSet.set( PixelType::UInt16C1 );

        else if ( image.depth() == CV_16S )
            pixelToSet.set( PixelType::Int16C1 );

        else 
            throw string(string("Unknown pixel depth: ") + opencvDepth2string(image.depth()));


        //load dataset with appropriate metadata
        for( int c=0; c<image.channels(); c++){

            //fetch band
            band = dataset->GetRasterBand(c+1);

            for( int y=0; y<image.rows; y++){
                for( int x=0; x<image.cols; x++){
                    int* value = new int;

                    *value  = pixelToSet.convert( image, x, y, c);

                    band->RasterIO( GF_Write, x, y, 1, 1, value, 1, 1, pixelToSet.get_gdal_type(), 0, 0);

                    delete value;
                }
            }

        }
    }

    */
    
    /**
    */
    bool GDALLoader::isOpenCVCompatible( )const{
        return openCVCompatible;
    }

    bool GDALLoader::isValid()const{
        return valid_data && !gdalLoadFailed;
    }
        
    int GDALLoader::get_image_channels( )const{
        
        //make sure the image is initialized
        if( isValid() == false )
            return -1;
        
        return dataset->GetRasterCount();

    }
    

    pair<int,int>  GDALLoader::getRasterDimensions( )const{
        
        return pair<int,int>( dataset->GetRasterXSize(), dataset->GetRasterYSize() );

    }

#if USE_OPENCV == 1
    int GDALLoader::getOpenCVPixelType( )const{
        
        int gdalType  = dataset->GetRasterBand(1)->GetRasterDataType();
        int nchannels = get_image_channels( );

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

    double GDAL2OpenCV( double const& val, const int cvtype, const int gdaltype ){
        
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

    /**
     * Get the OpenCV Image Data.
    */
    Mat GDALLoader::getOpenCVMat( const int& pixType )const{
    
        // get relevant image data
        pair<int,int> dims =  getRasterDimensions();
        
        // create an output image
        vector<Mat> layers( dataset->GetRasterCount() );
        for( size_t i=0; i<layers.size(); i++ )
            layers[i] = Mat( dims.second, dims.first, cvDepthChannel2Type(cvType2Depth( pixType), 1));
        
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

        //merge channels into single image
        Mat output;
        if( layers.size() > 1 )
            merge( output, layers );
        else
            output = layers[0].clone();

        return output;
    }


#endif

    string GDALLoader::getImageTypeName( )const{
        
        //return nothing if the dataset is null
        if( dataset == NULL ) return "__NONE__";
        
        return driver->GetDescription();
    }

} //end of GEO namespace 
