#include <boost/algorithm/string.hpp>

#include "GDAL_Data.h"
#include <iostream>

using namespace std;

namespace GEO{

/** Create the GDAL Dataset
 *
 * @brief This initializes all GDAL Dataset objects to NULL
*/
GDAL_Data::GDAL_Data(){

    driver = NULL;
    dataset = NULL;
    gdalLoadFailed = false;
}

/** Destroy the GDAL Data object
 *
 * @brief This will close the dataset and delete all allocated
 * gdal-specific memory
*/
GDAL_Data::~GDAL_Data(){

    if( dataset != NULL )
        GDALClose( dataset );

}

/** Write an image to a NITF Format
 *
*/
void GDAL_Data::write( std::string const& image_filename, std::string const& image_format ){

    //currently the only supported format is NITF
    if( image_format != "NITF" ){
        cout << "invalid format, exiting" << endl;
        return;
    }
    
    /// This is where we should set any internal flags
    char ** papszOptions = NULL;

    //create an output dataset
    //GDALDataset *outputData = driver->Create( image_filename.c_str(), 
    //        dataset->GetRasterXSize(), dataset->GetRasterYSize(),
    //        dataset->GetRasterCount(), GDALGetRasterDataType(dataset->GetRasterBand(1)),
    //        papszOptions);
    
    //copy an output dataset
    GDALDataset *outputData = driver->CreateCopy( image_filename.c_str(), 
            dataset, FALSE, papszOptions, NULL, NULL);
    
    //load dataset with appropriate metadata



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


}

vector<pair<string,string> >  GDAL_Data::retrieve_header_data()const{

    vector<pair<string,string> > headerList;
    char ** metadata = dataset->GetMetadata();

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



} //end of GEO namespace 
