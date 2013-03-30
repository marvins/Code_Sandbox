
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <numeric>
#include <omp.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/nonfree/features2d.hpp>
#include <opencv2/legacy/legacy.hpp>
#include "lmfit/lmmin.h"

using namespace std;
using namespace cv;

typedef pair<Point2d,Point2d> MatchPair; // doubles

struct PanoImage
{
   Mat img; // image
   double focal; // focal length of the camera in pixels
   double cx, cy; // optical centre of the camera in pixels
   double yaw, pitch, roll; // offset in the panoramic image
};

Mat global_image;
Mat global_count;
bool global_set = false;

/******************************************************************************/
// Global variables - used to pass to the optimisation routine, not the best programming practice ...
const double PANO_REF_YAW = 0; // reference image's yaw
const double PANO_REF_PITCH = 0; // reference image's pitch
const double PANO_REF_ROLL = 0; // reference image's roll
double CX = 0; // optical centre of image, determined on runtime
double CY = 0; // optical centre of image, determined on runtime

int TMP_PANO_WIDTH = 1;
int TMP_PANO_HEIGHT = 1;

/******************************************************************************/
void FindMatches(const Mat &img1, const Mat &img2, vector <MatchPair> &ret_matches); // Uses SURF
void Convert(const vector <MatchPair> &matches, Mat &src, Mat &dst);
void BuildPanoramic(const vector <PanoImage> &pano_images, double scale_image, int *tmp_width);
void FindBoundingBox(const Mat &img, int *x1, int *y1, int *x2, int *y2);
double CalcProjectionError(const vector < vector <MatchPair> > &matches, int pano_width);

// Debug
void SaveMatches(const char *filename, vector<MatchPair> matches);
vector<MatchPair> LoadMatches(const char *filename);

// Functions used in refinement
void SetDefaultCameraParams(vector <PanoImage> *pano_images, double focal, double cx, double cy);
void MakeParams(const vector <PanoImage> &pano_images, vector <double> *params);
void ObjectiveFunction(const double *params, int num_inputs, const void *_inputs, double *fvec, int *info);
double RAD_TO_DEG(double x);
double SQ(double x);
double BoundAngle(double x);

double Pixel2Yaw(int x, int width);
double Pixel2Pitch(int y, int height);
double Yaw2Pixel(double yaw, int width);
double Pitch2Pixel(double pitch, int height);
void Pixel2Angles(double x, double y, double focal, double cx, double cy, double *yaw, double *pitch);
bool Angles2Pixel(double yaw, double pitch, double roll, double focal, double cx, double cy, double *x, double *y);

void FindBestRotation(const vector <MatchPair> &matches, const PanoImage &pano1, const PanoImage &pano2, double *yaw, double *pitch, double *roll);

/******************************************************************************/


/**
 * Read images from file
 *
 * @param[in] file File list
 * @param[out] pano_images Panoramic Image List
 */
void read_images( const string& file, vector<PanoImage>& pano_images ){

   vector<string> filenames;

   ifstream fin;
   fin.open(file.c_str());

   string str;
   fin >> str;

   while( !fin.eof() ){

      filenames.push_back(str);
      fin >> str;
   }

   fin.close();

   pano_images.resize(filenames.size());
   for(size_t i=0; i<filenames.size(); i++){
      pano_images[i].img = imread(filenames[i].c_str());
      if( pano_images[i].img.empty() == true ){
         cout << "Can't read " << filenames[i] << endl;
         exit(0);
      }
   }

}


/**
 * Main Driver
 *
 * param[in] argc number of arguments
 * param[in] argv list of arguments
 * return Exit Status
 */
