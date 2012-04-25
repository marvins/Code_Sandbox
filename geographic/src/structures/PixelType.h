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
    
    virtual PixelType*& clone()const = 0;
    
    virtual GDALDataType get_gdal_type()const = 0;
    
    virtual std::string get_name()const = 0;
    
    virtual int convert( cv::Mat const& image, const int x, const int y) = 0;
    
private:
     

};

#endif	/* PIXELTYPE_H */

