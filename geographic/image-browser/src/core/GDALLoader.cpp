/**
 * @file   GDALLoader.cpp
 * @author Marvin Smith
 * @date   10/1/2013
*/
#include "GDALLoader.hpp"

#include <src/core/FilesystemUtilities.hpp>

using namespace std;

GDALLoader::GDALLoader( const string& filename ){

    // set filename
    this->filename = filename;
    
    // load the gdal object
    load();
}

void GDALLoader::load(){
    
    // make sure the file actually exists
	if( file_exists( filename ) == false ){
		throw string("Error:  File does not exist");
	}
    
	// register the gdal driver
    GDALAllRegister();

	// load the dataset 
    dataset = (GDALDataset*) GDALOpen( filename.c_str(), GA_ReadOnly);
	
    // if dataset is null, then there was a problem
	if( dataset == NULL ){
        cout << "Driver Null" << endl;
		return;
	}
	
	//extract the driver infomation
    driver = dataset->GetDriver();


}

Rect GDALLoader::get_bbox()const{
    
    double  xform[6];
    double width  = dataset->GetRasterXSize();
    double height = dataset->GetRasterYSize();

    /// Grab the projection transformation
    if( dataset->GetGeoTransform( xform ) == CE_None ){

        return Rect(    // Top Left Corner (Low X, High Y)
                        Point( xform[0], xform[3]),

                        // Bottom Right Corner (High X, Low Y)
                        Point( xform[0] + width*xform[1] + height*xform[2],
                               xform[3] + width*xform[4] + height*xform[5])
                    );
    }

    return Rect(Point(0,0),Point(0,0));
}

std::vector<std::string> GDALLoader::filter( const std::vector<std::string> const& input ){
    
    // create output 
    vector<string> output;

    // iterate through each file and compare filenames
    GDALAllRegister();
    
    for( size_t i=0; i<input.size(); i++ ){

        // create a temporary dataset
        GDALDataset* temp = (GDALDataset*) GDALOpen( input[i].c_str(), GA_ReadOnly );
        if( temp == NULL ){
            continue;
        }
        else{
            output.push_back(input[i]);
        }
    }
    return output;
}

std::string GDALLoader::getShortName()const{
    return GDALGetDriverShortName(driver);
}

std::string GDALLoader::getLongName()const{
    return GDALGetDriverLongName(driver);
}

