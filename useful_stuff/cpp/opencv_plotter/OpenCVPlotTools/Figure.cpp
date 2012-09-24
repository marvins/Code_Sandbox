#include "Figure.hpp"

#include <iostream>

namespace cvplt{

FigureParams::FigureParams( ){
    figure_size = Size(1000,1000);
    figure_cv_type = CV_8UC3;
    
    background_color = Vec3b( 100, 100, 100);
    plot_background_color = Vec3b(255,255,255);
    
    figure_use_percentage = 0.8;

    figure_title           = "Figure 1";
    figure_title_fontFace  = FONT_HERSHEY_SCRIPT_SIMPLEX;
    figure_title_fontScale = 2;
    figure_title_thickness = 1;
    figure_title_lineType  = 8;

}


void Figure::plot( vector<double> const& x, vector<double> const& y, PlotParams const& plot_params ){
    
    //save data as a line item
    PlotData object( x, y, plot_params );
    
    //add object to plot data list
    plot_data.push_back( object );
    
}


Mat Figure::print()const{
    
    //create an output image
    Mat output_image( params.figure_size, params.figure_cv_type );  
    
    //set the background color
    output_image = Scalar(params.background_color);
    
    //compute the size and position of the figure
    Size plot_size( params.figure_size.width  * params.figure_use_percentage, 
                    params.figure_size.height * params.figure_use_percentage );

    Rect rect_pos( (params.figure_size.width  - plot_size.width )/2.0,
                   (params.figure_size.height - plot_size.height)/2.0,
                   plot_size.width, plot_size.height);

    //print the title
    int baseline=0;
    Size textSize = getTextSize( params.figure_title,            params.figure_title_fontFace, 
                                 params.figure_title_fontScale,  params.figure_title_thickness, 
                                 &baseline );

    Point title_center( (output_image.cols/2.0)-(textSize.width/2.0), (rect_pos.y/2) );
    putText( output_image, "Figure 1", title_center, FONT_HERSHEY_SIMPLEX, 2, Scalar::all(50), 3, 8 );
    
    //print the figure background
    for( int c=rect_pos.tl().x; c<rect_pos.br().x; c++ )
    for( int r=rect_pos.tl().y; r<rect_pos.br().y; r++ )
        output_image.at<Vec3b>(r,c) = params.plot_background_color;
    
    //for each plot in plot data, draw them onto the image
    for( size_t plt=0; plt < plot_data.size(); plt++ )
        plot_data[plt].render( output_image, rect_pos );


    return output_image;
}


} //end of cvp Namespace
