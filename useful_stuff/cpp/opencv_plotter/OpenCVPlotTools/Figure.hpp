#ifndef __OPENCVPLOTTOOLS_FIGURE_HPP__
#define __OPENCVPLOTTOOLS_FIGURE_HPP__


//OpenCV 
#include <opencv2/core/core.hpp>

//STL
#include <deque>
#include <vector>

//Plot Library Stuff
#include <OpenCVPlotTools/PlotData.hpp>

using namespace cv;
using namespace std;


///This is the namespace for the OpenCVPlotTools Library
namespace cvplt{

/**
 * Figure Params
*/
class FigureParams{
    
    public:
        
        FigureParams();
        
        //size of the output image
        cv::Size figure_size;

        //opencv image type of figure
        int figure_cv_type;

        //background color
        Vec3b background_color;
        Vec3b plot_background_color;

        //plot window percentage
        double figure_use_percentage;
        
        //Font data
        string figure_title;
        int figure_title_fontFace;
        double figure_title_fontScale;
        int figure_title_thickness;
        int figure_title_lineType;

};

/**
 * @class Figure
 * 
 * An opencv figure. Stores figure data until
 * ready to plot.
*/
class Figure{
    
    public:
    
        //plot some data to the figure
        void plot( vector<double> const& x, vector<double> const& y, PlotParams const& params );
        
        //show the data
        cv::Mat print()const;

    private:
        
        //this is the list of shapes that we are saving to be printed
        deque<PlotData> plot_data;
        
        //this is the figure parameters
        FigureParams params;
};



} //namespace cvplt


#endif
