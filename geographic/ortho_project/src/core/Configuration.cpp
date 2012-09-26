#include "Configuration.hpp"
#include "Utilities.hpp"
#include "OrthoExceptions.hpp"

#include <GeoImage.h>

#include <iostream>
#include <sstream>
#include <vector>

#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

Mat load_point( vector<double> const& array ){

    Mat output(4, 1, CV_64FC1);
    output.at<double>(0,0) = array[0];
    output.at<double>(1,0) = array[1];
    output.at<double>(2,0) = array[2];
    output.at<double>(3,0) = 1;
    return output;
}

Options::Options( const int& argc, char ** argv ){

    //set the config file
    config_filename = "data/options.cfg";

    //load the config options
    load_configuration( argc, argv);
}

/**
 * Get the run type. 
 * 
 * Possible values
 * - BUILD
 * - RECTIFY
 * - FULL
*/
string Options::get_run_type()const{
    return run_type;
}


/** 
  * Use camera parameters from config
*/
void Options::load_camera_parameters_file( ){
    
    bool found = false;
    
    /*****************************/
    /*        Focal Length       */
    /*****************************/
    //pull the focal length
    focal_length = parser.getItem_double("CAMERA_FOCAL_LENGTH", found);
    
    // make sure the focal length exists
    if( found == false )
        throw ParserTagNotFoundException("CAMERA_FOCAL_LENGTH", run_type, __FILE__, __LINE__);
    
    /********************************/
    /*    Camera Plane Dimensions   */
    /********************************/
    camera_plane_width  = parser.getItem_double("CAMERA_PLANE_WIDTH", found);
    
    // make sure the camera_plane_width exists
    if( found == false )
        throw ParserTagNotFoundException("CAMERA_PLANE_WIDTH", run_type, __FILE__, __LINE__);
    
    camera_plane_height = parser.getItem_double("CAMERA_PLANE_HEIGHT", found);
    
    // make sure the camera_plane_height exists
    if( found == false )
        throw ParserTagNotFoundException("CAMERA_PLANE_HEIGHT", run_type, __FILE__, __LINE__);
    
    
    /******************************/
    /*     Rotation Parameters    */
    /******************************/
    //load the Rotation Quaternion Matrix
    vector<double> rot_axis = parser.getItem_vec_double("Camera_Rotation_Axis", found);
    if( found == false )
        throw string("ERROR: Camera Rotation axis tag invalid or not found");

    double rot_ang = parser.getItem_double("Camera_Rotation_Angle", found);
    if( found == false )
        throw string("ERROR: Camera Rotation angle tag invalid or not found");

    //create the quaternion
    RotationQ = Quaternion( rot_ang*M_PI/180.0, vec( rot_axis[0], rot_axis[1], rot_axis[2]));

    //convert the quaternion into a rotation matrix
    RotationM = RotationQ.get_rotation_matrix();

    /****************************/
    /*     Camera Position      */
    /****************************/
    vector<double> position = parser.getItem_vec_double("Camera_Position", found);
    if( found == false )
        throw string("ERROR: Camera Position tag invalid or not found");

    //set the camera position
    Position_i = load_point( position );


    //load the image if in RECTIFY run mode and the image is not a geo image
        if( rectify_image_type == "CV_8UC1" )
            image = imread( image_filename.c_str(), 0);    
        else if( rectify_image_type == "CV_8UC3" )
            image = imread( image_filename.c_str());
        else
            throw string("UNKNOWN TYPE");

}



/**
 * Use GeoImage camera parameters
*/
void Options::load_camera_parameters_geo( ){

    throw string("Function not implemented yet");
    bool found = false;
    GEO::GeoImage gimg(image_filename, true );


}


