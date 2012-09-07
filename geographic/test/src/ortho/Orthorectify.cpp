#include "Orthorectify.hpp"

#include "../math/Geometry.hpp"

#include <opencv2/highgui/highgui.hpp>

using namespace cv;

static void print_mat( const Mat& mat ){

    for( int y=0; y<mat.rows; y++){
        for( int x=0; x<mat.cols; x++){
            if(mat.type() == CV_64FC1)
                cout << mat.at<double>(y,x) << ", ";
        }
        cout << endl;
    }
}

static Mat load_vector( double const& x, double const& y, double const& z ){
    
    Mat out(4, 1, CV_64FC1);
    out.at<double>(0,0) = x;
    out.at<double>(1,0) = y;
    out.at<double>(2,0) = z;
    out.at<double>(3,0) = 0;

    return out;
}

static Mat load_point( double const& x, double const& y, double const& z ){

    Mat out(4, 1, CV_64FC1);
    out.at<double>(0,0) = x;
    out.at<double>(1,0) = y;
    out.at<double>(2,0) = z;
    out.at<double>(3,0) = 1;

    return out;
}

Mat orthorectify( Mat const& image, Options& options ){
    

    //we need to compute the required image size in order to properly align the image
    //  for now assume the image will remain the same size
    Size osize( options.image.size());

    //create the output image
    Mat output( osize, image.type());

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
    Mat P1 = options.Position_i;
    Mat P2 = input_principle_point;
    Mat P3 = load_point(0, 0, 0);
    Mat N  = earth_normal;
    Mat ground_point = compute_plane_line_intersection( P1, P2, N, P3);
    
    // this is the center of the camera on the rectified photo
    options.Position_f  = load_point( ground_point.at<double>(0,0), ground_point.at<double>(1,0), options.Position_i.at<double>(2,0));
    
    // this is the center of the image plane on the rectified photo
    Mat output_principle_point  = options.Position_f + focal_vector;
    
    // Iterate through the image
    int cnt = 0;
    for( int x=0; x<output.cols; x++)
        for( int y=0; y<output.rows; y++){

            //create a camera coordinate for the point
            Mat final_camera_point = (options.get_output_img2cam(osize) * load_point( x, y, 0 )) + focal_vector;
            
            //convert that to world coordinates
            Mat final_world_point = final_camera_point + options.Position_f - load_point(0,0,0);  
            
            //compute the intersection between the view vector and the earth
            P1 = final_world_point;
            P2 = options.Position_f;
            P3 = load_point(0,0,0);
            N  = earth_normal;
            
            //this is the location in world coordinates on where the starepoint intersects the input camera image plane
            // this is what the pixel in the output image is looking at in terms of geographic coordinates
            Mat stare_point = compute_plane_line_intersection( P1, P2, N, P3);
            
            //use the starepoint to compute the location in the original image
            P1 = stare_point.clone();
            P2 = options.Position_i;
            P3 = input_principle_point;
            N  = rotated_camera_normal.clone();
            
            //this is the location in world coordinates on where the starepoint intersects the input camera image plane
            Mat input_camera_plane_point = compute_plane_line_intersection( P2, P1, N, P3);
            
            //convert to the image coordinate system
            Mat cam_coord = options.RotationM.inv()*(input_camera_plane_point - options.Position_i) + load_point(0,0,0);
            Mat img_coord = options.get_output_cam2img(osize) * cam_coord;

            Point pnt( img_coord.at<double>(0,0), img_coord.at<double>(1,0));
            if( pnt.x >= 0 && pnt.x < output.cols && pnt.y >= 0 && pnt.y < output.rows ){
                
                //pull the image
                if( output.type() == CV_8UC1 )
                    output.at<uchar>(y,x) = image.at<uchar>(pnt);
                else if( output.type() == CV_8UC3 )    
                    output.at<Vec3b>(y,x) = image.at<Vec3b>(pnt);
                else
                    throw string("Unsupported pixel type");
            }

            if( cnt++ % 10000 == 0 )
                cout << 'x' << flush;
            

        }//end of x,y for loop
    
    namedWindow("OUT");
    imshow("OUT", output);
    waitKey(0);

    return output;

}
