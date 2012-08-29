#ifndef __SRC_NITF_HEADER_H__
#define __SRC_NITF_HEADER_H__

#include <opencv2/core/core.hpp>

#include <iostream>
#include <string>

#include "GDAL_Data.h"

#include "NITFHeader.h"
#include "DTEDHeader.h"
#include "SRTMHeader.h"
#include "GeoHeader.h"
#include "GDAL_Data.h"
#include "PixelType.h"
#include "CoordinateLatLon.h"

namespace GEO{

/**
 * Geographic Image Type
 *
 * @brief Stores and manipulates geographic images
*/
class GeoImage{
    
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
    
    /** Clean out the geoimage and start from scratch */
    void clean();

    std::string getImageTypeName()const;

    cv::Mat  get_image();
    void     set_image( const cv::Mat& img );

    /** Get the size of the opencv image */
    cv::Size getMatSize()const;

    /** Return true if the image is valid with OpenCV */
    bool isOpenCVValid()const;

    /** Return true if the gdal was loaded */
    bool gdal_load()const;

    /** Get the maximum pixel value (Grayscale 64FC1 image only) */
    double getMin()const;

    /** Get the minimum pixel value (Grayscale 64FC1 image only) */
    double getMax()const;

    /** Write the GeoImage to file */
    void write_image( const std::string& imgFilename = "__NONE__" );
    
    /** Return a copy of the header data */
    GeoHeader_Info*& get_header()const;

    /** Modify the header of the image 
     *  ACTION PARAMETERS
     *  1.  SEARCH FOR TAG AND REPLACE VALUE
     *  2.  SEARCH FOR VALUE AND REPLACE TAG
     *  3.  ADD TAG AND VALUE PAIR
     *  4.  DELETE FIRST ENTRY WHICH MATCHES TAG
     *  5.  DELETE FIRST ENTRY WHICH MATCHES VALUE
    */
    void modify_header_metadata( const std::string& tag, const std::string& val, const int& action );
    
    void get_corner_coordinates( cv::Point2f& ul, cv::Point2f& br ) const;
    void get_corner_coordinates( CoordinateLatLon& ul, CoordinateLatLon& br );
    
    std::string get_tre()const;

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

    /**
     *  Retrieve the type of file a particular string is
     */
    int getFileType( const std::string& fname );


};

cv::Point2f getElevationMeters( cv::Point2f coordinate );

} //end of GEO namespace 

#endif
