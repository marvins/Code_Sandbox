#include "Orthorectify.hpp"

#include "../core/dem.hpp"

#include "../core/dem.hpp"
#include "../core/ProgressBar.hpp"
#include "../core/Utilities.hpp"
#include "../math/Geometry.hpp"

#include <opencv2/highgui/highgui.hpp>


/**
 *  ASSUMPTIONS:
 *  1. Principle point is in the center of the image.
 *  2. Pixels in an image are square in real life.
*/


/**
 * Convert the World Coordinate to Pixel Coordinates
*/
Point convert_world2pixel( Mat const& world_coordinate, 
                           Mat const& camera_position,
                           Mat const& camera_principle_point,
                           Mat const& camera_normal,
                           Mat const& camera_rotation,
                           Size_<double> focal_plane_size,
                           Size image_size
                           ){
    
    /**
     * Compute the intersection between the world coordinate
     * and the focal plane on the desired camera. 
    */
    Mat world_plane_coordinate = compute_plane_line_intersection( world_coordinate, 
                                                                  camera_position, 
                                                                  camera_normal, 
                                                                  camera_principle_point);
    
    /**
     * Transform coordinate into the camera plane coordinate system
     *
     * - Subtract the world coordinate by the principle point of the camera
     *   * This will normalize the point about the focal plane
     *   * rotate the point by the plane rotation to level the point on a flat surface
    */
    Mat cam2plane = Mat::eye(4,4,CV_64FC1);
    cam2plane.at<double>(0,3) = -camera_principle_point.at<double>(0,0);
    cam2plane.at<double>(1,3) = -camera_principle_point.at<double>(1,0);
    cam2plane.at<double>(2,3) = -camera_principle_point.at<double>(2,0);
    
    //rotate by the camera rotation inverse
    cam2plane = camera_rotation.inv() * cam2plane;

    /** 
     * Transform camera plane coordinate into the pixel space
     *
     * - add half of the image focal plane size back to make top-left origin
     * - divide by the focal plane size to make from [0,1]
     * - mulitply by the image size to make from [0, max rows]
    */
    Mat orig_shift = Mat::eye(4,4,CV_64FC1);
    orig_shift.at<double>(0,3) = focal_plane_size.width /2.0;
    orig_shift.at<double>(1,3) = -focal_plane_size.height/2.0;
        
    Mat plane2pix = Mat::eye(4,4,CV_64FC1);
    plane2pix.at<double>(0,0) = image_size.width / focal_plane_size.width;
    plane2pix.at<double>(1,1) = -image_size.height/ focal_plane_size.height;

    plane2pix = plane2pix * orig_shift;

    
    /** 
     * Apply all transformations
    */
    Mat output = plane2pix * cam2plane * world_plane_coordinate;
    
    // return point
    return Point( _round(output.at<double>(0,0)), _round(output.at<double>(1,0)));
}

/**
 * Convert the Pixel Coordinate to World Coordinates
*/
Mat  convert_pixel2world(  Mat const&       pixel_coord, 
                           Size const&      image_size, 
                           Size_<double>    image_plane_size, 
                           Mat const&       rotation_axis,
                           double const&    focal_length,
                           Mat const&       camera_position,
                           Mat const&       earth_normal      ){ 


    /**************************/
    /*     Pixel 2 Plane      */
    /**************************/
    // convert the pixel to camera plane coordinates
    Mat pix2plane = Mat::eye( 4, 4, CV_64FC1 );
    
    // this will scale the pixels to match the range of the focal plane
    pix2plane.at<double>(0,0) = (double) image_plane_size.width / image_size.width;
    pix2plane.at<double>(1,1) = (double) -image_plane_size.height/ image_size.height;
    
    // this will center the pixel on the principle point which currently is assumed to be the center of the image.
    pix2plane.at<double>(0,3) = -image_plane_size.width / 2.0;
    pix2plane.at<double>(1,3) = image_plane_size.height/ 2.0;
    
    /********************************************/
    /*      Plane 2 Camera Transformation       */
    /********************************************/
    // now compute the transformation from pixel to local camera coordinates
    Mat plane2camera = Mat::eye( 4, 4, CV_64FC1 );
    
    // translate by ( 0 - focal_length )
    plane2camera.at<double>(2,3) = - focal_length;

    // apply rotation
    plane2camera = rotation_axis * plane2camera;  
    

    /**********************************************/
    /*       Camera 2 World Transformation        */
    /**********************************************/
    Mat camera2world = Mat::eye( 4, 4, CV_64FC1 );

    // give translation parameters equal to world camera position
    camera2world.at<double>(0,3) = camera_position.at<double>(0,0);
    camera2world.at<double>(1,3) = camera_position.at<double>(1,0);
    camera2world.at<double>(2,3) = camera_position.at<double>(2,0);

    /**********************************/
    /*      Apply Transformation      */
    /**********************************/
    // multiply pixel coordinate by the pixel 2 coordinate transform
    Mat plane_coord = camera2world * plane2camera * pix2plane * pixel_coord;
    
    /********************************************************************/
    /*     Compute the Intersection Between the view and the ground     */
    /********************************************************************/
    Mat ground_coord = compute_plane_line_intersection( camera_position, plane_coord, earth_normal, load_point( 0, 0, 0) );
    
    return ground_coord;
}


