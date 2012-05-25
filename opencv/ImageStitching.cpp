#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

const double PANO_REF_YAW   = 0; // reference image's yaw
const double PANO_REF_PITCH = 0; // reference image's pitch
const double PANO_REF_ROLL  = 0; // reference image's roll

using namespace cv;
using namespace std;

typedef pair<Point2f, Point2f> pntMatch;

class ImageModel{

    public:
        Mat cimg;
        Mat gimg;

        double focal;            // focal length of the camera in pixels
        double cx, cy;           // optical centre of the camera in pixels
        double yaw, pitch, roll; // offset in the panoramic image

        void print(){
            cout << "focal len: " << focal << ", cx: " << cx << ", cy: " << cy << ", yaw: " << yaw << ", pitch: " << pitch << ", roll: " << roll << endl;
        }

};

vector<Mat> convertBGR2Gray( vector<Mat>const& images );
vector<Mat> load_images( string const& filename );
vector<string>  read_image_list( string const& filename );
void show_image( Mat const& img );
void FindMatches( Mat const& img1, Mat const& img2, vector<pntMatch>& matches );
void convertMatch2Mat(const vector <pntMatch> &matches, Mat &src, Mat &dst);
void FindBestRotation(const vector <pntMatch>& matches, const ImageModel& pano1, const ImageModel&pano2 , double& yaw, double& pitch, double& roll);
void Pixel2Angles(double x, double y, double focal, double cx, double cy, double &yaw, double &pitch);
bool Angles2Pixel(double yaw, double pitch, double roll, double focal, double cx, double cy, double &x, double &y);

Mat show_matches( Mat const& imgA, Mat const& imgB, vector<pntMatch>const& matches );

int main( int argc, char* argv[] ){

    namedWindow("WINDOW");

    //create list of images
    vector<Mat> images      = load_images( argv[1] );
    vector<Mat> gray_images = convertBGR2Gray( images );
    int num_matches = 0;

    //build image metrics
    vector< ImageModel >  imageModel( gray_images.size() );
    for( size_t i=0; i<gray_images.size(); i++ ){
        imageModel[i].cimg  = images[i].clone();
        imageModel[i].gimg  = gray_images[i].clone();
        imageModel[i].focal = 2917.0 * 0.25;
        imageModel[i].cx    = imageModel[i].gimg.cols/2;
        imageModel[i].cy    = imageModel[i].gimg.rows/2;
    }

    //find correspondencies between images
    vector<pntMatch> matches, refined_matches;
    vector < vector<pntMatch> > all_matches(imageModel.size() - 1);

    for( size_t i=1; i< imageModel.size(); i++){

        //find matching keypoints between images
        FindMatches( imageModel[i-1].gimg, imageModel[i].gimg, matches );
        
        //convert match vector to Mats
        Mat pnts1, pnts2;
        convertMatch2Mat( matches, pnts1, pnts2 );

        //compute homography matrix
        vector<uchar> mask;
        Mat homography = findHomography( pnts2, pnts1, mask, CV_RANSAC, 2.0 );

        //filter out bad matches
        for( size_t j=0; j<mask.size(); j++)
            if( mask[j] )
                refined_matches.push_back( matches[j] );
        
        cout << "Show Matches" << endl;
        show_matches( imageModel[i-1].cimg, imageModel[i].cimg, refined_matches );
        cout << "End of Show Matches" << endl;

        
        //calculate yaw, pitch, and roll for the image
        double yaw, pitch, roll;
        FindBestRotation( refined_matches, imageModel[i-1], imageModel[i], yaw, pitch, roll );
        
        // These are our initial guesses. We'll covert them to absolute angles and refine them later
        imageModel[i].yaw   = -yaw;
        imageModel[i].pitch = -pitch;
        imageModel[i].roll  = -roll;

        all_matches[i-1] = refined_matches;

        num_matches += refined_matches.size();

    }
   
    // Convert our esimtated yaw, pitch, roll to absolute position in panoramic space
    // NOTE: assumes the images are taken in sequence
    // The reference image
    imageModel[0].yaw   = PANO_REF_YAW;
    imageModel[0].pitch = PANO_REF_PITCH;
    imageModel[0].roll  = PANO_REF_ROLL;

    for(unsigned int i=1; i < imageModel.size(); i++) {
        imageModel[i].yaw   += imageModel[i-1].yaw;
        imageModel[i].pitch += imageModel[i-1].pitch;
        imageModel[i].roll  += imageModel[i-1].roll;
    }

    /*
    // Now do the optimisation/refinement
    vector <double> params;
    {
        MakeParams(pano_images, &params);

        lm_control_struct control = lm_control_double;
        lm_status_struct status;

        control.printflags = 0; // print out the stuff from the optimisation process

        // Comment this out if you want to see the results without optimisation. Good for comparison.
        lmmin(params.size(), &params[0], num_matches, &all_matches, ObjectiveFunction, &control, &status, lm_printout_std);

        for(unsigned int i=0; i < pano_images.size(); i++) {
            pano_images[i].focal = params[i*4];
            pano_images[i].yaw   = params[i*4 + 1];
            pano_images[i].pitch = params[i*4 + 2];
            pano_images[i].roll  = params[i*4 + 3];
        }
    }*/

    // Okay, now we'll create our masterpeice!
    //int tmp_width; // the width of the big temporary panoramic image
    //BuildPanoramic(pano_images, 1.0 /* scale output */, &tmp_width); // Scaling is a good idea if you got big images!

    /*
    // Calculate the reprojection erorr in pixels
    {
    vector <double> fvec;
    fvec.resize(num_matches);

    ObjectiveFunction(&params[0], num_matches, &all_matches, &fvec[0], NULL);

    double mse = std::accumulate(fvec.begin(), fvec.end(), 0.0) / fvec.size();
    double rmse = sqrt(mse);
    printf("Root mean squared error (pixels): %g\n", rmse);
    }
     */

    return 0;

}

