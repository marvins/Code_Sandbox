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


using namespace cv;
using namespace std;

typedef pair<Point2f, Point2f> pntMatch;

class ImageModel{
    
    public:
        Mat cimg;
        Mat gimg;

};

vector<Mat> convertBGR2Gray( vector<Mat>const& images );
vector<Mat> load_images( string const& filename );
vector<string>  read_image_list( string const& filename );
void show_image( Mat const& img );
void FindMatches( Mat const& img1, Mat const& img2, vector<pntMatch>& matches );
void convertMatch2Mat(const vector <pntMatch> &matches, Mat &src, Mat &dst);


int main( int argc, char* argv[] ){

    namedWindow("WINDOW");

    //create list of images
    vector<Mat> images      = load_images( argv[1] );
    vector<Mat> gray_images = convertBGR2Gray( images );
    
    //build image metrics
    vector< ImageModel >  imageModel( gray_images.size() );
    for( size_t i=0; i<gray_images.size(); i++ ){
        imageModel[i].cimg = images[i].clone();
        imageModel[i].gimg = gray_images[i].clone();
    }

    //find correspondencies between images
    vector<pntMatch> matches;

    for( size_t i=1; i< imageModel.size(); i++){
        
        //find matching keypoints between images
        FindMatches( imageModel[i-1].gimg, imageModel[i].gimg, matches );
        
        //convert match vector to Mats
        Mat pnts1, pnts2;
        convertMatch2Mat( matches, pnts1, pnts2 );
        
        //compute homography matrix
        vector<uchar> mask;
        Mat homography = findHomography( pnts2, pnts1, mask, CV_RANSAC, 2.0 );


    }


    return 0;

}

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
	FlannBasedMatcher matcher;
    matcher.match(descriptors1, descriptors2, matches);

    match_list.clear();
    for( int i = 0; i < (int)matches.size(); i++) {
        int i1 = matches[i].queryIdx;
        int i2 = matches[i].trainIdx;

        const KeyPoint &kp1 = keypoints1[i1];
        const KeyPoint &kp2 = keypoints2[i2];

        // This is ad-hoc, adjust to suit your need
        if(matches[i].distance < 0.05) {
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
