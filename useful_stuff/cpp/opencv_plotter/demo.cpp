#include <OpenCVPlotTools.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <vector>

using namespace cv;
using namespace std;

int main( int argc, char** argv ){
    
    double xval[] = {1,2,3,4,5};
    double yval[] = {2,2,2,2,2};

    vector<double> x( xval, xval+sizeof(xval)/sizeof(double));
    vector<double> y( yval, yval+sizeof(yval)/sizeof(double));

    //create a plot
    cvplt::Figure  fig;
    cvplt::PlotParams plotParams;

    //plot some data to the 
    fig.plot( x, y, plotParams);


    Mat result = fig.print();
    
    namedWindow("Result");
    imshow("Result", result);
    waitKey(0);

    return 0;
}
