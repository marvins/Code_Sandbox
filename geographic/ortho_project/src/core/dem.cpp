#include "dem.hpp"

#include "../math/Geometry.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <set>
#include <vector>

using namespace cv;
using namespace std;

class pnt_sort{
    
    public:

    bool operator () ( const Point3f& a, const Point3f& b ){
        if( fabs( a.x - b.x ) < 0.0001 ){
            if( fabs( a.y - b.y ) < 0.0001 )
                return true;
            else
                return a.y < b.y;
        }
        return a.x < b.x;
    }
};

class pnt_comp{
    
    public:

    bool operator () ( const Point3f& a, const Point3f& b ){
        if( fabs( a.x - b.x ) < 0.0001 ){
            if( fabs( a.y - b.y ) < 0.0001 )
                return true;
        }
        return false;
    }
};



/**
 * Correct the coordinate using elevation data. 
 *
 * - Compute the maximum radius to search around.
 * - Start at ground point and iterate to radius, searching for occlusions. 
 * 
*/
void dem_correction( Mat& world_position, const double& gsd, Options& options, const Mat& minPnt, const Mat& maxPnt ){
   
    // return when in a live run
    return;

    //compute the initial position
    Mat dnorm = options.Position_i - world_position;
    Point2f base(dnorm.at<double>(0,0), dnorm.at<double>(1,0));

    Point3f camPoint( options.Position_i.at<double>(0,0), 
                      options.Position_i.at<double>(1,0),
                      options.Position_i.at<double>(2,0));
    Point3f gndPoint( world_position.at<double>(0,0),
                      world_position.at<double>(1,0),
                      world_position.at<double>(2,0));

    vector<Point3f> pntList;

    bool intersects=false;
    double distance = norm(base);

    Mat max_point = world_position.clone();

    //compute the step count
    Point2f s0 = Point2f( world_position.at<double>(0,0), 
                          world_position.at<double>(1,0) );
   

    // the direction is the direction vector with a length of the gsd
    Point2f dir = (1.0 / gsd) * ( 1 / norm(base) ) * base;
    Point2f pnt0, pnt1;

    // get the number of steps
    int stepCount = norm(base) / gsd;

    // find all points on the line
    for( int i=0; i<stepCount; i++ ){

        // increment the point along the line
        pnt1 = s0 + (i-0)*dir; 
        
        for( int x=-1; x<=1; x++ )
        for( int y=-1; y<=1; y++ ){

            //increment and decrement by gsd
            Point3f tpnt( pnt1.x + (x*gsd), pnt1.y + (y*gsd), 
                         query_dem( Point2f( pnt1.x + (x*gsd), pnt1.y + (y*gsd)), 
                                    options.minDem, options.maxDem, options.dem ));

            //add point to set
            pntList.push_back( tpnt );
        }

        vector<Point3f>::iterator it = pntList.begin();
        sort( pntList.begin(), pntList.end(), pnt_sort() );
        it = unique( pntList.begin(), pntList.end(), pnt_comp() );
        pntList.resize( it - pntList.begin());
        
        //it = pntList.begin();
        intersects = false;
        int state;
       
        //check every existing point for an intersection
        for( int z=0; z<pntList.size(); z++ ){
            
            //check if the points intersect the line
            double tdist;
            state = compute3d_line_line_intersection( camPoint, gndPoint, pntList.front(), Point3f((*it).x,(*it).y, -1), tdist, 0.001 ); 
            
            if( state != 0 ){
                
                //compare the distance
                if( tdist < distance ){
                    distance = tdist;
                    max_point = load_point( (*it).x, (*it).y, (*it).z);
                }
            }
            
            pntList.pop_back();
        }
        
    }

    world_position = max_point.clone();

    //compare every point on the list for an intersection


    /*
    Point2f dir(base.x/norm(base), base.y/norm(base));

    int maxD = min( norm(base), options.max_elevation );

    //start at the base and step in increments of one until the full range is met
    set<Point3f,pnt_comp> pntList;
    Point pnt0 = base;
    Point pnt1 = pnt0;
    int minx, miny, maxx, maxy;
    for( int i=1; i<maxD; i++){
        
        //add all surrounding points to the set
        pnt1 = base + i*dir;
        pnt0 = base + (i-1)*dir;
       
        minx = std::min( pnt1.x, pnt0.x);
        maxx = std::max( pnt1.x, pnt0.x);
        miny = std::min( pnt1.y, pnt0.y);
        maxy = std::max( pnt1.y, pnt0.y);
        
        for( int x=minx; x<=maxx; x++ )
        for( int y=miny; y<=maxy; y++ )
            pntList.insert( Point(x,y, ) );

    }
    */
    

}


double query_dem( Point2f const& pnt, Point2f const& minPnt, Point2f const& maxPnt, Mat const& dem ){
    
    Point pix; 
    pix.x = (pnt.x - minPnt.x)/( maxPnt.x - minPnt.x) * dem.cols;
    pix.y = dem.rows - ((pnt.y - minPnt.y)/( maxPnt.y - minPnt.y) * dem.rows);
   
    if( pix.x < 0 || pix.y < 0 || pix.x >= dem.cols || pix.y >= dem.rows )
        return 0;

    if( dem.type() == CV_8UC1 )
        return (double)dem.at<uchar>(pix);
    else
        throw string("ERROR: STOP HERE");

    return 0;

}


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

double query_max_elevation( const Mat& dem ){
    
    double maxE = dem.at<double>(0,0);
    for( int i=0; i<dem.cols; i++ )
        for( int j=0; j<dem.rows; j++ ){

            if( dem.type() == CV_8UC1 ){
                if( maxE < dem.at<uchar>(j,i) )
                    maxE = dem.at<uchar>(j,i);
            }
            else if( dem.type() == CV_64FC1 ){
                if( maxE < dem.at<double>(j,i) )
                    maxE = dem.at<double>(j,i);
            }
            else
                throw string("ERROR: PAUSE");
        }
    
    // return max elevation
    return maxE;
}
