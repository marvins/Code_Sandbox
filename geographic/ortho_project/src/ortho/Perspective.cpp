#include "Perspective.hpp"

#include "../core/ProgressBar.hpp"
#include "../core/dem.hpp"
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
    
    
    namedWindow("Flat Test Image");
    imshow("Flat Test Image", flat_img);
    imshow("DEM", dem);
    
    imwrite("data/flat_image.jpg", flat_img);
    imwrite("data/dem.png", dem);
    
    waitKey(0);
    destroyWindow("Flat Test Image");
    destroyWindow("DEM");
    
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
            if (((px >= .858 && px <= .962) && (py >= .598 && py <= .600 ))||
                ((px >= .858 && px <= .962) && (py >= .690 && py <= .692 ))||
                ((px >= .858 && px <= .860) && (py >= .598 && py <= .692 ))||
                ((px >= .960 && px <= .962) && (py >= .598 && py <= .692 ))){

                if( image.type() == CV_8UC1 )
                    image.at<uchar>(j,i) = 20;
                if( image.type() == CV_8UC3 )
                    image.at<Vec3b>(j,i) = Vec3b(20, 20, 20);
                
                //set the dem value
                dem.at<uchar>(j,i) = 100;
            }

			if ( (px > .86  && px <  .96) && (py > .6   && py <  .69)){

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
            
            //draw an ocllusion
            if (((px >= .097 && px <= .202) && (py >= .097 && py <  .100 ))||
                ((px >= .097 && px <= .202) && (py >= .200 && py <= .202 ))||
                ((px >= .097 && px <  .100) && (py >= .097 && py <= .202 ))||
                ((px >= .200 && px <= .202) && (py >= .097 && py <= .202 ))){

                if( image.type() == CV_8UC1 )
                    image.at<uchar>(j,i) = 20;
                if( image.type() == CV_8UC3 )
                    image.at<Vec3b>(j,i) = Vec3b(20, 20, 20);
                
                //set the dem value
                dem.at<uchar>(j,i) = 100;
            }

			if ( (px >= .1  && px <  .2) && (py >= .10  && py <  .2)){

                if( image.type() == CV_8UC1 )
                    image.at<uchar>(j,i) = 250;
                if( image.type() == CV_8UC3 )
                    image.at<Vec3b>(j,i) = Vec3b(250, 250, 250);
                
                //set the dem value
                dem.at<uchar>(j,i) = 100;
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
    
    
    //create a progress bar object and make sure we have permission to show it
    ProgressBar progressBar(0, input_image.cols*input_image.rows, 50);
    bool show_progress_bar = false;
    if( ( options.logger.get_console_run_state() & LOG_INFO ) == LOG_INFO )
        show_progress_bar = true;
    
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
    Point3f  maxZPnt;
    double   maxZDist;
    int      maxType;
    bool     intersection;

    //generate a ground coordinate list
    vector<vector<Point3f> > outCoordinateList = build_ground_coordinate_list( options.dem, 
                                                                            output_image.size(), 
                                                                            options.get_focal_length(), 
                                                                            options.RotationM, 
                                                                            final_position, 
                                                                            options.get_output_img2cam(output_image.size()));   
    
    vector<vector<Point3f> > inCoordinateList( input_image.cols);
    for( int i=0; i<input_image.cols; i++){
        inCoordinateList[i].resize(input_image.rows);
        for( int j=0; j<input_image.rows; j++)
            inCoordinateList[i][j] = Point3f( i - (input_image.cols/2), j - (input_image.rows/2), options.dem.at<uchar>(j,i));
    }
    
    vector<vector<double> > astack, cstack, estack;
    vector<vector<Mat> >    wstack, ustack;
    
    Mat cam2img = options.get_output_cam2img( options.get_build_image_size());
    
    //we now have a region to search, we should next prune this region to only include 
    //items actually in the image
    Point2f imgMin( std::min( outCoordinateList[            0      ][            0      ].x, 
                    std::min( outCoordinateList[            0      ][output_image.rows-1].x, 
                    std::min( outCoordinateList[output_image.cols-1][            0      ].x, 
                                                outCoordinateList[output_image.cols-1][output_image.rows-1].x))),
                    std::min( outCoordinateList[            0      ][            0      ].y, 
                    std::min( outCoordinateList[            0      ][output_image.rows-1].y, 
                    std::min( outCoordinateList[output_image.cols-1][            0      ].y, 
                              outCoordinateList[output_image.cols-1][output_image.rows-1].y))));

    Point2f imgMax( std::max( outCoordinateList[            0      ][            0      ].x, 
                    std::max( outCoordinateList[            0      ][output_image.rows-1].x, 
                    std::max( outCoordinateList[output_image.cols-1][            0      ].x, 
                              outCoordinateList[output_image.cols-1][output_image.rows-1].x))), 
                    std::max( outCoordinateList[            0      ][            0      ].y, 
                    std::max( outCoordinateList[            0      ][output_image.rows-1].y, 
                    std::max( outCoordinateList[output_image.cols-1][            0      ].y, 
                              outCoordinateList[output_image.cols-1][output_image.rows-1].y))));


    Point2f demMin(-500,-500);
    Point2f demMax( 500, 500);
    double maxElevation = query_max_elevation( imgMin, imgMax, Point2f(0,0), options );

    //this will rotate the image according to the required values
    int cnt = 0;
    for( int x=0; x<output_image.cols; x++ ){
        for( int y=0; y<output_image.rows; y++ ){
            
            /** Now we know what we are staring at.  Its time to now find what pixel will be shown here. */
            if( options.doZBuffering() == true ){
                /** DEPTH PROCESSING MODULE */
                
                Mat stare_point = load_point( outCoordinateList[x][y].x, outCoordinateList[x][y].y, 0);
                Point2f starePoint( outCoordinateList[x][y].x, outCoordinateList[x][y].y);
    
                Mat maxDistance = compute_plane_line_intersection( final_position, stare_point, earth_normal, load_point(0,0,maxElevation));
                double distRadius = norm( Point2f( maxDistance.at<double>(0,0), maxDistance.at<double>(1,0)) - starePoint);
                
                //compute the bounding box from the current point to the camera origin
                Point2f minBound( std::min( starePoint.x-distRadius,  std::min(final_position.at<double>(0,0), (double)outCoordinateList[x][y].x )),
                                  std::min( starePoint.y-distRadius,  std::min(final_position.at<double>(1,0), (double)outCoordinateList[x][y].y )));

                Point2f maxBound( std::min( starePoint.x+distRadius,  std::max(final_position.at<double>(0,0), (double)outCoordinateList[x][y].x )),
                                  std::min( starePoint.y+distRadius,  std::max(final_position.at<double>(1,0), (double)outCoordinateList[x][y].y )));
                
                
                
                //now that we know the geographic extent of this search, we need to relate this to 
                //  pixels on the original image
                Point imgPixMin( std::max(_round(minBound.x)+(input_image.cols/2),            0    ), std::max(_round(minBound.y)+(input_image.rows/2), 0));
                Point imgPixMax( std::min(_round(maxBound.x)+(input_image.cols/2), input_image.cols-1), std::min(_round(maxBound.y)+(input_image.rows/2), input_image.rows-1));

                //set the default positions
                maxZDist = norm( Mat2Point3f(final_position)-outCoordinateList[x][y]);
                maxZPnt  = outCoordinateList[x][y];
                maxType  = 1;
                intersection = false;
                
                /** Start searching through the original image */
                for( int xx=imgPixMin.x; xx<=imgPixMax.x; xx++ )
                    for( int yy=imgPixMin.y; yy<=imgPixMax.y; yy++ ){

                        //make sure the physical locations are not the same
                        if( norm( inCoordinateList[xx][yy]-outCoordinateList[x][y]) > 0.1 ){

                            //make sure that the test point has a higher elevation than the current point 
                            if( ( inCoordinateList[xx][yy].z ) > 0.0001 ){
                                
                                //make sure point intersects line on 2D level
                                if( compute2d_line_point_distance( Mat2Point3f(final_position), outCoordinateList[x][y], inCoordinateList[xx][yy]) < 1.42 ){

                                    //check if the lines intersect
                                    double dist;
                                    int result = compute3d_line_line_intersection( Mat2Point3f(final_position), Point3f(outCoordinateList[x][y].x, outCoordinateList[x][y].y, 0), 
                                            inCoordinateList[xx][yy], Point3f( inCoordinateList[xx][yy].x, inCoordinateList[xx][yy].y, 0), 
                                            dist, 1 );
                                    
                                    /** If there is an intersection, then update the buffer for that point */
                                    if( result > 0 && dist < maxZDist ){
                                        maxZDist = dist;
                                        maxZPnt  = inCoordinateList[xx][yy];
                                        maxType = result;
                                        intersection = true;

                                    }
                                }}}
                    } // end of xx and yy loops

                //draw the point

                //first find the actual location of the pixel
                Point pix( _round(maxZPnt.x) + (input_image.cols/2), _round(maxZPnt.y) + (input_image.rows/2) );

                //pull the image
                if( pix.x >= 0 && pix.y >= 0 && pix.x <= input_image.cols && pix.y <= input_image.rows ){

                    if( output_image.type() == CV_8UC1 ){
                        //if( intersection == false )
                        //    output_image.at<uchar>(y,x) = input_image.at<uchar>(pix);
                        //else if( maxType == 1 )
                            output_image.at<uchar>(y,x) = input_image.at<uchar>(pix);
                        //else if( maxType == 2 )    
                        //    output_image.at<uchar>(y,x) = 255;
                    }
                    else if( output_image.type() == CV_8UC3 ){   
                        if( intersection == false )
                            output_image.at<Vec3b>(y,x) = input_image.at<Vec3b>(pix);
                        else if( maxType == 1 )
                            output_image.at<Vec3b>(y,x) = input_image.at<Vec3b>(pix);
                        else if( maxType == 2 )    
                            output_image.at<Vec3b>(y,x) = Vec3b(255,255,255);
                    }
                    else
                        throw string("Unsupported pixel type");

                }
            }
            else{

                // relate the position to image coordinates in the original
                Mat final_image_point = cam2img * load_point( outCoordinateList[x][y]);

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
            
            //update the progress bar
            if( show_progress_bar )
                progressBar.update( cnt );
            cnt++;
        }
        //print the progress bar to console
        if( show_progress_bar )
            cout << progressBar.toString() << '\r' << flush;

    }
    if( show_progress_bar )
        cout << endl;
}
