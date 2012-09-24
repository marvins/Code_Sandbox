#include "PlotData.hpp"
#include "PlotMath.hpp"
#include "PlotUtils.hpp"

#include <iostream>

namespace cvplt{

/**
 * PlotParams
*/
PlotParams::PlotParams(){
    load_defaults();
}

PlotParams::PlotParams( const string& plot_args ){
    
    load_defaults();

    //break up plot arguments
    vector<pair<string,string> > tokens = parse_plot_tokens( plot_args );

    for( size_t i=0; i<tokens.size(); i++){
        if( tokens[i].first == "LineWidth" )
            line_width = str2num<double>(tokens[i].second);
        else if( tokens[i].first == "LineStyle" ){
            
        }
        else 
            throw string(string("ERROR: ")+tokens[i].first+string(" is not a supported field"));
    }

}


void PlotParams::load_defaults(){
    //create the color
    color = Vec3b(0,0,255);
    
    //line width 
    line_width = 2;
    
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
    vector<Point> pointList;
    for( size_t i=0; i<data[0].size(); i++){
        
        pos.x = ((data[0][i] - params.xlim().x)/(params.xlim().y-params.xlim().x)*(rect_pos.br().x-rect_pos.tl().x))+rect_pos.tl().x;
        pos.y = output_image.rows - (((data[1][i] - params.ylim().x)/(params.ylim().y-params.ylim().x)*(rect_pos.br().y-rect_pos.tl().y))+rect_pos.tl().y);
        pointList.push_back(pos);
    }
    
    for( size_t i=0; i<pointList.size()-1; i++){
        line( output_image, pointList[i], pointList[i+1], Scalar(0, 0,255), 1 );
    }

}

}// end of cvplt Namespace
