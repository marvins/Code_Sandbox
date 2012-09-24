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

        ///Plot Element Color
        Vec3b color;
        
};

/**
 * @class ShapeData Object
*/
class PlotData{

    public:
        
        void render( Mat& output_image, Rect const& rect_pos )const;
        
        std::vector<std::vector<double> > data;

        PlotParams params;

};
}

#endif
