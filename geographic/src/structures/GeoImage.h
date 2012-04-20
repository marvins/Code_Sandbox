#ifndef __SRC_NITF_HEADER_H__
#define __SRC_NITF_HEADER_H__

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <iostream>
#include <string>


#include "../../../opencv/OpenCVUtils.h"
#include "../utilities/GDAL2OpenCV.h"


using namespace cv;
using namespace std;

/**
 * Geographic Image Type
 *
 * @brief Stores and manipulates geographic images
 *
*/
class GeoImage{
    
    /**
     * Enumeration useful for loading different image types. The
     * only planned type considered now is the NITF.
    */
    enum GEO_IMAGE_TYPE{
        NITF
    };

    public:

        /** Default constructor */
        GeoImage();

        /** Parameterized Constructor */
        GeoImage(const string& fname, const bool& init = true );
        
        /** Copy Constructor */
        GeoImage( const GeoImage& rhs );

        /** Destructor */
        ~GeoImage();

        /** Assignment operator (DEEP COPY) */
        GeoImage& operator = (const GeoImage& rhs );

        /** Assign a new image filename */
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

        void write_image( const string& imgFilename = "__NONE__" );

    private:

        /** Load The Image Into Memory
         *
         * @brief if you are using GDAL, then it will open, do sanity
         * checking, then load the image dataset.
        */
        void load_image();


        Mat  merge_bands( vector<Mat>const& imgStack, vector<int> colors, vector<int> depths )const;

        NITF_Header header_info;
        
        bool initialize;

        double adfGeoTransform[6];

        bool gdalLoadFailed;
        bool openCVCompat;
        
        double adfMinMax[2];

};

#endif
