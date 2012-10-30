#include "Geometry.hpp"

#include "../core/Utilities.hpp"

#include <iostream>

#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

/** 
 * Computes the intersection point between a plane and a line
*/
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

void matrix_add_translation( cv::Mat& matrix, cv::Mat const& translation ){

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
    int xx, yy;

    //iterate throught the output image
    for( int i=0; i<img_size.width; i++){
        
        //resize the column
        output_coordinate_list[i].resize(img_size.height);
    
        //iterate through rows
        for( int j=0; j<img_size.height; j++ ){
            
            Mat b0 = multiplier * load_point( Point3f( i, j, -1));
            
            Mat Line = b0 - camera_origin;
            Mat b1 = camera_origin + (top/earth_normal.dot(Line))*(Line);
            
            output_coordinate_list[i][j].x = b1.at<double>(0,0);
            output_coordinate_list[i][j].y = b1.at<double>(1,0);
            
            xx = _round(b1.at<double>(1,0)) + hx;
            yy = _round(b1.at<double>(0,0)) + hy;

            if( xx >= 0 && xx < dem.cols && yy >= 0 && yy < dem.rows )
                output_coordinate_list[i][j].z = dem.at<uchar>(_round(b1.at<double>(1,0)) + hx, _round(b1.at<double>(0,0))+hy);
            else        
                output_coordinate_list[i][j].z = 0;

        }
    }
    
    return output_coordinate_list;
}


double compute2d_line_point_distance( Point3f const& l1, Point3f const& l2, Point3f const& pt ){
    
    return fabs( (l2.x - l1.x)*(l1.y - pt.y) - (l1.x - pt.x)*(l2.y - l1.y))/(sqrt( (l2.x-l1.x)*(l2.x-l1.x)+(l2.y-l1.y)*(l2.y-l1.y)));
    
}

double compute3d_line_point_distance( Point3f const& l1, Point3f const& l2, Point3f const& pt ){

    Point3f v = l2 - l1;
    Point3f w = pt - l1;

    double c1 = w.dot(v);
    if( c1 <= 0 )  return norm(pt - l1);

    double c2 = v.dot(v);
    if( c2 <= c1 ) return norm(pt - l2);

    double b = c1/c2;
    Point3f pb = l1 + b*v;
    return norm(pt-pb);

}

#define SMALL_NUM 0.000001

int compute3d_line_line_intersection( Point3f const& a1, Point3f const& u, 
                                      Point3f const& b1, Point3f const& v, 
                                      const double& a, double& distance, double const& threshold ){

    //compute some baseline variables
    Point3f w = a1 - b1;

    double b = u.dot(v);
    double c = v.dot(v);
    double d = u.dot(w);
    double e = v.dot(w);

    double D = a*c - b*b;
    double  sc, sN, sD = D;      // sc = sN / sD, default sD = D >= 0
    double  tc, tN, tD = D;      // tc = tN / tD, default tD = D >= 0


    // compute the line parameters of the two closest points
    if (D < 0.000001) { // the lines are almost parallel
        sN = 0.0;        // force using point P0 on segment S1
        sD = 1.0;        // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) {       // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1 => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
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
            sD = a;
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
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);
    
    // get the difference of the two closest points
    Point3f  dP = w + (sc * u) - (tc * v);  // = S1(sc) - S2(tc)
    Point3f  it = b1 + tc*v;
    distance = norm(a1 - it);

    double dd = norm(dP);   // return the closest distance
    double tpd = norm(b1 - it);
    
    //we have a match
    if( dd < threshold ){
        
        //check if the intersection is near on of the points on line b
        if( tpd < .2 )
            return 1;
        else
            return 2;
    }
    else{
        return 0;
    }
}



