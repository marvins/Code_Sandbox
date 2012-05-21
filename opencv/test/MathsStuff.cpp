#include "MathsStuff.h"

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