/** 
 * Open the configuration file and read values 
*/
void Options::load_configuration( const int& argc, char ** argv ){
   
    //Parse the config file
    parser.init( argc, argv, config_filename );
    bool found = false;
    
    /**************************************/
    /*               LOGGING              */
    /**************************************/
    vector<string> log_flags;
    
    // extract the log filename here
    string logfilename = parser.getItem_string("LOG_LOGFILE_NAME", found);
    if( found )
        logger.set_logfile_name(logfilename);

    // extract the logfile run states desired here
    log_flags = parser.getItem_vec_string("LOG_LOGFILE_CONFIGURATION", found );
    unsigned int state = 0;
    for( size_t i=0; i<log_flags.size(); i++ ){
        if( log_flags[i] == "MAJOR" )
            state = state | LOG_MAJOR;
        else if( log_flags[i] == "MINOR" )
            state = state | LOG_MINOR;
        else if( log_flags[i] == "WARNING" )
            state = state | LOG_WARNING;
        else if( log_flags[i] == "INFO" )
            state = state | LOG_INFO;
        else
            throw string("ERROR: UNKNOWN LOG FLAG");
    }
    logger.set_logfile_run_state( state );

    // extract the console run states desired here
    log_flags = parser.getItem_vec_string("LOG_CONSOLE_CONFIGURATION", found );
    state = 0;
    for( size_t i=0; i<log_flags.size(); i++ ){
        if( log_flags[i] == "MAJOR" )
            state = state | LOG_MAJOR;
        else if( log_flags[i] == "MINOR" )
            state = state | LOG_MINOR;
        else if( log_flags[i] == "WARNING" )
            state = state | LOG_WARNING;
        else if( log_flags[i] == "INFO" )
            state = state | LOG_INFO;
        else
            throw string("ERROR: UNKNOWN LOG FLAG");
    }
    logger.set_console_run_state( state );

    /**************************************/
    /*               RUN TYPE             */
    /**************************************/
    run_type = parser.getItem_string("RUN_TYPE", found);
    if( found == false )
        throw string("ERROR: RUN_TYPE tag not found in configuration");

    if( run_type != "FULL"    &&
        run_type != "BUILD"   &&
        run_type != "RECTIFY"   )
        throw string(string("ERROR: Invalid RUN_TYPE tag ")+run_type+string(" found"));
    
    /************************************/
    /*      Load the Configuration      */
    /************************************/
    if( run_type == "BUILD" )
        load_build_configuration();
    else if( run_type == "RECTIFY" )
        load_rectify_configuration();
    else if( run_type == "FULL" )
        load_full_configuration();
    else    
        throw string(string("ERROR: Invalid RUN_TYPE tag ")+run_type+string(" found"));
    

    /**************************************/
    /*     Validate the configuration     */
    /**************************************/
    if( validate_configuration() == false )
        throw string("ERROR: validation failed");

}


void Options::load_build_configuration(){
    
    bool found;

    /*************************************************************/
    /* Image Filename                                            */
    /*                                                           */
    /*  - The image filename must be in the configuration file,  */
    /*    but does not need to exist in the filesystem.          */
    /*************************************************************/
    //query the image filename
    image_filename = parser.getItem_string("IMAGE_FILENAME", found);
    
    //check to make sure the flag exists
    if( found == false )    
        throw ParserTagNotFoundException("IMAGE_FILENAME", run_type, __FILE__, __LINE__);


    /*************************************************************/
    /* DEM_NAME                                                  */
    /*                                                           */
    /*   - The dem name must exist, however it does not need to  */
    /*     exist in the filesystem as it will be overwritten.    */
    /*************************************************************/
    //query the dem filename
    dem_filename = parser.getItem_string("DEM_NAME", found);
    
    // check to make sure the flag exists
    if( found == false )
        throw ParserTagNotFoundException("DEM_NAME", "BUILD", __FILE__, __LINE__);

    
    /***********************************************************/
    /* Camera Parameters                                       */
    /*                                                         */
    /*  - We will always load the camera parameters using the  */
    /*    config file for the BUILD run type.                  */
    /***********************************************************/
    load_camera_parameters_file();


    throw string("ERROR: RUN_TYPE BUILD not configured yet");

}