/**
 * Convert an array of images from color to grayscale
 *
 * @brief Convert an array of images from color to grayscale
 *
 * @param[in] images STL Vector of OpenCV Mat images in BGR Color
 * @return    STL Vector of OpenCV Mat images in Grayscale
 */
vector<Mat> convertBGR2Gray( vector<Mat>const& images ){

    Mat img;
    vector<Mat> img_out;

    for( size_t i=0; i<images.size(); i++){
        cvtColor( images[i], img, CV_BGR2GRAY);  
        img_out.push_back( img.clone() );
    }

    return img_out;
}

vector<Mat> load_images( string const& filename ){

    vector<string> filenames = read_image_list( filename );
    vector<Mat> images;

    for( size_t i=0; i<filenames.size(); i++){
        images.push_back( imread(filenames[i]).clone() );
    }

    return images;
}


vector<string>  read_image_list( string const& filename ){

    vector<string> files;
    string str;

    ifstream fin;
    fin.open( filename.c_str() );

    fin >> str;

    while( !fin.eof() ){
        files.push_back(str);
        fin >> str;
    }

    fin.close();

    return files;
}

void show_image( Mat const& img ){

    imshow("WINDOW", img);
    waitKey(0);

}

void FindMatches( Mat const& img1, Mat const& img2, vector<pntMatch>& match_list ){

    //compute keypoints
    SurfFeatureDetector detector(400);
    vector<KeyPoint> keypoints1, keypoints2;
    detector.detect( img1, keypoints1 );
    detector.detect( img2, keypoints2 );

    //compute descriptors
    SurfDescriptorExtractor extractor;
    Mat descriptors1, descriptors2;
    extractor.compute( img1, keypoints1, descriptors1 );
    extractor.compute( img2, keypoints2, descriptors2 );

    //find matches
    vector<DMatch> matches;
    BFMatcher matcher( NORM_L2 );
    matcher.match(descriptors1, descriptors2, matches);

    match_list.clear();
    for( int i = 0; i < (int)matches.size(); i++) {
        int i1 = matches[i].queryIdx;
        int i2 = matches[i].trainIdx;

        const KeyPoint kp1 = keypoints1[i1];
        const KeyPoint kp2 = keypoints2[i2];

        // This is ad-hoc, adjust to suit your need
        if(matches[i].distance < 0.45) {
            match_list.push_back( pntMatch(kp1.pt,kp2.pt));
        }
    }

}


