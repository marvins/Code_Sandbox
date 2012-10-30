
#include "../image/GeoImage.hpp"
#include "../utilities/File_Utilities.hpp"

#include <cstdlib>
#include <iostream> 
#include <string>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

class Options{

    public:
        string input_filename;
        string output_filename;
        vector<string> flags;

        bool has_flag( const string& flag ){
            
            for( size_t i=0; i<flags.size(); i++ )
                if( flags[i] == flag ) return true;
            return false;
        }

} options;

bool is_valid_raster_ext( const string& ext ){
    
    // check for the image raster flag
    for( size_t i=0; i<options.flags.size(); i++ ){
        
        // check for image
        if( options.flags[i] == "-i" )
            return true;

        // check for dem
        if( options.flags[i] == "-d" )
            return true;
    }

    // NITF
    if( ext == ".ntf" || ext == ".NTF" || ext == ".NITF" || ext == ".nitf" )
        return true;
    
    // DTED
    if( ext == ".dt2" || ext == ".dt1" || ext == ".dt0" || 
        ext == ".DT2" || ext == ".DT1" || ext == ".DT0" ){
        options.flags.push_back("-d");
        return true;
    }

    return false;
}

void usage( ){

    cout << "geo_convert" << endl;
    cout << endl;
    cout << "usage: " << endl;
    cout << "geo_convert  -I=<input file> -O=<output file> [options]" << endl;
    cout << endl;
    cout << "   Options:" << endl;
    cout << "       -h : help" << endl;
    cout << "       -i : force input as image" << endl;
    cout << "       -d : Input is DEM  ) - Highly, highly recommended" << endl;
    cout << "       -s : Scale image to dynamic range" << endl;
    cout << "       -H : Hillshade) [ Use for DEM image ]" << endl;
}

void error( string const& message ){

    cout << message << endl;
    usage();
    exit(1);
}

int main( int argc, char* argv[] ){

    try{

    // check the input arguments
    if( argc < 3 ) 
        error("ERROR: Invalid number of arguments");

    // start parsing arguments
    for( size_t i=1; i<argc; i++ ){

        if( string(argv[i]).substr(0,3) == "-I=" ){
            options.input_filename = string(argv[i]).substr(3);
        }
        else if( string(argv[i]).substr(0,3) == "-O=" ){
            options.output_filename = string(argv[i]).substr(3);
        }
        else if( string(argv[i]) == "help" || string(argv[i]) == "-h" || string(argv[i]) == "-help" ){
            usage();
            exit(0);
        }
        else{
            options.flags.push_back( argv[i]);
        }


    }
    
    // make sure the input file exists
    if( GEO::STR::file_exists( options.input_filename ) == false )
        error(string("ERROR: ") + options.input_filename + string(" does not exist" ));

    // do different things depending on the input file
    string in_ext = GEO::STR::file_extension( options.input_filename );
    if( is_valid_raster_ext( in_ext ) ){
        
        // read file as GeoImage
        GEO::GeoImage geoimage( options.input_filename, true );
        
        // check if we have a Image or DEM File
        if( options.has_flag( "-d" ) == true ){
            
            // pull out data
            Mat image = geoimage.get_image( CV_64FC1 );
            
            cout << image.cols << ", " << image.rows << endl;

            // query for scale flag
            if( options.has_flag( "-s") == true )
                normalize( image, image, 0, 255, CV_MINMAX );

            // convert to CV_8UC1
            Mat output( image.size(), CV_8UC1 );
            for( int c=0; c<image.cols; c++ )
                for( int r=0; r<image.rows; r++ ){
                    if( image.at<double>(r,c) < 0 )
                        output.at<uchar>(r,c) = 0;
                    else if( image.at<double>(r,c) > 255 )
                        output.at<uchar>(r,c) = 255;
                    else
                        output.at<uchar>(r,c) = image.at<double>(r,c);
                }

            // write to file
            imwrite( options.output_filename, output );

        }
        else{
            
            // pull out opencv mat
            Mat image = geoimage.get_image(CV_8UC3);
        
            // write to output
            imwrite( options.output_filename, image );
        }

    }
    else{
        error("ERROR: Unknown file type");
    }
    } catch ( string e ){
        cout << e << endl;
        usage();
    }

    return 0;
}

