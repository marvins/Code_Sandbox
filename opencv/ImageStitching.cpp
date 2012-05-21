#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <fstream>
#include <iostream>
#include <utility>
#include <vector>

#include <opencv2/features2d/features2d.hpp>

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


int main( int argc, char* argv[] ){

    namedWindow("WINDOW");

    //create list of images
    vector<Mat> images = load_images( argv[1] );
    vector<Mat> gray_images = convertBGR2Gray( images );

    //build image metrics
    vector< ImageModel >  imageModel( gray_images.size() );
    for( size_t i=0; i<gray_images.size(); i++ ){
        imageModel[i].cimg = images[i].clone();
        imageModel[i].gimg = gray_images[i].clone();
    }

    //find correspondencies between images
    vector<pntMatch> matches;

    for( size_t i=0; i< imageModel.size(); i++)
        FindMatches( imageModel[i-1].gimg, imageModel[i].gimg, matches );



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

void FindMatches( Mat const& img1, Mat const& img2, vector<pntMatch>& matches ){

    SurfFeatureDetector detector(400);
    vector<KeyPoint> keypoints1, keypoints2;
    detector.detect( img1, keypoints1 );
    detector.detect( img2, keypoints2 );


}