/******************************************************************************/
void convertMatch2Mat(const vector <pntMatch> &matches, Mat &src, Mat &dst)
{
    src = Mat::zeros(matches.size(), 2, CV_32F);
    dst = Mat::zeros(matches.size(), 2, CV_32F);

    for(unsigned int i=0; i < matches.size(); i++) {
        src.at<float>(i,0) = matches[i].first.x;
        src.at<float>(i,1) = matches[i].first.y;

        dst.at<float>(i,0) = matches[i].second.x;
        dst.at<float>(i,1) = matches[i].second.y;
    }
}

void FindBestRotation(const vector <pntMatch>& matches, const ImageModel& pano1, const ImageModel&pano2 , double& yaw, double& pitch, double& roll){

    // Find the best yaw,pitch,roll that transforms pts2 to pts1
    // Uses the same technique used find the optimal transformation between 3D points, except in 2D.
    // This algorithm is used in Iterative Closest Point matching as well.

    // First, convert the pixel co-ordinates to panoramic space
    vector <Point2d> pts1(matches.size()) ;// x,y is yaw/pitch position in panoramic space
    vector <Point2d> pts2(matches.size());

    for(unsigned int i=0; i < matches.size(); i++) {
        double x1 = matches[i].first.x;
        double y1 = matches[i].first.y;
        double x2 = matches[i].second.x;
        double y2 = matches[i].second.y;

        double focal1 = pano1.focal;
        double cx1 = pano1.cx;
        double cy1 = pano1.cy;

        double focal2 = pano2.focal;
        double cx2 = pano2.cx;
        double cy2 = pano2.cy;

        Pixel2Angles(x1, y1, focal1, cx1, cy1, pts1[i].x, pts1[i].y);
        Pixel2Angles(x2, y2, focal2, cx2, cy2, pts2[i].x, pts2[i].y);
    }

    // Find the centroid of both sets
    Point2d centre1(0.0, 0.0);
    Point2d centre2(0.0, 0.0);

    for(unsigned int i=0; i < pts1.size(); i++) {
        centre1 += pts1[i];
        centre2 += pts2[i];
    }

    // OpenCV 2.x doesn't have operator/= implemented?
    centre1 *= (1.0 / pts1.size());
    centre2 *= (1.0 / pts1.size());

    // Now find the optimial rotation using SVD
    // Bring both sets to the centre
    for(unsigned int i=0; i < pts1.size(); i++) {
        pts1[i] -= centre1;
        pts2[i] -= centre2;
    }

    // Accumulate H
    Mat H = Mat::zeros(2,2, CV_64F);

    for(unsigned int i=0; i < pts1.size(); i++) {
        Mat A(2,1, CV_64F);
        Mat B(1,2, CV_64F);

        A.at<double>(0,0) = pts2[i].x;
        A.at<double>(1,0) = pts2[i].y;

        B.at<double>(0,0) = pts1[i].x;
        B.at<double>(0,1) = pts1[i].y;

        H += A*B;
    }

    SVD svd(H);

    // R = V*U'
    // This is a 2x2 rotation matrix, 2D rotation
    Mat R = svd.vt.t() * svd.u.t();

    // Build the 3x3 transformation matrix
    Mat R33 = Mat::eye(3,3, CV_64F);
    Mat T1 = Mat::eye(3,3, CV_64F); // bring points to centre of pts1
    Mat T2 = Mat::eye(3,3, CV_64F); // bring points to centre of pts2

    for(int y=0; y < 2; y++) {
        for(int x=0; x < 2; x++) {
            R33.at<double>(y,x) = R.at<double>(y,x);
        }
    }

    T1.at<double>(0,2) = centre1.x;
    T1.at<double>(1,2) = centre1.y;

    T2.at<double>(0,2) = -centre2.x;
    T2.at<double>(1,2) = -centre2.y;

    Mat transform = T1*R33*T2;

    yaw = transform.at<double>(0,2);
    pitch = transform.at<double>(1,2);
    roll = atan2(-R.at<double>(0,1), R.at<double>(0,0));
}

