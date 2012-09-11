#include "Perspective.hpp"

#include "../core/Utilities.hpp"
#include "../math/Geometry.hpp"

#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

#define SMALL_NUM 0.000001


Mat generate_perspective_test_image( Options& options ){

    //create an output image
    Mat output, dem, flat_img;
    create_flat_test_image( options, flat_img, dem );
    options.dem = dem;
    
    //rotate the scene of the image accordingly
    rotate_image_scene( flat_img, dem, output, options );
    
    namedWindow("Flat Test Image");
    imshow("Flat Test Image", flat_img);
    imshow("DEM", dem);
    waitKey(0);
    destroyWindow("Flat Test Image");
    destroyWindow("DEM");

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
    Mat earth_normal   = load_vector( 0, 0,  1);
    Mat final_normal     = load_vector( 0, 0, -1);
    Mat image_earth_origin = load_point(  0, 0,  0);

    // note that the axis must remain the same, but the angle must be reversed.
    Mat rotated_normal  = options.RotationM * earth_normal;
    Mat rotated_normalF = options.RotationM * final_normal;


    // We need to compute the final position of the camera
    // - This is done by adding a scaled and rotated normal vector to the ground center point
    //    scale = elevation/rotated normal height
    double scale_factor = options.Position_i.at<double>(2,0)/rotated_normal.at<double>(2,0);
    Mat final_position = (scale_factor*rotated_normal) + image_earth_origin;
   
    Mat PositionFinal = Mat::eye(4,4,CV_64FC1);
    matrix_add_translation( PositionFinal, final_position );

    // this is the center point of the final view image plane
    Mat plane_centerpoint = final_position + rotated_normalF;
    
    //create the zbuffer
    Point3f  maxZPos;
    double   maxZDist;
    
    //generate a ground coordinate list
    cout << "Start Ground Collection" << endl;
    vector<vector<Point3f> > coordinateList = build_ground_coordinate_list( options.dem, 
                                                                            output_image.size(), 
                                                                            options.get_focal_length(), 
                                                                            options.RotationM, 
                                                                            final_position, 
                                                                            options.get_output_img2cam(output_image.size()));   
    
    vector<vector<double> > astack, cstack, estack;
    vector<vector<Mat> >    wstack, ustack;
    
    cout << "Building Buffers" << endl;
    build_buffer_stacks( final_position, coordinateList, astack, cstack, estack, ustack, wstack );


    Mat cam2img = options.get_build_cam2img();
    
    double a, b, d, D;
    Mat u, v;
    
    cout << "Iterating through image" << endl;
    //this will rotate the image according to the required values
    int cnt = 0;
    for( int x=0; x<output_image.cols; x++ ){
        for( int y=0; y<output_image.rows; y++ ){
    
            /** Now we know what we are staring at.  Its time to now find what pixel will be shown here. */
            if( options.doZBuffering() == true ){
                /** DEPTH PROCESSING MODULE */
                //now we need to compute the depth buffer for this image
                /*  
                   Algorithm
                   1. Iterate over every pixel, computing the actual 3D location
                   NOTE: This should be done before this loop!!!!!!!!
                */
                
                //define the line-to-line parameters
                cout << x << ", " << y << endl;
                for( int xx=450; xx<input_image.cols; xx++ ){
                    for( int yy=350; yy<input_image.rows; yy++ ){

                        cout << "Pos: " << x << ", " << y << " -> " << coordinateList[x][y] << endl;
                        cout << "Tst: " << xx<< ", " << yy<< " -> " << coordinateList[xx][yy] << endl;
                        cout << compute2d_line_point_distance( Mat2Point3f(final_position), coordinateList[x][y], coordinateList[xx][yy]) << endl;
                        cin.get();
                        
                        
                        //make sure point intersects line on 2D level
                        if( compute2d_line_point_distance( Mat2Point3f(final_position), coordinateList[x][y], coordinateList[xx][yy]) < 1 ){



                            //now we need to compute the distance between these two lines
                            v = load_point( 0, 0, coordinateList[xx][yy].z);

                            b = ustack[x][y].dot(v);
                            d = ustack[x][y].dot(wstack[xx][yy]);
                            D = astack[x][y] * cstack[xx][yy] - b*b;


                            float    sc, sN, sD = D;      // sc = sN / sD, default sD = D >= 0
                            float    tc, tN, tD = D;      // tc = tN / tD, default tD = D >= 0


                            // compute the line parameters of the two closest points
                            if (D < 0.000001) { // the lines are almost parallel
                                sN = 0.0;        // force using point P0 on segment S1
                                sD = 1.0;        // to prevent possible division by 0.0 later
                                tN = estack[xx][yy];
                                tD = cstack[xx][yy];
                            }
                            else {                // get the closest points on the infinite lines
                                sN = (b*estack[xx][yy] - cstack[xx][yy]*d);
                                tN = (a*estack[xx][yy] - b*d);
                                if (sN < 0.0) {       // sc < 0 => the s=0 edge is visible
                                    sN = 0.0;
                                    tN = estack[xx][yy];
                                    tD = cstack[xx][yy];
                                }
                                else if (sN > sD) {  // sc > 1 => the s=1 edge is visible
                                    sN = sD;
                                    tN = estack[xx][yy] + b;
                                    tD = cstack[xx][yy];
                                }
                            }

                            if (tN < 0.0) {           // tc < 0 => the t=0 edge is visible
                                tN = 0.0;
                                // recompute sc for this edge
                                if (-d < 0.0)
                                    sN = 0.0;
                                else if (-d > a)
                                    sN = sD;
                                else {
                                    sN = -d;
                                    sD = astack[x][y];
                                }
                            }
                            else if (tN > tD) {      // tc > 1 => the t=1 edge is visible
                                tN = tD;
                                // recompute sc for this edge
                                if ((-d + b) < 0.0)
                                    sN = 0;
                                else if ((-d + b) > a)
                                    sN = sD;
                                else {
                                    sN = (-d + b);
                                    sD = astack[x][y];
                                }
                            }
                            // finally do the division to get sc and tc
                            sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
                            tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);

                            // get the difference of the two closest points
                            Mat   dP = wstack[xx][yy] + (sc * ustack[x][y]) - (tc * v);  // = S1(sc) - S2(tc)

                            double dist = norm(dP);   // return the closest distance

                            if( dist < 10 ){
                                cout << "A1: "; print_mat(load_point( coordinateList[x][y]).t());
                                cout << "A2: "; print_mat(final_position.t());
                                cout << "B : "; print_mat(load_point(coordinateList[xx][yy]));
                                cin.get();
                            }
                        }
                    }
                }
            }
            else{

                // relate the position to image coordinates in the original
                Mat final_image_point = cam2img * load_point( coordinateList[x][y]);

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
            }

            if( cnt++ % 10000 == 0 )
                cout << 'x' << flush;
        }
    }

}
