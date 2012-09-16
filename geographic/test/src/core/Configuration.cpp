#include "Configuration.hpp"
#include "Utilities.hpp"

#include "Parser.hpp"

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
 * Open the configuration file and read values 
*/
void Options::load_configuration( const int& argc, char ** argv ){
    
    //Parse the config file
    PSR::Parser parser( argc, argv, config_filename );
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

    /**************************************/
    /*           Image Filename           */
    /**************************************/
    image_filename = parser.getItem_string("Image_Name", found);
    if((run_type == "FULL" || run_type == "RECTIFY") && found == false )
        throw string("ERROR: Image_File tag not found in configuration. Tag required by RUN_LEVELs FULL and RECTIFY");
  
    //make sure the file exists
    if((run_type == "FULL" || run_type == "RECTIFY") && PSR::Parser::fileExists( image_filename ) == false )
        throw string(string("ERROR: image file ")+image_filename+string(" does not exist"));

    
    /*****************************/
    /*        Focal Length       */
    /*****************************/
    //pull the focal length
    focal_length = parser.getItem_double("Focal_Length", found);
    if( found == false )
        throw string("ERROR: Focal_Length tag not found in configuration");
   
    
    /***********************/
    /*   BUILD_IMG_TYPE    */
    /***********************/
    build_image_type = parser.getItem_string("BUILD_IMG_TYPE", found );
    if((run_type == "FULL" || run_type == "BUILD") && found == false )
        throw string("ERROR: RUN_TYPE tag not found in configuration. Required by FULL and BUILD run_level.");
    
    if((run_type == "FULL" || run_type == "BUILD") && 
       ( build_image_type != "CV_8UC1" && build_image_type != "CV_8UC3" ))
        throw string(string("ERROR: BUILD_IMG_TYPE flag ")+build_image_type+string(" is invalid"));




    /*************************/
    /*   RECTIFY_IMG_TYPE    */
    /*************************/
    rectify_image_type = parser.getItem_string("RECTIFY_OUTPUT_TYPE", found);
    if( found == false )
        throw string("ERROR: RECTIFY_OUTPUT_TYPE NOT FOUND");
    
    if( run_type == "RECTIFY" ){
        if( rectify_image_type == "CV_8UC1" )
            image = imread( image_filename.c_str(), 0);    
        else if( rectify_image_type == "CV_8UC3" )
            image = imread( image_filename.c_str());
        else
            throw string("UNKNOWN TYPE");
    }
    

    int c = parser.getItem_int("BUILD_IMG_COLS", found );
    if( found == false )
        throw string("ERROR: BUILD_IMG_COLS tag not found in configuration");
    
    int r = parser.getItem_int("BUILD_IMG_ROWS", found );
    if( found == false )
        throw string("ERROR: BUILD_IMG_ROWS tag not found in configuration");

    build_image_size = Size(c, r);
    
    //load the Position Point
    vector<double> position = parser.getItem_vec_double("Camera_Position", found);
    if( found == false )
        throw string("ERROR: Camera Position tag invalid or not found");
    Position_i = load_point( position );

    //load the Rotation Quaternion Matrix
    vector<double> rot_axis = parser.getItem_vec_double("Camera_Rotation_Axis", found);
    if( found == false )
        throw string("ERROR: Camera Rotation axis tag invalid or not found");
    
    double rot_ang = parser.getItem_double("Camera_Rotation_Angle", found);
    if( found == false )
        throw string("ERROR: Camera Rotation angle tag invalid or not found");
    RotationQ = Quaternion( rot_ang*M_PI/180.0, vec( rot_axis[0], rot_axis[1], rot_axis[2]));
    RotationM = RotationQ.get_rotation_matrix();

    //check for the zbuffer flag
    zbufferEnabled = parser.getItem_bool("ZBUFFER_ENABLED", found);
    if( found == false )
        throw string("ERROR: ZBUFFER_ENABLED not found or enabled");
    
    //check for the perspective 2 parallel item
    perspective2parallel = parser.getItem_bool("RECTIFY_PERFORM_PERSPECTIVE_TO_PARALLEL", found);
    if( found == false )
        throw string("ERROR: Perspective 2 Parallel not found");
    
    //Check for dem image
    string demname = parser.getItem_string("DEM_Name", found);
    if( found == false )
        throw string("DEM_Name not found");

    dem = imread( demname.c_str(), 0);
    
}

bool Options::doZBuffering()const{
    return zbufferEnabled;
}


void Options::print(){
    
    string ENDL("\n");
    string output = ENDL;

    ostringstream sout;

    output += string("Program Configuration\n");
    output += string(" - Run Type      : ") +    run_type    + ENDL;
    output += string(" - Image Filename: ") + image_filename + ENDL;
    output += ENDL;
    output += string("Camera Configuration") + ENDL;
    output += string(" - Focal Length  : ") + num2str(focal_length) + ENDL;
    
    sout.clear(); sout.str(""); sout << RotationQ.get_angle(false);
    output += string(" - Rotation Angle: ") + sout.str() + ENDL;
    sout.clear(); sout.str(""); sout << RotationQ.get_axis();
    output += string(" - Rotation Axis : ") + sout.str() + ENDL;
    output += ENDL;
    
    cout << "Build Image Configuration" << endl;
    cout << " - Build Image Size: " << build_image_size.width << ", " << build_image_size.height << endl;
    cout << " - Build Image Type: " << build_image_type << endl;
    cout << " - Z Buffering     : ";
    if( zbufferEnabled == true )
        cout << "True" << endl;
    else
        cout << "False" << endl;
    cout << endl;
    cout << "Rectify Image Configuration" << endl;
    cout << " - Rectify Image Type: " << rectify_image_type << endl;
    cout << " - Perspective2Parallel: ";
    if( perspective2parallel == true )
        cout << "True" << endl;
    else
        cout << "False" << endl;
    cout << endl;
    
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

Mat Options::get_output_img2cam( Size const& sz )const{

    Mat matrix(4,4,CV_64FC1);

    matrix = Scalar(0);
    matrix.at<double>(0,0) = 1.0/sz.width;
    matrix.at<double>(0,3) = -0.5;
    matrix.at<double>(1,1) = 1.0/sz.height;
    matrix.at<double>(1,3) = -0.5;
    matrix.at<double>(2,2) = 1;
    matrix.at<double>(3,3) = 1;

    return matrix.clone();
}

Mat Options::get_build_img2cam( )const{

    Mat matrix(4,4,CV_64FC1);

    matrix = Scalar(0);
    matrix.at<double>(0,0) = 1.0/build_image_size.width;
    matrix.at<double>(0,3) = -0.5;
    matrix.at<double>(1,1) = 1.0/build_image_size.height;
    matrix.at<double>(1,3) = -0.5;
    matrix.at<double>(2,2) = 1;
    matrix.at<double>(3,3) = 1;
    
    return matrix.clone();
}

Mat Options::get_build_cam2img( )const{

    Mat matrix(4,4,CV_64FC1);
    
    matrix = Scalar(0);
    matrix.at<double>(0,0) = 1;//build_image_size.width;
    matrix.at<double>(1,1) = 1;//build_image_size.height;
    matrix.at<double>(3,3) = 1;
    matrix.at<double>(0,3) = build_image_size.width/2.0;
    matrix.at<double>(1,3) = build_image_size.height/2.0;
    return matrix;
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

