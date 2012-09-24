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
        Vec3b color;

};

/**
 * @class ShapeData Object
*/
class PlotData{

    public:
        std::vector<std::vector<double> > data;

        PlotParams params;

};
}

#endif