int main(int argc, char **argv)
{

   //create list of image containers for stitching
   vector <PanoImage> pano_images;
   read_images( argv[1], pano_images);


   // Initialise some camera intrinsics for each image
   {
      // We'll just assume the optical centre is at the centre of the image.
      // This works good enough for me in practice.
      CX = pano_images[0].img.cols/2;
      CY = pano_images[0].img.rows/2;

      // Sets the camera parameter for all the images, assume the same parameters
      // I got the focal length from checkerboard calibration
      // I shrunk the original images 4 time, so scaling the focal length by 0.25
      SetDefaultCameraParams(&pano_images, 321.26 /* focal in pixels */, CX, CY);
   }

   // Find matches between each image pair
   vector < vector<MatchPair> > all_matches(pano_images.size() - 1);
   int num_matches = 0; // keep track of total good matched points

#pragma omp parallel for
   for(unsigned int i=1; i < pano_images.size(); i++) {
      vector<MatchPair> matches, matches2; // unfiltered and filtered

      FindMatches(pano_images[i].img, pano_images[i-1].img, matches);

      // For debugging - skips the slow finding matches step
      {
         char file[512];
         sprintf(file, "matches-%d.txt", i);
         SaveMatches(file, matches);
         matches = LoadMatches(file);
      }

      Mat pts1, pts2;
      Convert(matches, pts1, pts2);

      // Use the homography to filter out bad matches
      vector <uchar> mask; // mask to indicate which matches are good or bad
      Mat H = findHomography(pts2, pts1, mask, CV_RANSAC, 2.0);

      // Filter out bad matches
      for(unsigned int j=0; j < mask.size(); j++) {
         if(mask[j]) {
            matches2.push_back(matches[j]);
         }
      }

      // Calculate initial yaw, pitch, roll for this image
      double yaw, pitch, roll;
      FindBestRotation(matches2, pano_images[i-1], pano_images[i], &yaw, &pitch, &roll);

      // These are our initial guesses. We'll covert them to absolute angles and refine them later
      pano_images[i].yaw = -yaw;
      pano_images[i].pitch = -pitch;
      pano_images[i].roll = -roll;

      all_matches[i-1] = matches2;

#pragma omp critical
      {
         num_matches += matches2.size();
      }
   }

   // Convert our esimtated yaw, pitch, roll to absolute position in panoramic space
   {
      // NOTE: assumes the images are taken in sequence

      // The reference image
      pano_images[0].yaw = PANO_REF_YAW;
      pano_images[0].pitch = PANO_REF_PITCH;
      pano_images[0].roll = PANO_REF_ROLL;

      for(unsigned int i=1; i < pano_images.size(); i++) {
         pano_images[i].yaw += pano_images[i-1].yaw;
         pano_images[i].pitch += pano_images[i-1].pitch;
         pano_images[i].roll += pano_images[i-1].roll;
      }
   }

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
   }

   // Okay, now we'll create our masterpeice!
   int tmp_width; // the width of the big temporary panoramic image
   BuildPanoramic(pano_images, 1.0 /* scale output */, &tmp_width); // Scaling is a good idea if you got big images!

   // Calculate the reprojection erorr in pixels
   {
      vector <double> fvec;
      fvec.resize(num_matches);

      ObjectiveFunction(&params[0], num_matches, &all_matches, &fvec[0], NULL);

      double mse = std::accumulate(fvec.begin(), fvec.end(), 0.0) / fvec.size();
      double rmse = sqrt(mse);
      printf("Root mean squared error (pixels): %g\n", rmse);
   }


   cout << "Printing Global Image" << endl;
   Mat result_image( global_image.size(), CV_8UC3 );
   for( int i=0; i<global_image.rows; i++ )
      for( int j=0; j<global_image.cols; j++ ){
         if( global_count.at<uchar>(i,j) > 0 ){
            result_image.at<Vec3b>(i,j)[0] = ((double)global_image.at<Vec3d>(i,j)[0])/ global_count.at<uchar>(i,j);
            result_image.at<Vec3b>(i,j)[1] = ((double)global_image.at<Vec3d>(i,j)[1])/ global_count.at<uchar>(i,j);
            result_image.at<Vec3b>(i,j)[2] = ((double)global_image.at<Vec3d>(i,j)[2])/ global_count.at<uchar>(i,j);
         }
         else
            result_image.at<Vec3b>(i,j) = Vec3b(0,0,0);
      }   

   imwrite("output.png", result_image );

   return 0;
}
/******************************************************************************/
void Convert(const vector <MatchPair> &matches, Mat &src, Mat &dst)
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
/******************************************************************************/
// Find matches between img1 and img2 and store in ret_matches
void FindMatches(const Mat &img1, const Mat &img2, vector<MatchPair> &ret_matches)
{
   Mat grey1, grey2;

   cvtColor(img1, grey1, CV_BGR2GRAY);
   cvtColor(img2, grey2, CV_BGR2GRAY);

   // detecting keypoints
   SurfFeatureDetector detector(400);
   vector<KeyPoint> keypoints1, keypoints2;
   detector.detect(grey1, keypoints1);
   detector.detect(grey2, keypoints2);

   // computing descriptors
   SurfDescriptorExtractor extractor;
   Mat descriptors1, descriptors2;
   extractor.compute(grey1, keypoints1, descriptors1);
   extractor.compute(grey2, keypoints2, descriptors2);

   // matching descriptors
   vector<DMatch> matches;
   BruteForceMatcher<L2<float> > matcher;
   matcher.match(descriptors1, descriptors2, matches);

   ret_matches.clear();
   for( int i = 0; i < (int)matches.size(); i++) {
      int i1 = matches[i].queryIdx;
      int i2 = matches[i].trainIdx;

      const KeyPoint &kp1 = keypoints1[i1];
      const KeyPoint &kp2 = keypoints2[i2];

      // This is ad-hoc, adjust to suit your need
      if(matches[i].distance < 0.55) {
         ret_matches.push_back(MatchPair(kp1.pt,kp2.pt));
      }
   }
}
/******************************************************************************/
// Output the stitched panoramic as multiple layers/images.
// It creates a big temporary buffer first and fills in the pixel.
// A bounding box is then calculated to find the layer size to output.
// Then each layer is filled up.
void BuildPanoramic(const vector <PanoImage> &pano_images, double scale_image, int *tmp_width)
{
   int width = pano_images[0].img.cols;
   int height = pano_images[0].img.rows;

   // Calculate the optimal size panoramic image

   // Calculate the FOV for the first image, and use that to determine the size of the temporary panoramic image
   // This image can get VERY VERY large, so be careful!
   double x = pano_images[0].img.cols - pano_images[0].cx;
   double FOV = atan2(x, pano_images[0].focal) * 2;

   int pano_width = width * (2*M_PI/FOV);
   int pano_height = pano_width / 2;

   pano_width *= scale_image;
   pano_height *= scale_image;

   *tmp_width = pano_width;
   TMP_PANO_WIDTH = pano_width;
   TMP_PANO_HEIGHT = pano_height;

   printf("temporary panoramic width: %d\n", pano_width);
   printf("temporary panoramic height: %d\n", pano_height);

   Mat temp_image = Mat::zeros(pano_height, pano_width, CV_8U);

   // This bit can get very slow for very large images
   // Use OpenMP for automatic parallelisation of loop
#pragma omp parallel for
   for(int y=0; y < pano_height; y++) {
      for(int x=0; x < pano_width; x++) {
         double yaw = Pixel2Yaw(x, pano_width) - FOV/2; // Added the offset so the panoramic image starts at x=0 (or close to it)
         double pitch = Pixel2Pitch(y, pano_height);

         // Search list of panoramic image to see any pixels can be used here
         for(unsigned int i=0; i < pano_images.size(); i++) {
            double yaw_rel = yaw - pano_images[i].yaw;
            double pitch_rel = pitch - pano_images[i].pitch;

            double px, py;

            if(!Angles2Pixel(yaw_rel, pitch_rel, pano_images[i].roll, pano_images[i].focal, pano_images[i].cx, pano_images[i].cy, &px, &py)) {
               continue;
            }

            if(px >= 0 && px < width && py >= 0 && py < height) {
               temp_image.at<uchar>(y,x) = 1;
            }
         }
      }
   }

   int x1, y1, x2, y2;
   int layer_width, layer_height;

   FindBoundingBox(temp_image, &x1, &y1, &x2, &y2);

   layer_width = x2 - x1 + 1;
   layer_height = y2 - y1 + 1;

   if( global_set == false ){
      global_set = true;
      global_image = Mat::zeros( layer_height, layer_width, CV_64FC3 );
      global_count = Mat::zeros( layer_height, layer_width, CV_8UC1 );
   }

   // Use OpenMP for automatic parallelisation of loop
#pragma omp parallel for
   for(unsigned int i=0; i < pano_images.size(); i++) {
      Mat layer = Mat::zeros(layer_height, layer_width, CV_8UC4); // 4 channel

      for(int y=y1; y <= y2; y++) {
         for(int x=x1; x <= x2; x++) {
            double yaw = Pixel2Yaw(x, pano_width) - FOV/2;
            double pitch = Pixel2Pitch(y, pano_height);

            double yaw_rel = yaw - pano_images[i].yaw;
            double pitch_rel = pitch - pano_images[i].pitch;

            double px, py;

            if(!Angles2Pixel(yaw_rel, pitch_rel, pano_images[i].roll, pano_images[i].focal, pano_images[i].cx, pano_images[i].cy, &px, &py)) {
               continue;
            }

            if(px > 0 && px < width-1 && py > 0 && py < height-1) {
               // For better results, use bilinear interpolation
               int ix = (int)(px + 0.5);
               int iy = (int)(py + 0.5);

               layer.at<Vec4b>(y-y1, x-x1)[0] = pano_images[i].img.at<Vec3b>(iy,ix)[0];
               layer.at<Vec4b>(y-y1, x-x1)[1] = pano_images[i].img.at<Vec3b>(iy,ix)[1];
               layer.at<Vec4b>(y-y1, x-x1)[2] = pano_images[i].img.at<Vec3b>(iy,ix)[2];
               layer.at<Vec4b>(y-y1, x-x1)[3] = 255;

               global_image.at<Vec3d>(y-y1, x-x1)[0] += layer.at<Vec4b>(y-y1, x-x1)[0];
               global_image.at<Vec3d>(y-y1, x-x1)[1] += layer.at<Vec4b>(y-y1, x-x1)[1];
               global_image.at<Vec3d>(y-y1, x-x1)[2] += layer.at<Vec4b>(y-y1, x-x1)[2];
               global_count.at<uchar>(y-y1, x-x1) += 1;
            }
         }
      }

      char file[512];
      sprintf(file, "layer-%03d.png", i);
      imwrite(file, layer);

      printf("Saving %s ...\n", file);
   }
}
/******************************************************************************/
// Set some default camera intrinisic properties for all the images
void SetDefaultCameraParams(vector <PanoImage> *pano_images, double focal, double cx, double cy)
{
   vector <PanoImage> &_pano_images = *pano_images;

   for(unsigned int i=0; i < _pano_images.size(); i++) {
      _pano_images[i].focal = focal;
      _pano_images[i].cx = cx;
      _pano_images[i].cy = cy;
   }
}
/******************************************************************************/
void MakeParams(const vector <PanoImage> &pano_images, vector <double> *params)
{
   // Optimise for focal, cx, cy, yaw, pitch, roll - 6 params
   vector <double> &_params = *params;

   for(unsigned int i=0; i < pano_images.size(); i++) {
      _params.push_back(pano_images[i].focal);
      _params.push_back(pano_images[i].yaw);
      _params.push_back(pano_images[i].pitch);
      _params.push_back(pano_images[i].roll);
   }
}
/******************************************************************************/
// The function we aim to minimise
void ObjectiveFunction(const double *params, int num_inputs, const void *_inputs, double *fvec, int *info)
{
   vector < vector<MatchPair> > &all_matches = *((vector < vector<MatchPair> >*)_inputs);

   int k = 0;

   for(unsigned int i=0; i < all_matches.size(); i++) {
      double focal1 = params[i*4];
      double yaw1   = params[i*4 + 1];
      double pitch1 = params[i*4 + 2];
      double roll1  = params[i*4 + 3];

      // Hackish way to stop the algorithm from refining these values
      if(i == 0) {
         yaw1 = PANO_REF_YAW;
         pitch1 = PANO_REF_PITCH;
         roll1 = PANO_REF_ROLL;
      }

      double focal2 = params[(i+1)*4];
      double yaw2   = params[(i+1)*4 + 1];
      double pitch2 = params[(i+1)*4 + 2];
      double roll2  = params[(i+1)*4 + 3];

      vector <MatchPair> &matches = all_matches[i];

      for(unsigned int j=0; j < matches.size(); j++) {
         double x1 = matches[j].second.x;
         double y1 = matches[j].second.y;
         double x2 = matches[j].first.x;
         double y2 = matches[j].first.y;

         // relative and absolute
         double yaw_rel1, yaw_abs1;
         double yaw_rel2, yaw_abs2;
         double pitch_rel1, pitch_abs1;
         double pitch_rel2, pitch_abs2;

         // Calculate the relative (relative to the image) yaw, pitch the matching (x,y) makes
         Pixel2Angles(x1, y1, focal1, CX, CY, &yaw_rel1, &pitch_rel1);
         Pixel2Angles(x2, y2, focal2, CX, CY, &yaw_rel2, &pitch_rel2);

         // Convert the relative to absolute co-ordinates (panoramic space)
         yaw_abs1 = yaw1 + yaw_rel1*cos(roll1) - pitch_rel1*sin(roll1);
         pitch_abs1 = pitch1 + yaw_rel1*sin(roll1) + pitch_rel1*cos(roll1);

         // Repeat for the second matches
         yaw_abs2 = yaw2 + yaw_rel2*cos(roll2) - pitch_rel2*sin(roll2);
         pitch_abs2 = pitch2 + yaw_rel2*sin(roll2) + pitch_rel2*cos(roll2);

         // Calculate the projection error in pixel space, more intuitive
         yaw_abs1 = BoundAngle(yaw_abs1);
         yaw_abs2 = BoundAngle(yaw_abs2);

         pitch_abs1 = BoundAngle(pitch_abs1);
         pitch_abs2 = BoundAngle(pitch_abs2);

         // Initially TMP_PANO_WIDTH/HEIGHT is set to 1.0. This is fine.
         x1 = Yaw2Pixel(yaw_abs1, TMP_PANO_WIDTH);
         x2 = Yaw2Pixel(yaw_abs2, TMP_PANO_WIDTH);

         y1 = Pitch2Pixel(pitch_abs1, TMP_PANO_HEIGHT);
         y2 = Pitch2Pixel(pitch_abs2, TMP_PANO_HEIGHT);

         double err2 = SQ(x1-x2) + SQ(y1-y2);

         // Error in panoramic space
         //double err2 = SQ(yaw_abs2 - yaw_abs1) + SQ(pitch_abs2 - pitch_abs1);

         fvec[k] = err2;
         k++;
      }
   }
}
/******************************************************************************/
// Find the bounding box of an image
void FindBoundingBox(const Mat &img, int *x1, int *y1, int *x2, int *y2)
{
   *x1 = img.cols;
   *y1 = img.rows;
   *x2 = -img.cols;
   *y2 = -img.rows;

   for(int y=0; y < img.rows; y++) {
      for(int x=0; x < img.cols; x++) {
         if(img.at<uchar>(y,x) > 0 || img.at<uchar>(y,x) > 0 || img.at<uchar>(y,x) > 0) {
            if(x < *x1) *x1 = x;
            if(y < *y1) *y1 = y;
            if(x > *x2) *x2 = x;
            if(y > *y2) *y2 = y;
         }
      }
   }
}
/******************************************************************************/
void SaveMatches(const char *filename, vector<MatchPair> matches)
{
   FILE *fp = fopen(filename, "w+");
   if(fp == NULL) {
      fprintf(stderr, "Error opening %s\n", filename);
      perror("fopen");
      exit(1);
   }

   fprintf(fp, "%ld\n", matches.size());
   for(unsigned int i=0; i < matches.size(); i++) {
      fprintf(fp, "%f %f %f %f\n", matches[i].first.x, matches[i].first.y, matches[i].second.x, matches[i].second.y);
   }

   fclose(fp);
}
/******************************************************************************/
vector<MatchPair> LoadMatches(const char *filename)
{
   vector<MatchPair> ret;
   char line[1024];
   int num;

   FILE *fp = fopen(filename, "r");
   if(fp == NULL) {
      fprintf(stderr, "Error opening %s\n", filename);
      perror("fopen");
      exit(-1);
   }

   if(fgets(line, sizeof(line), fp) == NULL) {
      perror("fgets");
      exit(-1);
   }

   sscanf(line, "%d", &num);

   ret.resize(num);

   for(unsigned int i=0; i < ret.size(); i++) {
      if(fgets(line, sizeof(line), fp) == NULL) {
         perror("fgets");
         exit(-1);
      }

      sscanf(line, "%lf %lf %lf %lf", &ret[i].first.x, &ret[i].first.y, &ret[i].second.x, &ret[i].second.y);
   }

   fclose(fp);

   return ret;
}