/**
 * Compute the ground sampling distance. 
 * 
 * The ground sampling distance is the ratio between the ground range and the 
 * input image size.  Also factored in is the rotation angle.
*/
pair<double,double> compute_gsd( Mat const& tl, Mat const& tr, Mat const& bl, Mat const& br,  
                                 Size const& image_size, Mat const& rotation_axis ){

    // take the average span of the image width and height, then divide by the number of pixels 
    // to determine a reasonable gsd
    double topX =  fabs(tl.at<double>(0,0) - tr.at<double>(0,0));
    double botX =  fabs(bl.at<double>(0,0) - br.at<double>(0,0));
    double midX =  ( topX + botX ) / 2.0; 
    
    double minY =  fabs(tl.at<double>(1,0) - bl.at<double>(1,0));
    double maxY =  fabs(tr.at<double>(1,0) - br.at<double>(1,0));
    double midY =  ( minY + maxY ) / 2.0;
    
    // compute gsd elements
    double gsdx = midX / image_size.width;
    double gsdy = midY / image_size.height;

    //return the variable
    return pair<double,double>( gsdx, gsdy );
}



/** 
 * Primary orthorectification module 
*/
Mat orthorectify( Mat const& image, Options& options ){
    
    /**
     * Compute some focal length-based vectors
    */
    
    // focal length ( input parameter )
    double focal_length = options.get_focal_length();

    // output focal vector ( looks straight down )
    Mat output_focal_vector = load_vector( 0, 0, -focal_length );

    // input focal vector ( rotated by quaternion )
    Mat  input_focal_vector = options.RotationM * output_focal_vector;

    /**
     * Need to compute the camera plane normals
    */
    
    // this is a unit vector in the direction of the output camera
    Mat  output_camera_normal = load_vector( 0, 0, -1);

    // this is a unit vector in the direction of the input camera
    Mat  input_camera_normal  = options.RotationM * output_camera_normal;

    // this is a unit vector in the direction of the earth normal
    Mat  earth_normal         = load_vector( 0, 0, 1);
    
    // this is the rotation axis
    Mat rotation_axis = options.RotationM;

    /**
     * Need to compute the position displacement vectors
    */
    // the position of the input camera in world coordinates
    Mat input_camera_position = options.Position_i;

    // the position of the input camera in world coordinates ( vector format )
    Mat input_camera_position_vector = load_vector( input_camera_position.at<double>(0,0), 
                                                    input_camera_position.at<double>(1,0), 
                                                    input_camera_position.at<double>(2,0));
    
    // The principle point on the input image is the camera position plus the focal vector
    Mat input_camera_principle_point = input_camera_position + input_focal_vector;
    
    /**
     * Build a size object for the focal plane size
    */
    Size_<double> image_plane_size( options.camera_plane_width, options.camera_plane_height );


    /**
     * Compute the location of the 4 camera corners in world coordinates
    */
    
    // pixel coordinates
    Mat corner00_pixel = load_point(                0  ,                0  , 0 );
    Mat corner10_pixel = load_point( options.image.cols,                0  , 0 );
    Mat corner01_pixel = load_point(                0  , options.image.rows, 0 );
    Mat corner11_pixel = load_point( options.image.cols, options.image.rows, 0 );

    // world coordinates
    Mat corner00_world = convert_pixel2world( corner00_pixel, options.image.size(),  image_plane_size,  rotation_axis, focal_length, input_camera_position, earth_normal );
    Mat corner10_world = convert_pixel2world( corner10_pixel, options.image.size(),  image_plane_size,  rotation_axis, focal_length, input_camera_position, earth_normal );
    Mat corner01_world = convert_pixel2world( corner01_pixel, options.image.size(),  image_plane_size,  rotation_axis, focal_length, input_camera_position, earth_normal );
    Mat corner11_world = convert_pixel2world( corner11_pixel, options.image.size(),  image_plane_size,  rotation_axis, focal_length, input_camera_position, earth_normal );
    
    /**
     * Convert the world coordinates into a image bounding box
    */
    Rect_<double> ground_bbox = compute_ground_bbox( corner00_world, corner01_world, corner10_world, corner11_world ); 
    
    cout << "Zone: " << options.zone << endl;
    cout << "ISNORTH: " << options.isNorth << endl;
    cout << "GROUND BBOX: " << ground_bbox.tl() << ", " << ground_bbox.br() << endl;
    cin.get();
    
    //load dem
    if( options.dem_mode == "FILE" ){
        
        throw string("UNTESTED");
        //options.minDem = GEO::CoordinateUTM( -500,  500, 0);
        //options.maxDem = GEO::CoordinateUTM(  500, -500, 0);
        //options.dem   = GEO::DEM( GEO::convert_coordinate(options.minDem), GEO::convert_coordinate(options.maxDem), imread( options.dem_filename, 0 ) );
        //options.max_elevation = options.dem.max_elevation( );
    }
    else if( options.dem_mode == "DTED" ){
        
        options.minDem = GEO::CoordinateUTM( options.zone, options.isNorth, ground_bbox.tl().y, ground_bbox.tl().x, 0);
        options.maxDem = GEO::CoordinateUTM( options.zone, options.isNorth, ground_bbox.br().y, ground_bbox.br().x, 0);
        options.dem = GEO::DEM( GEO::convert_coordinate( options.minDem ).toPoint2f(),
                                GEO::convert_coordinate( options.maxDem ).toPoint2f(),
                                GEO::DEM_Params( GEO::DTED, options.dem_filename ));
        
        options.logger.add_message( LOG_INFO, string("DEM NE Corner: (") + num2str(options.dem.ne().x) + string(", ") + num2str(options.dem.ne().y) + string(") "));
        options.logger.add_message( LOG_INFO, string("DEM NW Corner: (") + num2str(options.dem.nw().x) + string(", ") + num2str(options.dem.nw().y) + string(") "));
        options.logger.add_message( LOG_INFO, string("DEM SE Corner: (") + num2str(options.dem.se().x) + string(", ") + num2str(options.dem.se().y) + string(") "));
        options.logger.add_message( LOG_INFO, string("DEM SW Corner: (") + num2str(options.dem.sw().x) + string(", ") + num2str(options.dem.sw().y) + string(") "));
        cin.get(); 
    }
    else
        throw string("ERROR: Unknown DEM Mode");

    /** 
     * Create a simple polygon which we can use to test if points in the output image exist 
     * in the input image.  For images with extreme rotations, this will save significant 
     * processing power. 
    */
    vector<Point2f> imgPolygon;
    imgPolygon.push_back( Point2f(corner00_world.at<double>(0,0), corner00_world.at<double>(1,0)));
    imgPolygon.push_back( Point2f(corner10_world.at<double>(0,0), corner10_world.at<double>(1,0)));
    imgPolygon.push_back( Point2f(corner11_world.at<double>(0,0), corner11_world.at<double>(1,0)));
    imgPolygon.push_back( Point2f(corner01_world.at<double>(0,0), corner01_world.at<double>(1,0)));


    /**
     * Now that we have the region outlined, we need to compute the proper image size. 
     *
     * This requires us to compute the GSD as we don't yet know what one pixel means
    */
    pair<double,double> gsd = compute_gsd(  corner00_world, corner10_world, 
                                            corner01_world, corner11_world, 
                                            options.image.size(), rotation_axis );
    
    
    /** 
     * The image size is the gsd multiplied with the image dimensions
    */
    Size osize( ground_bbox.width  / gsd.first  + 1,
                ground_bbox.height / gsd.first + 1);
    
    double viewScale = (double) osize.width / osize.height;
    

    // initialize the output image
    // TODO use the option from the config file
    Mat output( osize, CV_8UC3 );
    output = Scalar(0);
    
    /**
     *        create a progress bar object and make 
     *        sure we have permission to show it 
    */
    ProgressBar progressBar(0, output.cols*output.rows, 50);
    bool show_progress_bar = false;
    if( ( options.logger.get_console_run_state() & LOG_INFO ) == LOG_INFO )
        show_progress_bar = true;

    options.logger.add_message( LOG_INFO, string("Input  image size: ") + num2str(options.image.cols) + string(" x ") + num2str(options.image.rows));
    options.logger.add_message( LOG_INFO, string("Output image size: ") + num2str(output.cols) + string(" x ") + num2str(output.rows));
    options.logger.add_message( LOG_INFO, string("Max Elevation: ") + num2str(options.max_elevation));
    options.logger.add_message( LOG_INFO, string("GSD: ") + num2str(gsd.first) );
    options.logger.add_message( LOG_INFO, string("TL World: ( ") + num2str(corner00_world.at<double>(0,0)) + string(", ") +
                                                                   num2str(corner00_world.at<double>(1,0)) + string(", ") +
                                                                   num2str(corner00_world.at<double>(2,0)) + string(") "));
    options.logger.add_message( LOG_INFO, string("TR World: ( ") + num2str(corner10_world.at<double>(0,0)) + string(", ") +
                                                                   num2str(corner10_world.at<double>(1,0)) + string(", ") +
                                                                   num2str(corner10_world.at<double>(2,0)) + string(") "));
    options.logger.add_message( LOG_INFO, string("BL World: ( ") + num2str(corner01_world.at<double>(0,0)) + string(", ") +
                                                                   num2str(corner01_world.at<double>(1,0)) + string(", ") +
                                                                   num2str(corner01_world.at<double>(2,0)) + string(") "));
    options.logger.add_message( LOG_INFO, string("BR World: ( ") + num2str(corner11_world.at<double>(0,0)) + string(", ") +
                                                                   num2str(corner11_world.at<double>(1,0)) + string(", ") +
                                                                   num2str(corner11_world.at<double>(2,0)) + string(") "));
    options.logger.add_message( LOG_INFO, string("Cam Position: ( ") + num2str(options.Position_i.at<double>(0,0)) + string(", ") +
                                                                       num2str(options.Position_i.at<double>(1,0)) + string(", ") +
                                                                       num2str(options.Position_i.at<double>(2,0)) + string(") "));


    /**
     *    Iterate the Output Image, performing orthorectification
    */
    int cnt = 0;
    bool occluded; 
    for( int y=0; y<output.rows; y++){
        for( int x=0; x<output.cols; x++){
            
            /**
             * Compute the geographic position of the output pixel location
             *
             * - Since we are dealing with an axis-aligned coordinate system,
             *   just interpolate the position against the bbox
            */
            Mat world_position = load_world_point( Point(x,y), osize, ground_bbox, options.dem );
            

            // TODO make sure this function loads the proper elevation for this coordinate

            /**
             * Make sure the world position resides in the input image. Otherwise skip.
            */
            if( pointInConvexPolygon( imgPolygon, Point2f(world_position.at<double>(0,0), world_position.at<double>(1,0)) ) == false )continue;

            /** 
             * Convert the world coordinate into a pixel value in the input camera
             * camera system.
            */
            
            /**
             * TODO Compute any dem induced intersections here
            */
            occluded = dem_correction( world_position, gsd.first, options, load_point( ground_bbox.tl().x, ground_bbox.br().y, 0),
                                                                           load_point( ground_bbox.br().x, ground_bbox.tl().y, 0));
            if( occluded == true ){
                
                if( output.type() == CV_8UC3 && options.image.type() == CV_8UC3 ){
                    output.at<Vec3b>(y,x) = Vec3b(0,255,0);
                }
                else if( output.type() == CV_8UC1 && options.image.type() == CV_8UC1 ){
                    output.at<uchar>(y,x) = 0;
                }
                else 
                    throw string("ERROR: Unknown data types" );
            }

            /**
             * Convert the world coordinate into pixel value
            */
            Point pixel_position = convert_world2pixel( world_position, 
                                                        input_camera_position,
                                                        input_camera_principle_point,
                                                        input_camera_normal,
                                                        rotation_axis,
                                                        image_plane_size,
                                                        options.image.size());


            // apply to image
            if( pixel_position.x >= 0 && pixel_position.y >= 0 && pixel_position.x < options.image.cols && pixel_position.y < options.image.rows ){

                if( output.type() == CV_8UC3 && options.image.type() == CV_8UC3 ){
                    output.at<Vec3b>(y,x) = options.image.at<Vec3b>(pixel_position);
                }
                else if( output.type() == CV_8UC1 && options.image.type() == CV_8UC1 ){
                    output.at<uchar>(y,x) = options.image.at<uchar>(pixel_position);
                }
                else 
                    throw string("ERROR: Unknown data types" );

            }

            
            //update the progress bar
            if( show_progress_bar )
                progressBar.update( cnt );
            
            // temporary debugging tool to allow you to view results, mid run
            if( cnt % 100000 == 0 ){
                imwrite("temp.jpg",output);
            }
            cnt++;
        }
        //print the progress bar to console
        if( show_progress_bar ){
            cout << progressBar.toString() << '\r' << flush;
        }

    }

    return output;
}