void Options::load_rectify_configuration(){
    
    bool found;

    /*************************************************************/
    /* Image Filename                                            */
    /*                                                           */
    /*  - The image filename must be in the configuration file,  */
    /*    and needs to exist in the filesystem.                  */
    /*************************************************************/
    //query the image filename
    image_filename = parser.getItem_string("IMAGE_FILENAME", found);
    
    //check to make sure the flag exists
    if( found == false )
        throw ParserTagNotFoundException("IMAGE_FILENAME", run_type, __FILE__, __LINE__);  
    
    //ensure that the file actually exists
    if( file_exists( image_filename ) == false )
        throw FileNotFoundException( image_filename, run_type, __FILE__, __LINE__);

    
    /*************************************************************/
    /* DEM_MODE                                                  */
    /*                                                           */
    /*   - The dem mode must exist and be a supported option, as */
    /*     determines how we will load the dem model information.*/
    /*************************************************************/
    string dem_mode = parser.getItem_string("DEM_MODE", found);

    //make sure flag exists
    if( found == false )
        throw ParserTagNotFoundException("DEM_MODE", run_type, __FILE__, __LINE__);

    //make sure it matches the right list of options
    if( dem_mode != "FILE" && dem_mode != "DTED" )
        throw ParserInvalidTagValueException("DEM_MODE", dem_mode, run_type, __FILE__, __LINE__);
    
    /*************************************************************/
    /* DEM_NAME                                                  */
    /*                                                           */
    /*   - The dem name must exist, however it does not need to  */
    /*     exist in the filesystem as it will be overwritten.    */
    /*************************************************************/
    //query the dem filename
    dem_filename = parser.getItem_string("DEM_NAME", found);
    
    // check to make sure the flag exists
    if( found == false )
        throw ParserTagNotFoundException("DEM_NAME", run_type, __FILE__, __LINE__);  
     

    //we need to make sure that the dem information is valid for our use
    //
    // - if dem_mode = file, then make sure the file exists
    // - if dem_mode = dted, then make sure the directory structure is valid
    if( dem_mode == "FILE" ){
        if( file_exists( dem_filename ) == false )
            throw FileNotFoundException( dem_filename, run_type, __FILE__, __LINE__ );
     }
     else if( dem_mode == "DTED" ){
        if( file_exists( dem_filename ) == false )
             throw FileNotFoundException( dem_filename, run_type, __FILE__, __LINE__ );
     }

    /***********************************************************/
    /* Camera Parameters                                       */
    /*                                                         */
    /*  - We will load the camera parameters from either the   */
    /*    geo image meta-data or from the configuration file.  */
    /*    This depends on the geo tag.                         */
    /***********************************************************/
    rectify_is_geo = parser.getItem_bool("USE_GEOIMAGE_CAMERA_PARAMS", found );

    //make sure the tag exists
    if( found == false )
        throw ParserTagNotFoundException("USE_GEOIMAGE_CAMERA_PARAMS", run_type, __FILE__, __LINE__);  
    
    // if the flag is true, then load using metadata
    if( rectify_is_geo == true )
        load_camera_parameters_geo();

    // otherwise use the config file
    else
        load_camera_parameters_file();


     
     
     
     throw string("ERROR: RUN_TYPE RECTIFY not configured yet");


}

