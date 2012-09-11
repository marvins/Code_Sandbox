#include "Geometry.hpp"

#include "../core/Utilities.hpp"

#include <iostream>

using namespace cv;
using namespace std;

Mat compute_plane_line_intersection( Mat const& P1, Mat const& P2, Mat const& N, Mat const& P3){

    
    double u = N.dot(P3-P1)/N.dot(P2-P1);
    
    Mat output = (P1 + u*(P2-P1));

    return output;
}

Mat pixel2world_coordinates( const Point3f& pnt, const Mat& RotationMatrix, const Mat& camera_origin, const Mat& image2camera ){
    
    //create translation matrix
    Mat CameraTranslation = Mat::eye(4, 4, CV_64FC1);

    //add translation coefficients
    CameraTranslation.at<double>(0,3) = camera_origin.at<double>(0,0);
    CameraTranslation.at<double>(1,3) = camera_origin.at<double>(1,0);
    CameraTranslation.at<double>(2,3) = camera_origin.at<double>(2,0);
    

    /* convert pixel 2 world
      1.  Convert from Pixel To Camera
      2.  Rotate Image Coordinate System
      3.  Add Origin to convert to world
    */
    Mat camera_coord = CameraTranslation * RotationMatrix * image2camera * load_point(pnt);

    return camera_coord;
}

void matrix_add_translation( cv::Mat& matrix, cv::Mat& translation ){

    matrix.at<double>(0,3) += translation.at<double>(0,0);
    matrix.at<double>(1,3) += translation.at<double>(1,0);
    matrix.at<double>(2,3) += translation.at<double>(2,0);

}

vector<vector<cv::Point3f> > build_ground_coordinate_list( cv::Mat const& dem, cv::Size img_size, const double& f, const Mat& RotationM, const Mat& camera_origin, const Mat& img2cam ){ 
   
    //create translation matrix
    Mat CameraTranslation = Mat::eye(4, 4, CV_64FC1);

    //add translation coefficients
    CameraTranslation.at<double>(0,3) = camera_origin.at<double>(0,0);
    CameraTranslation.at<double>(1,3) = camera_origin.at<double>(1,0);
    CameraTranslation.at<double>(2,3) = camera_origin.at<double>(2,0);
    
    //create multiplication matrix
    Mat multiplier = CameraTranslation * RotationM * img2cam;
    
    //create output container
    vector<vector<Point3f> > output_coordinate_list( img_size.width);

    Mat earth_normal = load_vector( 0, 0, 1);

    double top = earth_normal.dot(load_point(0,0,0)-camera_origin);
    int hx = img_size.width/2;
    int hy = img_size.height/2;

    //iterate throught the output image
    for( int i=0; i<img_size.width; i++){
        
        //resize the column
        output_coordinate_list[i].resize(img_size.height);
    
        //iterate through rows
        for( int j=0; j<img_size.height; j++ ){
            Mat b0 = multiplier * load_point( Point3f( i, j, -f));
            Mat Line = b0 - camera_origin;
            Mat b1 = camera_origin + (top/earth_normal.dot(Line))*(Line);
    
            
            output_coordinate_list[i][j].x = b1.at<double>(0,0);
            output_coordinate_list[i][j].y = b1.at<double>(1,0);
            output_coordinate_list[i][j].z = dem.at<uchar>(_round(b1.at<double>(1,0)) + hx, _round(b1.at<double>(0,0)+hy));
        }
    }
    return output_coordinate_list;
}

void build_buffer_stacks( Mat const& final_position, vector<vector<Point3f> > const& coordList, 
                                                     vector<vector<double> >& astack, 
                                                     vector<vector<double> >& cstack, 
                                                     vector<vector<double> >& estack, 
                                                     vector<vector<Mat> >& ustack,
                                                     vector<vector<Mat> >& wstack ){

    Mat u, v;
    
    //initialize the cstack
    astack.resize(coordList.size());
    cstack.resize(coordList.size());
    estack.resize(coordList.size());
    ustack.resize(coordList.size());
    wstack.resize(coordList.size());

    //iterate through every coordinate in the coordinate list
    for( int x=0; x<coordList.size(); x++ ){
        
        astack[x].resize(coordList[x].size());
        cstack[x].resize(coordList[x].size());
        estack[x].resize(coordList[x].size());
        ustack[x].resize(coordList[x].size());
        wstack[x].resize(coordList[x].size());
        for( int y=0; y<coordList[x].size(); y++ ){
            
            u = load_point( coordList[x][y] ) - final_position;
            v = load_point( 0, 0, coordList[x][y].z);
            

            ustack[x][y] = u.clone();
            wstack[x][y] = final_position - load_point( coordList[x][y].x, coordList[x][y].y, 0);
            
            astack[x][y] = u.dot(u);
            cstack[x][y] = v.dot(v);
            estack[x][y] = v.dot(wstack[x][y]);
         }
    }
}

double compute2d_line_point_distance( Point3f const& l1, Point3f const& l2, Point3f const& pt ){
    
    double u = ((pt.x - l1.x)*(l2.x - l1.x) + (pt.y - l1.y)*(l2.y - l1.y))/(pow(norm(l2-l1),2));

    Point3f res = l1 + u*(l2-l1);
    
    cout << "Closest: " << res << endl;
    return norm(res - pt);

}
