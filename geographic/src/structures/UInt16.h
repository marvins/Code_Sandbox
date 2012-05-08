/* 
 * File:   UInt16.h
 * Author: ms6401
 *
 * Created on April 25, 2012, 10:13 AM
 */

#ifndef UINT16_H
#define	UINT16_H

#include <string>

#include "gdal_priv.h"
#include "cpl_conv.h"

#include <PixelType.h>

class UInt16 : public PixelType {
public:
    
    PixelType*& clone()const;
    
    GDALDataType get_gdal_type()const;
    
    std::string get_name()const;
    
    int convert( cv::Mat const& image, const int x, const int y);
        
private:
    std::string name;

};

#endif	/* UINT16_H */

