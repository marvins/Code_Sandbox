#ifndef __SRC_NITF_HEADER_H__
#define __SRC_NITF_HEADER_H__

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <iostream>
#include <string>

#include "gdal_priv.h"
#include "cpl_conv.h"

#include "../../../opencv/OpenCVUtils.h"
#include "../utilities/GDAL2OpenCV.h"


using namespace cv;
using namespace std;

class GeoImage{

    public:

        GeoImage();
        GeoImage(const string& fname, const bool& init = false );

        void set_filename( const string& fname );
        string get_filename( )const;
   
        bool get_init()const;
        void set_init( const bool& val );

        void init();

        GDALDriver* getDriver()const;

        Mat get_image();
        Size getMatSize()const;

        bool isOpenCVValid()const;
        
        bool gdal_load()const;

        double getMin()const;
        double getMax()const;

    private:

        void load_image();
        Mat  merge_bands( vector<Mat>const& imgStack, vector<int> colors, vector<int> depths )const;

        string filename;
        bool initialize;

        GDALDriver  *driver;
        GDALDataset *poDataset;
        double adfGeoTransform[6];

        bool gdalLoadFailed;
        bool openCVCompat;
        
        double adfMinMax[2];
        double magScale;
};

#endif
