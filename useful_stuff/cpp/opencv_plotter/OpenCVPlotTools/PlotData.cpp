#include "PlotData.hpp"
#include "PlotMath.hpp"

#include <iostream>

namespace cvplt{

/**
 * PlotParams
*/
PlotParams::PlotParams(){

    //create the color
    color = Vec3b(0,0,255);
    
    limit_set = false;
}

bool PlotParams::is_limit_set()const{
    return limit_set;
}

void PlotParams::set_limits( const vector<Point2d>& lims ){
    limits = lims;
    limit_set = true;
}
        
Point2d PlotParams::xlim()const{
    return limits[0];
}

Point2d PlotParams::ylim()const{
    return limits[1];
}

Point2d PlotParams::get_limit( const int& idx )const{
    return limits[idx];
}

/**
 * Parameterize constructor for PlotData
*/
PlotData::PlotData( vector<double>const& x, vector<double>const& y, PlotParams const& plot_params ){
    
    //add axis to data
    data.push_back(x);
    data.push_back(y);
    params = plot_params;
    
    //compute the limits if not done so already
    if( params.is_limit_set() == false ){

        //compute the bounds
        vector<Point2d> limits(data.size());
    
        //iterate through each data dimension
        for( size_t i=0; i<data.size(); i++){
            limits[i] = compute_range(data[i]);
            if( fabs(limits[i].x - limits[i].y ) < 0.0001 ){
                limits[i].x -= 1;
                limits[i].y += 1;
            }
        }

        //pass to parameters
        params.set_limits( limits );
    }

}

/**
 * Render the plot data info on the output image
*/
void PlotData::render( Mat& output_image, Rect const& rect_pos )const{

    //for each point in the object, compute its image coordinate
    Point pos;
    for( size_t i=0; i<data[0].size(); i++){
        
        pos.x = ((data[0][0] - params.xlim().x)/(params.xlim().y-params.xlim().x)*(rect_pos.br().x-rect_pos.tl().x))+rect_pos.tl().x;
        pos.y = ((data[1][0] - params.ylim().x)/(params.ylim().y-params.ylim().x)*(rect_pos.br().y-rect_pos.tl().y))+rect_pos.tl().y;
    }

}

}// end of cvplt Namespace
