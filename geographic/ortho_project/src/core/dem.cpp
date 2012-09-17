#include "dem.hpp"

#include <iostream>

using namespace cv;
using namespace std;


double query_dem( Mat const& pnt, const Mat& stPnt, Options& options ){

    //find the min and max range, assum -500, 500
    Point demPnt(  pnt.at<double>(0,0) - stPnt.at<double>(0,0) + options.image.cols/2.0, 
                   pnt.at<double>(1,0) - stPnt.at<double>(1,0) + options.image.rows/2.0
                );
    
    if( demPnt.x >= 0 && demPnt.y >= 0 && demPnt.x < options.dem.cols && demPnt.y < options.dem.rows )
        return options.dem.at<uchar>(demPnt);
    else
        return 0;
}

double query_dem( Point2f const& pnt, const Point2f& stPnt, Options& options ){

    //find the min and max range, assum -500, 500
    Point demPnt = pnt - stPnt + Point2f( options.image.cols/2.0, options.image.rows/2.0);
    
    if( demPnt.x >= 0 && demPnt.y >= 0 && demPnt.x < options.dem.cols && demPnt.y < options.dem.rows )
        return options.dem.at<uchar>(demPnt);
    else
        return 0;

}

double query_dem( Point2f const& pnt, Options& options ){
    
    //find the min and max range, assum -500, 500
    Point demPnt = pnt + Point2f( options.image.cols/2.0, options.image.rows/2.0);
    
    if( demPnt.x >= 0 && demPnt.y >= 0 && demPnt.x < options.dem.cols && demPnt.y < options.dem.rows )
        return options.dem.at<uchar>(demPnt);
    else
        return 0;

}

double query_dem( Point3f const& pnt, Point3f const& stPnt, Options& options ){

    //find the min and max range, assum -500, 500
    Point demPnt( pnt.x - stPnt.x + options.image.cols/2.0, pnt.y -stPnt.y + options.image.rows/2.0);
    
    if( demPnt.x >= 0 && demPnt.y >= 0 && demPnt.x < options.dem.cols && demPnt.y < options.dem.rows )
        return options.dem.at<uchar>(demPnt);
    else
        return 0;

}

double query_dem( Point3f const& pnt, Options& options ){
    
    //find the min and max range, assum -500, 500
    Point demPnt( pnt.x + options.image.cols/2.0, pnt.y + options.image.rows/2.0);
    
    if( demPnt.x >= 0 && demPnt.y >= 0 && demPnt.x < options.dem.cols && demPnt.y < options.dem.rows )
        return options.dem.at<uchar>(demPnt);
    else
        return 0;

}

double query_max_elevation( const Mat& minPnt, const Mat& maxPnt, const Mat& starepoint, const Options& options ){

    
    //find the min and max range
    Point minP( std::max( (minPnt.at<double>(0,0)-starepoint.at<double>(0,0)) + options.image.cols/2.0, 0.0), 
                std::max( (minPnt.at<double>(1,0)-starepoint.at<double>(1,0)) + options.image.rows/2.0, 0.0));
    
    Point maxP( std::min( (maxPnt.at<double>(0,0)-starepoint.at<double>(0,0)) + options.image.cols/2.0, (double)options.dem.cols), 
                std::min( (maxPnt.at<double>(1,0)-starepoint.at<double>(1,0)) + options.image.rows/2.0, (double)options.dem.rows));
    
    double currentMax = 0;
    for( int i=minP.x; i<maxP.x; i++ )
        for( int j=minP.y; j<maxP.y; j++ )
            if( options.dem.at<uchar>(j,i) > currentMax ){
                currentMax = options.dem.at<uchar>(j,i);
            }
   
    return currentMax;
}

double query_max_elevation( const Point2f& minPnt, const Point2f& maxPnt, const Point2f& starepoint, const Options& options ){

    
    //find the min and max range
    Point minP( std::max( (minPnt.x-starepoint.x) + options.image.cols/2.0, 0.0), 
                std::max( (minPnt.y-starepoint.y) + options.image.rows/2.0, 0.0));
    
    Point maxP( std::min( (maxPnt.x-starepoint.x) + options.image.cols/2.0, (double)options.dem.cols), 
                std::min( (maxPnt.y-starepoint.y) + options.image.rows/2.0, (double)options.dem.rows));
    
    double currentMax = 0;
    for( int i=minP.x; i<maxP.x; i++ )
        for( int j=minP.y; j<maxP.y; j++ )
            if( options.dem.at<uchar>(j,i) > currentMax ){
                currentMax = options.dem.at<uchar>(j,i);
            }
   
    return currentMax;
}