// Inline functions have to be declared in the header
/******************************************************************************/
/*
   double RAD_TO_DEG(double x)
   {
   return x*180.0/M_PI;
   }
 */
/******************************************************************************/
/*
   double SQ(double x)
   {
   return x*x;
   }
 */
/******************************************************************************/
// Force angle to lie between -180 and 180 degrees
/*
   double BoundAngle(double x)
   {
   if(x < -M_PI) {
   x += 2*M_PI;
   }

   if(x > M_PI) {
   x -= 2*M_PI;
   }

   return x;
   }
 */
/******************************************************************************/
/*
   double Pixel2Yaw(int x, int width)
   {
// 0 degrees at x = 0
return 2.0*M_PI*x/width;
}
 */
/******************************************************************************/
/*
   double Pixel2Pitch(int y, int height)
   {
// -90 degrees at y = 0
return -M_PI_2 + M_PI*y/height;
}
 */
/******************************************************************************/
/*
   double Yaw2Pixel(double yaw, int width)
   {
   return yaw*width/(2.0*M_PI);
   }
 */
/******************************************************************************/
/*
   double Pitch2Pixel(double pitch, int height)
   {
   return (pitch + M_PI_2) / (M_PI/height);
   }
 */
/******************************************************************************/
void Pixel2Angles(double x, double y, double focal, double cx, double cy, double &yaw, double &pitch)
{

    double px = x - cx;
    double py = y - cy;

    yaw = atan2(px,focal);

    double r = sqrt(focal*focal + px*px);

    pitch = atan(py/r);
}
/******************************************************************************/
bool Angles2Pixel(double yaw, double pitch, double roll, double focal, double cx, double cy, double &x, double &y)
{
    // Given 3 angles we want to find where this ray intersects on the rectilinear image

    // Out of bounds for rectilinear images  180 degrees (-90, 90) FOV maximum
    if(fabs(yaw) > M_PI_2) {
        return false;
    }

    if(fabs(pitch) > M_PI_2) {
        return false;
    }

    // Apply the roll to the yaw/pitch
    roll = -roll;
    double yaw2 = yaw*cos(roll) - pitch*sin(roll);
    double pitch2 = yaw*sin(roll) + pitch*cos(roll);

    double px = focal*tan(yaw2);
    double r = sqrt(focal*focal + px*px);
    double py = r*tan(pitch2);

    x = cx + px;
    y = cy + py;

    return true;
}

Mat show_matches( Mat const& imgA, Mat const& imgB, vector<pntMatch>const& matches ){

    //concatenate the image together
    Mat concImage( std::max( imgA.rows, imgB.rows ), imgA.cols+imgB.cols, CV_8UC3 );
    concImage = Scalar(0);

    //add image A
    for( int i=0; i<imgA.rows; i++)
        for( int j=0; j<imgA.cols; j++)
            concImage.at<Vec3b>(i,j) = imgA.at<Vec3b>(i,j);
    
    //add image B
    for( int i=0; i<imgB.rows; i++)
        for( int j=0; j<imgB.cols; j++)
            concImage.at<Vec3b>(i,j+imgA.cols) = imgA.at<Vec3b>(i,j);
    
    //print lines for matches
    Point pt;
    for( size_t i=0; i<matches.size(); i++ ){
        pt.x = matches[i].second.x + imgA.cols;
        pt.y = matches[i].second.y;
        line( concImage, matches[i].first, pt, Scalar(0, 255, 0), 1 );
    }


    cout << "stat" << endl;
    imshow("WINDOW", concImage);
    waitKey(0);
    
    return concImage;
}