// Inline functions have to be declared in the header
/******************************************************************************/
double RAD_TO_DEG(double x)
{
   return x*180.0/M_PI;
}
/******************************************************************************/
double SQ(double x)
{
   return x*x;
}
/******************************************************************************/
// Force angle to lie between -180 and 180 degrees
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
/******************************************************************************/
double Pixel2Yaw(int x, int width)
{
   // 0 degrees at x = 0
   return 2.0*M_PI*x/width;
}
/******************************************************************************/
double Pixel2Pitch(int y, int height)
{
   // -90 degrees at y = 0
   return -M_PI_2 + M_PI*y/height;
}
/******************************************************************************/
double Yaw2Pixel(double yaw, int width)
{
   return yaw*width/(2.0*M_PI);
}
/******************************************************************************/
double Pitch2Pixel(double pitch, int height)
{
   return (pitch + M_PI_2) / (M_PI/height);
}
/******************************************************************************/
void Pixel2Angles(double x, double y, double focal, double cx, double cy, double *yaw, double *pitch)
{

   double px = x - cx;
   double py = y - cy;

   *yaw = atan2(px,focal);

   double r = sqrt(focal*focal + px*px);

   *pitch = atan(py/r);
}
/******************************************************************************/
bool Angles2Pixel(double yaw, double pitch, double roll, double focal, double cx, double cy, double *x, double *y)
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

   *x = cx + px;
   *y = cy + py;

   return true;
}

void FindBestRotation(const vector <MatchPair> &matches, const PanoImage &pano1, const PanoImage &pano2, double *yaw, double *pitch, double *roll)
{
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

      Pixel2Angles(x1, y1, focal1, cx1, cy1, &pts1[i].x, &pts1[i].y);
      Pixel2Angles(x2, y2, focal2, cx2, cy2, &pts2[i].x, &pts2[i].y);
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

   *yaw = transform.at<double>(0,2);
   *pitch = transform.at<double>(1,2);
   *roll = atan2(-R.at<double>(0,1), R.at<double>(0,0));
}
