#ifndef __OPENCVPLOTTOOLS_PLOTDATA_HPP__
#define __OPENCVPLOTTOOLS_PLOTDATA_HPP__

#include <opencv2/core/core.hpp>

#include <vector>

using namespace cv;
using namespace std;

namespace cvplt{

/**
 * @class PlotParams
*/
class PlotParams{

    public:
        
        /**
         * Default Constructor
        */
        PlotParams();
        PlotParams( string const& plot_args );
        
        bool is_limit_set()const;

        void set_limits( vector<Point2d>const& lims );

        void load_defaults();
        
        Point2d  xlim()const;
        Point2d  ylim()const;
        
        Point2d get_limit( const int& idx )const;
    
        ///Plot Element Color
        Vec3b color;
        
        ///Limits
        vector<Point2d> limits;

        ///make sure we know when we have modified the limits
        bool limit_set;

        double line_width;
        
};

/**
 * @class ShapeData Object
*/
class PlotData{

    public:
        
        /** 
         * Constructor for PlotData
        */
        PlotData( vector<double>const& x, vector<double>const& y, PlotParams const& plot_params );

        void render( Mat& output_image, Rect const& rect_pos )const;

        std::vector<std::vector<double> > data;

        PlotParams params;

};
}

#endif