void Options::load_full_configuration(){
    
    bool found;

    /*************************************************************/
    /* Image Filename                                            */
    /*                                                           */
    /*  - The image filename must be in the configuration file,  */
    /*    but does not need to exist in the filesystem.          */
    /*************************************************************/
    //query the image filename
    image_filename = parser.getItem_string("IMAGE_FILENAME", found);
    
    //check to make sure the flag exists
    if( found == false )    
        throw ParserTagNotFoundException("IMAGE_FILENAME", run_type, __FILE__, __LINE__);   

    
    /*************************************************************/
    /* DEM_NAME                                                  */
    /*                                                           */
    /*   - The dem name must exist, however it does not need to  */
    /*     exist in the filesystem as it will be overwritten.    */
    /*************************************************************/
    //query the dem filename
    dem_filename = parser.getItem_string("DEM_NAME", found);
    
    // check to make sure the flag exists
    if( found == false )
        throw ParserTagNotFoundException("DEM_NAME", run_type, __FILE__, __LINE__);   

    /***********************************************************/
    /* Camera Parameters                                       */
    /*                                                         */
    /*  - We will always load the camera parameters using the  */
    /*    config file for the FULL run type.                   */
    /***********************************************************/
    load_camera_parameters_file();







    throw string("ERROR: RUN_TYPE FULL not configured yet");


    /*******************************************/
    /*      RECTIFY IMAGE OUTPUT FILENAME      */
    /*******************************************/
    rectify_output_filename = parser.getItem_string("RECTIFY_OUTPUT_FILENAME", found);
    if((run_type == "FULL" || run_type == "RECTIFY" ) && found == false )
        throw string("ERROR: RECTIFY_OUTPUT_FILENAME tag does not exist");



    /***********************/
    /*   BUILD_IMG_TYPE    */
    /***********************/
    build_image_type = parser.getItem_string("BUILD_IMG_TYPE", found );
    if((run_type == "FULL" || run_type == "BUILD") && found == false )
        throw string("ERROR: RUN_TYPE tag not found in configuration. Required by FULL and BUILD run_level.");

    if((run_type == "FULL" || run_type == "BUILD") && 
            ( build_image_type != "CV_8UC1" && build_image_type != "CV_8UC3" ))
        throw string(string("ERROR: BUILD_IMG_TYPE flag ")+build_image_type+string(" is invalid"));


    
    /**************************/
    /*    BUILD IMAGE SIZE    */
    /**************************/
    // we only care about the build image size if the build process is called on
    int c = parser.getItem_int("BUILD_IMG_COLS", found );
    if( found == false && ( run_type == "FULL" || run_type == "BUILD"))
        throw string("ERROR: BUILD_IMG_COLS tag not found in configuration");

    int r = parser.getItem_int("BUILD_IMG_ROWS", found );
    if( found == false && ( run_type == "FULL" || run_type == "BUILD"))
        throw string("ERROR: BUILD_IMG_ROWS tag not found in configuration");

    if( run_type == "FULL" || run_type == "BUILD" )
        build_image_size = Size(c, r);
    
    
    /******************************************/
    /*       ZBuffer and 3D Usage Flags       */
    /******************************************/
    //check for the zbuffer flag
    zbufferEnabled = parser.getItem_bool("ZBUFFER_ENABLED", found);
    if( found == false && ( run_type == "FULL" || run_type == "BUILD" ) )
        throw string("ERROR: ZBUFFER_ENABLED not found or enabled");

    //check for the perspective 2 parallel item
    perspective2parallel = parser.getItem_bool("RECTIFY_PERFORM_PERSPECTIVE_TO_PARALLEL", found);
    if( found == false && ( run_type == "FULL" || run_type == "RECTIFY" ) )
        throw string("ERROR: Perspective 2 Parallel not found");


    /**************************************************/
    /*       Digital Elevation Model Parameters       */
    /**************************************************/
    //Query the DEM Mode
    string dem_mode = parser.getItem_string("DEM_Mode", found);
    if( found == false )
        throw string("DEM_Mode must be in config file");

    if( dem_mode == "FILE" ){

        //Check for dem image
        string demname = parser.getItem_string("DEM_Name", found);
        if( found == false && ( run_type == "FULL" || run_type == "RECTIFY" ) )
            throw string("DEM_Name not found");

        if( run_type == "FULL" || run_type == "RECTIFY" )
            dem = imread( demname.c_str(), 0);
    } else if( dem_mode == "DTED" ){

        //create a DTED Library lookup

    } else {
        throw string("ERROR: DEM Mode not recognized");
    }
   

}

bool Options::validate_configuration(){
    
    return false;

    return true;
}


bool Options::doZBuffering()const{
    return zbufferEnabled;
}


void Options::print(){

    string ENDL("\n");
    string output = ENDL;

    ostringstream sout;

    ///////////////////////////////////////////////
    //           Program Configuration           //
    ///////////////////////////////////////////////
    output += string("Program Configuration\n");
    output += string(" - Run Type      : ") +    run_type    + ENDL;
    output += string(" - Image Filename: ") + image_filename + ENDL;
    output += ENDL;

    ///////////////////////////////////////////////
    //            Camera Configuration           //
    ///////////////////////////////////////////////
    output += string("Camera Configuration") + ENDL;
    output += string(" - Focal Length  : ") + num2str(focal_length) + ENDL;

    sout.clear(); sout.str(""); sout << RotationQ.get_angle(false);
    output += string(" - Rotation Angle: ") + sout.str() + ENDL;
    sout.clear(); sout.str(""); sout << RotationQ.get_axis();
    output += string(" - Rotation Axis : ") + sout.str() + ENDL;
    output += ENDL;

    ///////////////////////////////////////////////
    //         Build Image Configuration         //
    ///////////////////////////////////////////////
    output += string("Build Image Configuration") + ENDL;
    output += string(" - Build Image Size: ") + num2str(build_image_size.width) + string(", ") + num2str(build_image_size.height) + ENDL;
    output += string(" - Build Image Type: ") + num2str(build_image_type) + ENDL;
    if( zbufferEnabled == true )
        output += string(" - Z Buffering     : True") + ENDL;
    else
        output += string(" - Z Buffering     : False") + ENDL;
    output += ENDL;

    ///////////////////////////////////////////////
    //         Rectify Image Configuration       //
    ///////////////////////////////////////////////
    output += string("Rectify Image Configuration") + ENDL;
    output += string(" - Rectify Image Type: ") + num2str(rectify_image_type) + ENDL;

    if( perspective2parallel == true )
        output += string(" - Perspective2Parallel: True") + ENDL;
    else
        output += string(" - Perspective2Parallel: False") + ENDL;

    //add message to logger
    logger.add_message( LOG_INFO, output );
}

