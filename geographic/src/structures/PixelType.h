/* 
 * File:   PixelType.h
 * Author: ms6401
 *
 * Created on April 25, 2012, 10:13 AM
 */

#ifndef PIXELTYPE_H
#define	PIXELTYPE_H

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <string>

#include "gdal_priv.h"
#include "cpl_conv.h"

class PixelType {
public:
    
    static const int UNKNOWN  = 0;
    static const int UInt8C1  = 1;
    static const int UInt16C1 = 2;
    static const int UInt32C1 = 3;

    GDALDataType get_gdal_type()const;
    
    std::string get_name()const;
    
    int convert( cv::Mat const& image, const int x, const int y, const int c);
    
    void set( const int& tp );

    int  get( )const;

private:
     int pixeltype;

};

#endif	/* PIXELTYPE_H */

