
// load the header file
#include "GDALLoader.hpp"
#include "../utilities/File_Utilities.hpp"

// STL Libraries
#include <iostream>

// Boost Libraries
#include <boost/algorithm/string.hpp>

using namespace std;


/// start of GEO Namespace
namespace GEO{

    /** Create the GDALLoader
     *
     * @brief This initializes all GDAL Dataset objects to NULL
     */
    GDALLoader::GDALLoader(){
        
        //driver = NULL;
        //dataset = NULL;
        //gdalLoadFailed = false;
    }

    /**
     * Create a GDALLoader object given a filename
    */
    GDALLoader::GDALLoader( const string& fname ){
        
        // make sure that the file exists
        if( STR::file_exists( fname ) == false )
            throw string( "ERROR: File does not exist" );
        
        // make sure we have an appropriate extension
        string ext = STR::file_extension( fname );
        if( ext != ".ntf" && ext != ".NTF" && ext != ".nitf" && ext != ".NITF" &&
            ext != ".dt2" && ext != ".DT2"                                        )
            throw string(string("ERROR: did not recognize extension (") + ext + string(")"));


        // register the gdal driver
        GDALAllRegister();
    
        // load the dataset 
        dataset = (GDALDataset*) GDALOpen( fname.c_str(), GA_ReadOnly);
        
        // if the file returns null, then exit
        if( dataset == NULL ){
            openCVCompatible = false;
            gdalLoadFailed   = true;
            return;
        }
    
        //check for pixel data and halt action if it is not present
        if ( dataset->GetRasterCount() <= 0 ){
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

    /** Destroy the GDAL Data object
     *
     * @brief This will close the dataset and delete all allocated
     * gdal-specific memory
     */
    GDALLoader::~GDALLoader(){

        if( dataset != NULL )
            GDALClose( dataset );

    }

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

    
    bool GDAL_Data::get_status()const{
        
        if( dataset == NULL ) return false;
        if( driver  == NULL ) return false;
        if( gdalLoadFailed == true ) return false;

        return true;
    }

    ** 
     *
     *
    void GDAL_Data::clean(){

        if( dataset != NULL )
            GDALClose( dataset );
        
        dataset = NULL;
        driver  = NULL;
        gdalLoadFailed = false;
    }

    */

} //end of GEO namespace 