Size Options::get_build_image_size()const{

    return build_image_size;
}


int Options::get_rectify_image_type()const{

    if( rectify_image_type == "CV_8UC1" ) return CV_8UC1;
    else if( rectify_image_type == "CV_8UC3" ) return CV_8UC3;
    else throw string("ERROR: unsupported type");

}

int Options::get_build_image_type()const{

    if( build_image_type == "CV_8UC1" ) return CV_8UC1;
    else if( build_image_type == "CV_8UC3" ) return CV_8UC3;
    else throw string("ERROR: unsupported type");

}

void Options::set_focal_length( const double& fl ){
    focal_length = fl;
}

double Options::get_focal_length( )const{
    return focal_length;
}

Mat Options::get_output_img2cam( const Size& imsize, double const& sx, double const& sy, double const& sz )const{
    
    Mat matrix(4,4,CV_64FC1);
    double ix = 1.0/imsize.width;
    double iy = 1.0/imsize.height;
    
    matrix = Scalar(0);
    matrix.at<double>(0,0) = ix;
    matrix.at<double>(1,1) = iy;
    matrix.at<double>(2,2) = 1;
    matrix.at<double>(3,3) = 1;

    matrix.at<double>(0,3) = 0;//-0.5;
    matrix.at<double>(1,3) = 0;//-0.5;
    matrix.at<double>(2,3) = sz;

    return matrix.clone();
}


Mat Options::get_output_img2cam( Size const& sz )const{

    Mat matrix(4,4,CV_64FC1);

    matrix = Scalar(0);
    matrix.at<double>(0,0) = 1.0/sz.width;
    matrix.at<double>(1,1) = 1.0/sz.height;
    matrix.at<double>(2,2) = 1;
    matrix.at<double>(3,3) = 1;

    matrix.at<double>(0,3) = -0.5;
    matrix.at<double>(1,3) = -0.5;

    return matrix.clone();
}

Mat Options::get_output_cam2img( Size const& sz )const{

    Mat matrix(4,4,CV_64FC1);

    matrix = Scalar(0);
    matrix.at<double>(0,0) = sz.width;
    matrix.at<double>(1,1) = sz.height;
    matrix.at<double>(2,2) = 1;
    matrix.at<double>(3,3) = 1;
    matrix.at<double>(0,3) = sz.width/2.0;
    matrix.at<double>(1,3) = sz.height/2.0;
    return matrix;
}

Mat Options::get_output_img2cam( cv::Size const& sz, double const& ref )const{

    Mat matrix(4,4,CV_64FC1);
    matrix = Scalar(0);

    matrix.at<double>(0,0) = 1.0/ref;
    matrix.at<double>(1,1) = 1.0/ref;
    matrix.at<double>(2,2) = 1;
    matrix.at<double>(3,3) = 1;
    matrix.at<double>(0,3) = -((sz.width /ref)/2.0); 
    matrix.at<double>(1,3) = -((sz.height/ref)/2.0);

    return matrix;
}

Mat Options::get_output_cam2img( cv::Size const& sz, double const& ref )const{

    Mat matrix(4,4,CV_64FC1);
    matrix = Scalar(0);

    matrix.at<double>(0,0) = sz.width;
    matrix.at<double>(1,1) = sz.height;
    matrix.at<double>(2,2) = 1;
    matrix.at<double>(3,3) = 1;
    matrix.at<double>(0,3) = sz.width/2.0;
    matrix.at<double>(1,3) = sz.height/2.0;


    return matrix;
}

bool Options::doPerspective2Parallel()const{
    return perspective2parallel;
}

