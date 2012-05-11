/* 
 * File:   PixelType.h
 * Author: ms6401
 *
 * Created on April 25, 2012, 10:13 AM
 */

#ifndef __PIXELTYPE_H__
#define	__PIXELTYPE_H__

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <string>

#include "gdal_priv.h"
#include "cpl_conv.h"


class PixelType {
public:
    
    static const int UNKNOWN;
    static const int UInt8C1;
    static const int UInt16C1;
    static const int UInt32C1;

    PixelType();
    PixelType( const int& tp );

    GDALDataType get_gdal_type()const;
    
    std::string get_name()const;
    
    int convert( cv::Mat const& image, const int x, const int y, const int c);
    
    void set( const int& tp );

    int  get( )const;

private:
     int pixeltype;

};


#endif	/* PIXELTYPE_H */

