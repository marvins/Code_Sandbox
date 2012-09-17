#include "Orthorectify.hpp"

#include "../core/dem.hpp"
#include "../core/ProgressBar.hpp"
#include "../core/Utilities.hpp"
#include "../math/Geometry.hpp"

#include <opencv2/highgui/highgui.hpp>

using namespace cv;


Mat compute_location( Mat const& corner, Mat const& center, Mat const& principle_point, Mat const& earth_normal, Size sz, Options const& options ){

    
    /* Compute the camera coordinate of the point */ 
    Mat cam_point = options.get_output_img2cam( sz ) * corner + load_vector( 0, 0, -1);

    /* Compute the world coordinate of the point */
    Mat world_point = options.RotationM * cam_point + options.Position_i - load_point(0,0,0);

    Mat P1 = options.Position_i;
    Mat P2 = world_point;
    Mat P3 = load_point(0, 0, 0);
    Mat N  = earth_normal;
            
    /*  This is the ground point */
    Mat ground_point = compute_plane_line_intersection( P1, P2, N, P3);
    return ground_point;
    
}

double compute_gsd( Mat const& earth_normal, Size sz, Options const& options ){
    
    Mat cam_pnt01, cam_pnt02, cam_pnt11, cam_pnt12;

    /* Compute the camera locations of adjacent points */ 
    cam_pnt01 = options.get_output_img2cam( sz ) * load_point( 0, 0, 0) + load_vector( 0, 0, -1);
    cam_pnt02 = options.get_output_img2cam( sz ) * load_point( 1, 1, 0) + load_vector( 0, 0, -1);

    cam_pnt11 = options.get_output_img2cam( sz ) * load_point( sz.width-2, sz.height-2, 0) + load_vector( 0, 0, -1);
    cam_pnt12 = options.get_output_img2cam( sz ) * load_point( sz.width-1, sz.height-1, 0) + load_vector( 0, 0, -1);

    /* Convert the camera locations to world coordinates */
    cam_pnt01 = options.RotationM * cam_pnt01 + options.Position_i - load_point(0,0,0);
    cam_pnt02 = options.RotationM * cam_pnt02 + options.Position_i - load_point(0,0,0);
    
    cam_pnt11 = options.RotationM * cam_pnt11 + options.Position_i - load_point(0,0,0);
    cam_pnt12 = options.RotationM * cam_pnt12 + options.Position_i - load_point(0,0,0);
    
    /* Find the ground locations */
    cam_pnt01 = compute_plane_line_intersection( options.Position_i, cam_pnt01, earth_normal, load_point(0,0,0));
    cam_pnt02 = compute_plane_line_intersection( options.Position_i, cam_pnt02, earth_normal, load_point(0,0,0));
    cam_pnt11 = compute_plane_line_intersection( options.Position_i, cam_pnt11, earth_normal, load_point(0,0,0));
    cam_pnt12 = compute_plane_line_intersection( options.Position_i, cam_pnt12, earth_normal, load_point(0,0,0));
    
    /* Choose the GSD with the smallest value */
    double gsdTL = std::min( fabs(cam_pnt01.at<double>(0,0) - cam_pnt02.at<double>(0,0)),
                             fabs(cam_pnt01.at<double>(1,0) - cam_pnt02.at<double>(1,0)));
    
    double gsdBR = std::min( fabs(cam_pnt11.at<double>(0,0) - cam_pnt12.at<double>(0,0)),
                             fabs(cam_pnt11.at<double>(1,0) - cam_pnt12.at<double>(1,0)));
    
    return std::min( gsdTL, gsdBR);
}





/** Primary orthorectification module */
Mat orthorectify( Mat const& image, Options& options ){
    
    // the focal vector is the focal length multiplied by the normal to the camera
    Mat focal_vector = options.get_focal_length() * load_vector(0, 0, -1);
    
    //find the intersection between the center coordinate on the image plane and the surface of the earth
    Mat camera_normal         = load_vector( 0, 0, -1);
    Mat rotated_camera_normal = options.RotationM * camera_normal;
    
    Mat earth_normal          = load_vector( 0, 0,  1);
    Mat rotated_earth_normal  = options.RotationM * earth_normal;
    
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
    
    double width  = maxPnt.at<double>(0,0) - minPnt.at<double>(0,0);
    double height = maxPnt.at<double>(1,0) - minPnt.at<double>(1,0);

    //compute the gsd of the image as the smallest available gsd known
    double gsd = 1;//compute_gsd( earth_normal, image.size(), options );
    
    //create a new image which spans this length
    Size osize( width/gsd, height/gsd);
    
    //create the output image
    Mat output( osize, options.get_rectify_image_type());
    output = Scalar(0);
    
    //create some useful identities
    Mat world2cam = Mat::eye(4,4,CV_64FC1);
    matrix_add_translation( world2cam, (options.Position_i*-1) );
    
    Mat cam2img = options.get_output_cam2img(image.size())*options.RotationM.inv() * world2cam;
    
    bool foundIntersection = false;
    Point inputPix;
    
    double maxElevation = query_max_elevation( minPnt, maxPnt, ground_point, options );
    double dist;
    
    cout << "Output Image Size: " << output.cols << ", " << output.rows << endl;
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
                Mat img_coord = options.get_output_cam2img(image.size())*options.RotationM.inv() * world2cam * input_camera_plane_point;
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

        if( x % 10 == 0 ){
            imwrite("temp.jpg", output);
        }
        //print the progress bar to console
        if( show_progress_bar )
            cout << progressBar.toString() << '\r' << flush;

    }//end of x,y for loop
    
    if( show_progress_bar )
        cout << endl;

    imwrite("temp.jpg", output);
    return output;

}


