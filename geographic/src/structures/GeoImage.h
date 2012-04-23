#ifndef __SRC_NITF_HEADER_H__
#define __SRC_NITF_HEADER_H__

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <iostream>
#include <string>

#include "GDAL_Data.h"

#include "GeoHeader.h"
#include "NITFHeader.h"

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
    GeoImage(const std::string& fname, const bool& init = false );

    /** Copy Constructor */
    GeoImage( const GeoImage& rhs );

    /** Destructor */
    ~GeoImage();

    /** Assignment operator (DEEP COPY) */
    GeoImage& operator = (const GeoImage& rhs );

    /** Assign a new image filename */
    void set_filename( const std::string& fname );

    /** Get the current image filename */
    std::string get_filename( )const;

    /** Get initialization status */
    bool get_init()const;

    /** Set initialization status */
    void set_init( const bool& val );

    /** Initialize and load image */
    void init();

    std::string getImageTypeName()const;

    cv::Mat get_image();

    /** Get the size of the opencv image */
    cv::Size getMatSize()const;

    /** Return true if the image is valid with OpenCV */
    bool isOpenCVValid()const;

    bool gdal_load()const;

    /** Get the maximum pixel value (Grayscale 64FC1 image only) */
    double getMin()const;

    /** Get the minimum pixel value (Grayscale 64FC1 image only) */
    double getMax()const;

    /** Write the GeoImage to file */
    void write_image( const std::string& imgFilename = "__NONE__" );

    private:

    /** Load The Image Into Memory
     *
     * @brief if you are using GDAL, then it will open, do sanity
     * checking, then load the image dataset.
     */
    void load_image();


    /** Merge multiple gdal bands into a coherent RGB Image
    */
    cv::Mat  merge_bands( std::vector<cv::Mat>const& imgStack, std::vector<int> colors, std::vector<int> depths )const;


    /**
     * Image Metadata 
     */
    GeoHeader_Info* header_data; /*< Container for image header and meta-data */

    /** 
     * GDAL Container
     */
    GDAL_Data gdal_data;

    /**
     * Sanity Flags 
     */
    bool initialize;             /*< Flag for whether or not to load data */
    bool openCVCompatible;       /*< Flag for whether or not the image is valid with OpenCV */


};

#endif