int compute3d_line_line_intersection( Point3f const a1, Point3f const a2, Point3f const& b1, Point3f const& b2, double& distance, double const& threshold ){


    //compute some baseline variables
    Point3f u = a2 - a1;
    Point3f v = b2 - b1;
    Point3f w = a1 - b1;

    double a = u.dot(u);
    double b = u.dot(v);
    double c = v.dot(v);
    double d = u.dot(w);
    double e = v.dot(w);

    double D = a*c - b*b;
    double  sc, sN, sD = D;      // sc = sN / sD, default sD = D >= 0
    double  tc, tN, tD = D;      // tc = tN / tD, default tD = D >= 0


    // compute the line parameters of the two closest points
    if (D < 0.000001) { // the lines are almost parallel
        sN = 0.0;        // force using point P0 on segment S1
        sD = 1.0;        // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else {                // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) {       // sc < 0 => the s=0 edge is visible
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) {  // sc > 1 => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
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
            sD = a;
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
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (abs(sN) < SMALL_NUM ? 0.0 : sN / sD);
    tc = (abs(tN) < SMALL_NUM ? 0.0 : tN / tD);
    
    // get the difference of the two closest points
    Point3f  dP = w + (sc * u) - (tc * v);  // = S1(sc) - S2(tc)
    Point3f  it = b1 + tc*v;
    distance = norm(a1 - it);

    double dd = norm(dP);   // return the closest distance
    double tpd = norm(b1 - it);
    
    //we have a match
    if( dd < threshold ){
        
        //check if the intersection is near on of the points on line b
        if( tpd < .2 )
            return 1;
        else
            return 2;
    }
    else{
        return 0;
    }
}

double Angle2D( Point2f const& c, Point2f const& p1, Point2f const& p2 ){
    
    Point2f ca = p1 - c;
    Point2f ba = p2 - c;
    
    return acos(ca.dot(ba)/(norm(ca)*norm(ba)));
}

bool pointInConvexPolygon( std::vector<cv::Point2f>const& lst, cv::Point2f const& testPoint ){
    
    //make sure point is not one of the listed points
    for( size_t i=0; i<lst.size(); i++)
        if( norm(testPoint-lst[i]) < 0.1 )return true;

    double angSum = 0;
    for( size_t i=0; i<lst.size()-1; i++){
        angSum += Angle2D( testPoint, lst[i], lst[i+1])*180.0/M_PI;
    }
    angSum += Angle2D( testPoint, lst.back(), lst[0])*180.0/M_PI;

    if( fabs( angSum - 360 ) < 1 ) return true;
    else                           return false;
}

double compute3d_point_point_distance( Mat const& pointA, Mat const& pointB ){

    //make sure the dimensions are the same
    if( pointA.cols != pointB.cols || pointA.rows != pointA.cols )
        return -1;
    
    double dist = 0;

    //search over the matricies
    for( int y=0; y<3; y++ )
        dist += (pointA.at<double>(y,0) - pointB.at<double>(y,0)) * (pointA.at<double>(y,0) - pointB.at<double>(y,0));

    return sqrt(dist);
}

Mat compute_vector_norm( Mat const& mat ){

    Mat output( mat.rows, mat.cols, CV_64FC1);

    //get the vector magnitude
    double mag = 0;
    for( int x=0; x<mat.cols; x++ )
    for( int y=0; y<mat.rows; y++ )
        mag += pow( mat.at<double>(y,x), 2);
    mag = sqrt(mag);
    
    //normalize
    for( int x=0; x<mat.cols; x++ )
    for( int y=0; y<mat.rows; y++ )
        output.at<double>(y,x) = mat.at<double>(y,x) / mag;

    return output;

}

/**  
 * Compute the bounding box for the 4 corners
*/
cv::Rect_<double> compute_ground_bbox( cv::Mat const& pntA, cv::Mat const& pntB, cv::Mat const& pntC, cv::Mat const& pntD ){
    
    // get the minimum points
    double minX = std::min( pntA.at<double>(0,0), std::min( pntB.at<double>(0,0), std::min( pntC.at<double>(0,0), pntD.at<double>(0,0))));
    double minY = std::min( pntA.at<double>(1,0), std::min( pntB.at<double>(1,0), std::min( pntC.at<double>(1,0), pntD.at<double>(1,0))));

    // get the maximum points
    double maxX = std::max( pntA.at<double>(0,0), std::max( pntB.at<double>(0,0), std::max( pntC.at<double>(0,0), pntD.at<double>(0,0))));
    double maxY = std::max( pntA.at<double>(1,0), std::max( pntB.at<double>(1,0), std::max( pntC.at<double>(1,0), pntD.at<double>(1,0))));

    // create the bounding box
    return Rect_<double>( minX, minY, maxX - minX, maxY - minY );
}


cv::Mat load_world_point(  Point const& pix, Size const& img_size, Rect_<double> const& ground_bbox, const GEO::DEM& dem ){
    
    //compute the scaled position (Remember that the y axis is flipped )
    double ranX = (double) pix.x / img_size.width;
    double ranY = (double) ( img_size.height - pix.y ) / img_size.height; 
    
    Point2f pnt( ground_bbox.width * ranX + ground_bbox.tl().x, 
                 ground_bbox.height* ranY + ground_bbox.tl().y);

    //apply to geographic range (Remember that the y axis is flipped )
    return load_point( pnt.x, pnt.y, dem.query_elevation( pnt ));

}

