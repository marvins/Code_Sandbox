#include "Perspective.hpp"
#include "../math/Geometry.hpp"

#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

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

static double _round( double const& val ){
    return std::floor( val + 0.5 );
}

Mat generate_perspective_test_image( Options& options ){

    //create an output image
    Mat output, dem, flat_img;
    create_flat_test_image( options, flat_img, dem );
    options.dem = dem;
    
    namedWindow("Flat Test Image");
    imshow("Flat Test Image", flat_img);
    imshow("DEM", dem);
    waitKey(0);
    destroyWindow("Flat Test Image");

    //rotate the scene of the image accordingly
    rotate_image_scene( flat_img, dem, output, options );
    

    return output;
}

void create_flat_test_image( Options const& options, Mat& image, Mat& dem ){

    //create the image
    image = Mat( options.get_build_image_size(), options.get_build_image_type());
    dem   = Mat( options.get_build_image_size(), CV_8UC1 );
    
    double px, py;

    //iterate through the image, creating the structure
    for( int i=0; i<image.cols; i++)
        for( int j=0; j<image.rows; j++){
            
            //measure the position relative to its size
            px = (double)i/image.cols;
            py = (double)j/image.rows;

            //set the positional color to allow us easy use of orientation
            if( image.type() == CV_8UC1 )
                image.at<uchar>(j,i) = std::min( 20.0 + 255*(i+j)/(double)(image.cols+image.rows), 255.0);
            if( image.type() == CV_8UC3 ){    
                uchar val = std::min( 20 + 255*(i+j)/(double)(image.cols+image.rows), 255.0);
                image.at<Vec3b>(j,i) = Vec3b(val, val, val);
            }

            //draw the road outer lines
            if( (px > 0.28 && px < 0.32 )||(px > .68 && px < .72) ){
                
                if( image.type() == CV_8UC1 )
                    image.at<uchar>(j,i) = 0;
                if( image.type() == CV_8UC3 )
                    image.at<Vec3b>(j,i) = Vec3b(0, 0, 0);
            }


            //draw the center lines	
			if ( ( px > .49 && px < .51 ) && (	
                 ( py > .1  && py < .2  ) ||
				 ( py > .3  && py < .4  ) ||
				 ( py > .5  && py < .6  ) ||
				 ( py > .7  && py < .8  ) ||
				 ( py > .9  && py < 1.0 ))){

                if( image.type() == CV_8UC1 )
                    image.at<uchar>(j,i) = 0;
                if( image.type() == CV_8UC3 )
                    image.at<Vec3b>(j,i) = Vec3b(0, 0, 0);
            }

            //draw an ocllusion
			if ( (px > .86 && px < .96) && (py > .6 && py < .69)){
                

                if( image.type() == CV_8UC1 )
                    image.at<uchar>(j,i) = 250;
                if( image.type() == CV_8UC3 )
                    image.at<Vec3b>(j,i) = Vec3b(250, 250, 250);
                
                //set the dem value
                dem.at<uchar>(j,i) = 100;
            }

			//draw an object to be occluded
			if( (px > .87 && px < .95)&&( py > .52 && py < .58)){
                if( image.type() == CV_8UC1 )
                    image.at<uchar>(j,i) = 0;
                if( image.type() == CV_8UC3 )
                    image.at<Vec3b>(j,i) = Vec3b(0, 0, 0);
            }

        }// end of the x,y for loop
}

/** 
  * This function will rotate the image scene for the demo image
  *
  * ASSUMPTIONS!!!!!!!
  * 1.  assume that the center of the image is 0, 0, 0
*/
void rotate_image_scene( Mat const& input_image, Mat const& dem_image, Mat& output_image, Options const& options ){
    
    //set some parameters
    output_image = Mat(input_image.size(), input_image.type());
    output_image = Scalar(0);

    Mat focal_vector = load_vector( 0, 0, options.get_focal_length());

    // Create a Normal Axis and rotate it by the inverse of the rotation quaternion
    Mat initial_normal   = load_vector( 0, 0,  1);
    Mat final_normal     = load_vector( 0, 0, -1);
    Mat initial_position = load_point(  0, 0,  0);

    // note that the axis must remain the same, but the angle must be reversed.
    Mat rotated_normal  = options.RotationM * initial_normal;
    Mat rotated_normalF = options.RotationM * final_normal;


    // We need to compute the final position of the camera
    // - This is done by adding a scaled and rotated normal vector to the ground center point
    //    scale = elevation/rotated normal height
    double scale_factor = options.Position_i.at<double>(2,0)/rotated_normal.at<double>(2,0);
    Mat final_position = (scale_factor*rotated_normal) + initial_position;
    
    // this is the center point of the final view image plane
    Mat plane_centerpoint = final_position + rotated_normalF;
    
    //this will rotate the image according to the required values
    int cnt = 0;
    for( int x=0; x<input_image.cols; x++ ){
        for( int y=0; y<input_image.rows; y++ ){
    
            // Set the initial pixel location in 3D coordinates
            // - assume that the camera position flator a flat image is x, y, 0
            Mat image_point = (options.get_build_img2cam() * load_point( x, y, 0 )) - focal_vector;
            
            // move the image point into the world space
            Mat cam_point = (options.RotationM*image_point) + final_position - load_point(0,0,0);
            
            // Find the position in the world space where the image pixel is pointing to
            Mat P1 = final_position.clone();
            Mat P2 = cam_point.clone();
            Mat N  = initial_normal;
            Mat P3 = initial_position;

            Mat stare_point_world = compute_plane_line_intersection( P1, P2, N, P3);
            
            // relate the position to image coordinates in the original
            Mat final_image_point = options.get_build_cam2img() * stare_point_world;
             
            // convert to cv point to allow passing to as an index
            Point final_image_coord( _round(final_image_point.at<double>(0,0)),
                                     _round(final_image_point.at<double>(1,0)));
            
            if( final_image_coord.x >= 0 && final_image_coord.x < input_image.cols &&
                final_image_coord.y >= 0 && final_image_coord.y < input_image.rows ){
                
                //pull the image
                if( output_image.type() == CV_8UC1 )
                    output_image.at<uchar>(y,x) = input_image.at<uchar>(final_image_coord);
                else if( output_image.type() == CV_8UC3 )    
                    output_image.at<Vec3b>(y,x) = input_image.at<Vec3b>(final_image_coord);
                else
                    throw string("Unsupported pixel type");
            }

            if( cnt++ % 10000 == 0 )
                cout << 'x' << flush;
        }
    }
    
}
