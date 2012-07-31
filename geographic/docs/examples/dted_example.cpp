#include <iostream>

#include <GeoImage.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){

    try{

        //query the point independent of the dted file
        Point2f pnt( -118.2922, 36.57857 );
        GEO::DEM_Params params(GEO::DTED, "../../data/dted");

        double elevation = GEO::DEM( pnt, params ).get_elevation( );
        cout << "Elevation: " << elevation << endl;


        //lets pull a dted tile set which requires 4 tiles
        Point2f ul_nw( -119.2500, 37.2500);
        Point2f br_nw( -119.0001, 37.0000);

        Point2f ul_ne( -119.0000, 37.2500);
        Point2f br_ne( -118.7500, 37.0000);

        Point2f ul_se( -119.0000, 36.9999);
        Point2f br_se( -118.7500, 36.7500);

        Point2f ul_sw( -119.2500, 36.9999);
        Point2f br_sw( -119.0001, 36.7500);


        //build 4 topo maps
        GEO::DEM topo_map_nw( ul_nw.y, ul_nw.x, br_nw.y, br_nw.x, params);
        GEO::DEM topo_map_ne( ul_ne.y, ul_ne.x, br_ne.y, br_ne.x, params);
        GEO::DEM topo_map_sw( ul_sw.y, ul_sw.x, br_sw.y, br_sw.x, params);
        GEO::DEM topo_map_se( ul_se.y, ul_se.x, br_se.y, br_se.x, params);

        Mat nw = topo_map_nw.relief_map();
        Mat ne = topo_map_ne.relief_map();
        Mat sw = topo_map_sw.relief_map();
        Mat se = topo_map_se.relief_map();

        //create pano
        int width  = nw.cols + ne.cols;
        int height = nw.rows + sw.rows;
        cv::Mat pano( height, width, CV_8UC3 );

        //load images into mat
        for( size_t i=0; i<nw.cols; i++ )
            for( size_t j=0; j<nw.rows; j++ )
                pano.at<Vec3b>(j,i) = nw.at<Vec3b>(j,i);

        for( size_t i=0; i<ne.cols; i++ )
            for( size_t j=0; j<ne.rows; j++ )
                pano.at<Vec3b>(j,i+nw.cols) = ne.at<Vec3b>(j,i);

        for( size_t i=0; i<sw.cols; i++ )
            for( size_t j=0; j<sw.rows; j++ )
                pano.at<Vec3b>(j+nw.rows,i) = sw.at<Vec3b>(j,i);

        for( size_t i=0; i<se.cols; i++ )
            for( size_t j=0; j<se.rows; j++ )
                pano.at<Vec3b>(j+nw.rows,i+nw.cols) = se.at<Vec3b>(j,i);


        imwrite("pano.png", pano);
        cout << pano.cols << ", " << pano.rows << endl; 

        //build a single pano from a single stitch
        Point2f ul_big( -119.2500, 37.2500);
        Point2f br_big( -118.7500, 36.7500);
        cout << "A" << endl;
        GEO::DEM topo_map_big( ul_big.y, ul_big.x, 
                               br_big.y, br_big.x, 
                               params);

        cout << "B" << endl;
        Mat big_topo = topo_map_big.relief_map();
        cout << big_topo.cols << ", " << big_topo.rows << endl;
        imwrite("big_topo.png", big_topo);
        

    } catch ( string str ){
        cout << str << endl;
    }
    return 0;
}
