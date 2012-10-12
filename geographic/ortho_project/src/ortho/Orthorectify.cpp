#include "Orthorectify.hpp"

#include "../core/dem.hpp"
#include "../core/ProgressBar.hpp"
#include "../core/Utilities.hpp"
#include "../math/Geometry.hpp"

#include <opencv2/highgui/highgui.hpp>

using namespace cv;

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
    

    /**
     * Now that we have the region outlined, we need to compute the proper image size. 
     *
     * This requires us to compute the GSD as we don't yet know what one pixel means
    */
    pair<double,double> gsd = compute_gsd(  corner00_world, corner10_world, 
                                            corner01_world, corner11_world, 
                                            options.image.size(), rotation_axis );
    
    
    
    // 
    cout << "Image Corner World Coordinate Projections" << endl;
    cout << "tl: "; print_mat( corner00_world.t() );
    cout << "tr: "; print_mat( corner10_world.t() );
    cout << "bl: "; print_mat( corner01_world.t() );
    cout << "br: "; print_mat( corner11_world.t() );
    cout << endl;

    // 
    cout << "Image Ground Sampling Distance" << endl;
    cout << "x : " << gsd.first  << " meters/pixel" << endl;
    cout << "y : " << gsd.second << " meters/pixel" << endl;
    cout << endl;

    /** 
     * The image size is the gsd multiplied with the image dimensions
    */
    Size osize( ground_bbox.width  * gsd.first  + 1,
                ground_bbox.height * gsd.second + 1);
    
    double viewScale = (double) osize.width / osize.height;
    
    cout << "input size: " << options.image.cols << ", " << options.image.rows << endl;
    cout << "output sz : " << osize.width << ", " << osize.height << endl;

    // initialize the output image
    // TODO use the option from the config file
    Mat output( osize, CV_8UC3 );
    output = Scalar(0);

    /**
     *    Iterate the Output Image, performing orthorectification
    */
    int cnt = 0;
    
    namedWindow("OUTPUT");
    for( int y=0; y<output.rows; y++){
        for( int x=0; x<output.cols; x++){
            
            cnt++;
            /**
             * Compute the geographic position of the output pixel location
             *
             * - Since we are dealing with an axis-aligned coordinate system,
             *   just interpolate the position against the bbox
            */
            Mat world_position = load_world_point( Point(x,y), osize, ground_bbox );
        
            /** 
             * Convert the world coordinate into a pixel value in the input camera
             * camera system.
            */
            
            /**
             * TODO Compute any dem induced intersections here
            */
            //dem_correction( world_position )

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


            if( cnt % 100000 == 0 ){
                cout << x << ", " << y << endl;
                imwrite("temp.jpg",output);
                imshow("OUTPUT", output);
                waitKey(0);
            }
        }
    }

    
    /*
    // this is the center of the image plane on the original photo.  
    Mat input_principle_point = options.Position_i + (options.get_focal_length() * rotated_camera_normal);
    
    //find the intersection between the earth plane and the view vector
    Mat ground_point = compute_plane_line_intersection( options.Position_i, input_principle_point, earth_normal, load_point(0,0,0));
    
    // this is the center of the camera on the non-rectified photo
    options.Position_f  = load_point( ground_point.at<double>(0,0), ground_point.at<double>(1,0), options.Position_i.at<double>(2,0));
    
    // this is the center of the image plane on the non-rectified photo
    Mat output_principle_point  = options.Position_f + focal_vector;
    
    Mat tl_image = load_point(      0    ,      0    , 0);
    Mat tr_image = load_point( image.cols,      0    , 0);
    Mat bl_image = load_point(      0    , image.rows, 0);
    Mat br_image = load_point( image.cols, image.rows, 0);
   
    // these are the geographic corners of the image
    Mat tl_world = compute_location( tl_image, ground_point, input_principle_point, earth_normal, image.size(), options );
    Mat tr_world = compute_location( tr_image, ground_point, input_principle_point, earth_normal, image.size(), options );
    Mat bl_world = compute_location( bl_image, ground_point, input_principle_point, earth_normal, image.size(), options );
    Mat br_world = compute_location( br_image, ground_point, input_principle_point, earth_normal, image.size(), options );
    
    //compute the coordinates of the image on the image plane
    Mat tl_plane = options.RotationM.inv() * (compute_vector_norm( tl_world - options.Position_i)*options.get_focal_length());
    Mat tr_plane = options.RotationM.inv() * (compute_vector_norm( tr_world - options.Position_i)*options.get_focal_length());
    Mat bl_plane = options.RotationM.inv() * (compute_vector_norm( bl_world - options.Position_i)*options.get_focal_length());
    Mat br_plane = options.RotationM.inv() * (compute_vector_norm( br_world - options.Position_i)*options.get_focal_length());
    
    double scalePlane = -1.0 / tl_plane.at<double>(2,0); 
    
    tl_plane = tl_plane * scalePlane;
    tr_plane = tr_plane * scalePlane;
    bl_plane = bl_plane * scalePlane;
    br_plane = br_plane * scalePlane;

    double xExtent = fabs( tl_plane.at<double>(0,0) - br_plane.at<double>(0,0));
    double yExtent = fabs( tl_plane.at<double>(1,0) - br_plane.at<double>(1,0));
    
    vector<Point2f> imgPolygon;
    imgPolygon.push_back( Point2f(tl_world.at<double>(0,0), tl_world.at<double>(1,0)));
    imgPolygon.push_back( Point2f(tr_world.at<double>(0,0), tr_world.at<double>(1,0)));
    imgPolygon.push_back( Point2f(br_world.at<double>(0,0), br_world.at<double>(1,0)));
    imgPolygon.push_back( Point2f(bl_world.at<double>(0,0), bl_world.at<double>(1,0)));

    Mat maxPnt = load_point(  std::max( tl_world.at<double>(0,0), std::max( tr_world.at<double>(0,0), std::max( bl_world.at<double>(0,0), br_world.at<double>(0,0)))),
                              std::max( tl_world.at<double>(1,0), std::max( tr_world.at<double>(1,0), std::max( bl_world.at<double>(1,0), br_world.at<double>(1,0)))),
                              std::max( tl_world.at<double>(2,0), std::max( tr_world.at<double>(2,0), std::max( bl_world.at<double>(2,0), br_world.at<double>(2,0)))));
    
    Mat minPnt = load_point(  std::min( tl_world.at<double>(0,0), std::min( tr_world.at<double>(0,0), std::min( bl_world.at<double>(0,0), br_world.at<double>(0,0)))),
                              std::min( tl_world.at<double>(1,0), std::min( tr_world.at<double>(1,0), std::min( bl_world.at<double>(1,0), br_world.at<double>(1,0)))),
                              std::min( tl_world.at<double>(2,0), std::min( tr_world.at<double>(2,0), std::min( bl_world.at<double>(2,0), br_world.at<double>(2,0)))));
    
    cout << "Max Point: " << maxPnt << endl;
    cout << "Min Point: " << minPnt << endl;
    cin.get();

    double width  = maxPnt.at<double>(0,0) - minPnt.at<double>(0,0);
    double height = maxPnt.at<double>(1,0) - minPnt.at<double>(1,0);
    
    cout << "DIMENSIONS" << endl;
    cout << (double)image.cols / image.rows << endl;
    cout << (double)width      / height     << endl;
    cin.get();

    //compute the gsd of the image as the smallest available gsd known
    double gsd = .55;//compute_gsd( earth_normal, image.size(), options );
    
    //create a new image which spans this length
    Size osize( width/gsd, height/gsd);
    
    cout << "output size: " << osize.width << ", " << osize.height << endl;
    cin.get();

    //create the output image
    Mat output( osize, options.get_rectify_image_type());
    output = Scalar(0);
    
    //create some useful identities
    Mat world2cam = Mat::eye(4,4,CV_64FC1);
    matrix_add_translation( world2cam, (options.Position_i*-1) );
    
    Mat cam2img = options.get_output_cam2img(image.size())*options.RotationM.inv() * world2cam;
   

    //matrix to convert a world coordinate to camera coordinate
    Mat camShift = Mat::eye( 4, 4, CV_64FC1);
    camShift.at<double>(0,3) = image.cols/2.0;
    camShift.at<double>(1,3) = image.rows/2.0;
    camShift.at<double>(2,3) = 0;

    Mat camScale = Mat::eye( 4, 4, CV_64FC1);
    camScale.at<double>(0,0) = (image.cols / xExtent);
    camScale.at<double>(1,1) = (image.rows / yExtent);
    //camShift.at<double>(0,3) = osize.width/2.0;
    //camShift.at<double>(1,3) = osize.height/2.0;

    Mat cam2pix = camShift * camScale;
    
    bool foundIntersection = false;
    Point inputPix;
    
    double maxElevation = query_max_elevation( minPnt, maxPnt, ground_point, options );
    double dist;
    
    /////////////////////////////////////////////////////////
    //        Create some random variables to test on      //
    /////////////////////////////////////////////////////////
    Point3f camera_origin = Mat2Point3f( options.Position_i );
    Mat     PositionTranslation = Mat::eye(4, 4, CV_64FC1);
    matrix_add_translation( PositionTranslation, options.Position_i);

    /////////////////////////////////////////////////////////
    //        create a progress bar object and make        //
    //          sure we have permission to show it         //
    /////////////////////////////////////////////////////////
    ProgressBar progressBar(0, output.cols*output.rows, 50);
    bool show_progress_bar = false;
    if( ( options.logger.get_console_run_state() & LOG_INFO ) == LOG_INFO )
        show_progress_bar = true;
    
    
    /////////////////////////////////////////////////////////////////////
    //      Iterate the Output Image, performing orthorectification    //
    /////////////////////////////////////////////////////////////////////
    int cnt = 0;
    double distRadius;
    
    for( int x=0; x<output.cols; x++){
        for( int y=0; y<output.rows; y++){
            

            //compute the expected location of the output pixel in geographic coordinates
            Mat stare_point = load_point( ((double)x/output.cols)*(maxPnt.at<double>(0,0) - minPnt.at<double>(0,0)) + minPnt.at<double>(0,0), 
                                          ((double)y/output.rows)*(maxPnt.at<double>(1,0) - minPnt.at<double>(1,0)) + minPnt.at<double>(1,0), 
                                                                                0                                                         );
            
            // make sure that the point in the output image is actually in the input image
            Point2f starePoint( stare_point.at<double>(0,0), stare_point.at<double>(1,0));
            if( pointInConvexPolygon( imgPolygon, starePoint ) == false )continue;
            
            
            //Only run if doing Perspective 2 Parallel Transformation
            if( options.doPerspective2Parallel() == true ){
                
                //only consider elevation here
                stare_point.at<double>(2,0) = query_dem( stare_point,  ground_point, options);
               
                //one useful limit will be to compute the distance at which the max height is below
                // the stare point vector
                Mat maxDistance = compute_plane_line_intersection( options.Position_i, stare_point, earth_normal, load_point(0,0,maxElevation));
                distRadius = norm( Point2f( maxDistance.at<double>(0,0), maxDistance.at<double>(1,0)) - starePoint);

                //we need to compute the bounding box from the stare point to the origin
                // compare this with the maximum possible distance
                Point3f pntMin( std::min( stare_point.at<double>(0,0)-distRadius, std::min( stare_point.at<double>(0,0), options.Position_i.at<double>(0,0))),
                                std::min( stare_point.at<double>(1,0)-distRadius, std::min( stare_point.at<double>(1,0), options.Position_i.at<double>(1,0))),0);
                
                Point3f pntMax( std::min( stare_point.at<double>(0,0)+distRadius, std::max( stare_point.at<double>(0,0), options.Position_i.at<double>(0,0))),
                                std::min( stare_point.at<double>(1,0)+distRadius, std::max( stare_point.at<double>(1,0), options.Position_i.at<double>(1,0))),0);
                
                
                //now that we know the range, we need to search the elevation data
                // this would be a good moment to compute the relative accuracy or granularity of the info
                double gsd = 1; //one pixel per one unit of measure

                double ranX = (pntMax.x - pntMin.x)*gsd;
                double ranY = (pntMax.y - pntMin.y)*gsd;
                
                foundIntersection = false;
                
                //create some variables for the intersection 
                Point3f u = Mat2Point3f(stare_point) - Mat2Point3f(options.Position_i); 
                double  a = u.dot(u);


                //begin iterating over image
                for( int xx=0; xx<ranX && !foundIntersection; xx++){
                for( int yy=0; yy<ranY && !foundIntersection; yy++){
                    
                    //compute the actual geographical location
                    Point3f pos = pntMin + Point3f(xx*gsd, yy*gsd, 0);
                    
                    //don't process if it is beyond the radius
                    if( norm(Point2f(pos.x, pos.y) - starePoint ) > distRadius )continue;
                    
                    //query the dem data for the right elevation
                    pos.z = query_dem( pos, Mat2Point3f(ground_point), options);
                    
                    //make sure we are not evaluating the same point
                    if( norm(pos - Mat2Point3f(stare_point)) > 0.11 ){
                    
                    //make sure the elevation is higher than the test pixel
                    if( pos.z > stare_point.at<double>(2,0) ){

                        //make sure point intersects line on 2D level
                        if( compute2d_line_point_distance( camera_origin, Mat2Point3f(stare_point), pos ) < 1.5 ){

                            //check if the lines intersect
                            int result = compute3d_line_line_intersection( camera_origin, u,
                                                                            pos, Point3f(0, 0, -pos.z), 
                                                                            a, dist, 1 );


                            if( result != 0 ){//we have an intersection and cannot paint the point
                                foundIntersection = true;
                            }

                        }
                    }
                    }

                }} // end of xx, yy loop
                
                if( foundIntersection == true ){
                    if( output.type() == CV_8UC1 )
                        output.at<uchar>(y,x) = 0;
                    else if( output.type() == CV_8UC3 )    
                        output.at<Vec3b>(y,x) = Vec3b(0,255,0);
                    else
                        throw string("Unsupported pixel type");
                }
                else{ // we do not have an intersection

                    //now compute the expected location
                    //compute the location on the ground where this vector intersects with the earth plane
                    Mat imgPosition = compute_plane_line_intersection( options.Position_i, stare_point, earth_normal, load_point(0,0,0));
                    
                    //compute where this point intersects the input camera plane
                    //convert this to actual pixel coordinates
                    Mat pixPos = cam2img * compute_plane_line_intersection( options.Position_i, imgPosition, rotated_camera_normal, input_principle_point);
                    Point pixLoc( _round(pixPos.at<double>(0,0)), _round(pixPos.at<double>(1,0)));
                    
                    //make sure the pixel is inside the image
                    if( pixLoc.x >= 0 && pixLoc.y >= 0 && pixLoc.x < image.cols && pixLoc.y < image.rows ){

                        if( output.type() == CV_8UC1 )
                            output.at<uchar>(y,x) = image.at<uchar>(pixLoc);
                        else if( output.type() == CV_8UC3 )    
                            output.at<Vec3b>(y,x) = image.at<Vec3b>(pixLoc);
                        else
                            throw string("Unsupported pixel type");
                    }

                }
            }
            else{
                
        
                //this is the location in world coordinates on where the starepoint intersects the input camera image plane
                Mat input_camera_plane_point = compute_plane_line_intersection( options.Position_i, 
                        stare_point, 
                        rotated_camera_normal, 
                        input_principle_point);
                
                
                //convert the world coordinate into local camera coordinates
                //convert to the image coordinate system
                
                Mat img_coord = cam2pix * options.RotationM.inv() * world2cam * input_camera_plane_point;
                Point pnt( _round(img_coord.at<double>(0,0)), _round(img_coord.at<double>(1,0)));
                

                if( pnt.x >= 0 && pnt.x < image.cols && pnt.y >= 0 && pnt.y < image.rows ){

                    //pull the image
                    if( output.type() == CV_8UC1 )
                        output.at<uchar>(y,x) = image.at<uchar>(pnt);
                    else if( output.type() == CV_8UC3 )    
                        output.at<Vec3b>(y,x) = image.at<Vec3b>(pnt);
                    else
                        throw string("Unsupported pixel type");
                }
            }

            //update the progress bar
            if( show_progress_bar )
                progressBar.update( cnt );
            cnt++;

        }

        //print the progress bar to console
        if( show_progress_bar ){
            cout << progressBar.toString() << '\r' << flush;
        }

    }//end of x,y for loop
    
    if( show_progress_bar )
        cout << endl;

    imwrite("temp.jpg", output);
    return output;
    */

    return output;
}


