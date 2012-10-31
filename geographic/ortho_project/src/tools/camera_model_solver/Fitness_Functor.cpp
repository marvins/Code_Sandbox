#include "Fitness_Functor.hpp"

#include "../../math/Quaternion.hpp"
#include "../../math/Vector.hpp"
#include "../../math/Geometry.hpp"
#include "../../core/Utilities.hpp"

using namespace cv;
using namespace std;


double  pixel2world( Point const& pix, Point3f const& expected, Size const& image_size, Variables const& vars ){


    // convert each variable into a usable item
    Mat camera_position = load_point( vars.camera_position );
    Size_<double> image_plane_size( vars.image_plane.x, vars.image_plane.y );
    double focal_length = vars.focal_length;
    Mat rotation_axis = Quaternion( vars.rotation_angle, vec( vars.rotation_axis )).get_rotation_matrix();

    // set some given values
    Mat pixel_coord     = load_point( pix.x, pix.y, 0);
    Mat earth_normal    = load_vector(   0 ,    0 , 1);
    Mat expected_coord  = load_point( expected.x, expected.y, expected.z);


    ////////////////////////////////////////////////////////////////////////
    //           convert the pixel to camera plane coordinates            //
    ////////////////////////////////////////////////////////////////////////
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
    Mat ground_coord = compute_plane_line_intersection( camera_position, plane_coord, earth_normal, expected_coord );
    
    Point3f result( ground_coord.at<double>(0,0),
                    ground_coord.at<double>(1,0),
                    ground_coord.at<double>(2,0));
    
    return norm( result - expected );

}


int bits2int( vector<bool>const& genome, const int& start, const int& length ){

    int result = 0;
    for( int i=start; i<start+length; i++ ){
        result += ((int)genome[i])*pow(2,start+length - i-1);
    }
    return result;
}


double convert_bits2double( vector<bool>const& genome, const int& start, const int& length, 
                            const double& step, const double& _min, const double& _max ){

    // convert the bits to an integer
    double value = bits2int( genome, start, length );
    
    // subtract the min and divide by the range
    value = std::max( std::min( (value)*step + _min, _max ), _min );

    return value;
}


/**
 * This is the class which holds the required parameters for our class
*/
Variables::Variables( vector<bool> const& genome, Point3f const& earth_point ){
    
    rotation_axis.resize(3);
    total_length = 0;
    int start=0;
    int length=0;
    double step;

    /** 
      The following is our bit map which we will follow
     */

    // focal_length: [min=-1, max=10, step=.01] , total length=log2(11*100)=11
    start  = 0;
    length = 11;
    step   = 0.01;
    focal_length  = convert_bits2double( genome, start, length, step, -1, 10 );

    // image plane.x: [min=0, max=2, step=0.01] , total length=2*100=log2(200)=8
    start  += length;
    length =  8;
    step   = 0.01;
    image_plane.x = convert_bits2double( genome, start, length, step, 0, 2);
    
    // image plane.y: [min=0, max=2, step=0.01] , total length=2*100=log2(200)=8
    start += length;
    length = 8;
    step   = 0.01;
    image_plane.y = convert_bits2double( genome, start, length, step, 0, 2);
    
    // rotation angle: [min=0, max=359.99, step=0.01], total length=log2(360*100-1)=16
    start += length;
    length = 16;
    step   = 0.01;
    rotation_angle = convert_bits2double( genome, start, length, step, 0, 359.99);
    
    // rotation axis x: [min=-1, max=1, step=0.01], total length=log2(1*100)=16
    // rotation axis y: [min=-1, max=1, step=0.01], total length=log2(1*100)=16
    // rotation axis z: [min=-1, max=1, step=0.01], total length=log2(1*100)=16
    start += length;
    length = 8;
    step   = 0.01;
    rotation_axis[0] = convert_bits2double( genome, start, length, step, -1, 1);
    start += length;
    rotation_axis[1] = convert_bits2double( genome, start, length, step, -1, 1);
    start += length;
    rotation_axis[2] = convert_bits2double( genome, start, length, step, -1, 1);
    
    // camera_position.x: [min=-10000,max=10000, step=1], total length=log2(20000)=15
    start += length;
    length=15;
    step=1;
    camera_position.x = convert_bits2double( genome, start, length, step, -10000, 10000);
    
    start += length;
    camera_position.y = convert_bits2double( genome, start, length, step, -10000, 10000);
    
    start += length;
    camera_position.z = convert_bits2double( genome, start, length, step, -10000, 10000);

    camera_position += earth_point;

    total_length = start + length;
    
}

void Variables::print( ostream& ostr )const{

    ostr << "Variable Container" << endl;
    ostr << "- Focal Length: " << focal_length   << endl;
    ostr << "- Image Plane : " << image_plane    << endl;
    ostr << "- Rotation Ang: " << rotation_angle << endl;
    ostr << "- Rotation_x  : " << rotation_axis[0] << endl;
    ostr << "- Rotation_y  : " << rotation_axis[1] << endl;
    ostr << "- Rotation_z  : " << rotation_axis[2] << endl;
    ostr << "- Cam Position: " << camera_position << endl;

}


bool Fitness_Func_Sort::operator()( pair<vector<bool>,double>const& a, pair<vector<bool>,double>const& b )const{

    return (a.second > b.second);

}



/**
 * Parameterized Constructor
*/
Fitness_Functor::Fitness_Functor( const vector<Point>& image_pnts, const vector<Point3f>& earth_pnts, const Size& img_size ): 
                                  image_points(image_pnts), earth_points(earth_pnts), image_size(img_size) {


}


/**
 * This is the main fitness function
*/
double Fitness_Functor::operator()( std::vector<bool>const& genome )const{

    //convert the boolean string into a useable class
    Variables vars( genome, earth_points[0] );
    
    // iterate through each pair of points, computing the sum of difference
    double sum = 0;
    for( size_t i=0; i<image_points.size(); i++ )
        sum += pixel2world( image_points[i], earth_points[i], image_size, vars );
    

    return -sum;
}

void Fitness_Functor::print_vars( ostream& ostr, vector<bool>const& str )const{

    Variables( str, earth_points[0] ).print( ostr );
}

