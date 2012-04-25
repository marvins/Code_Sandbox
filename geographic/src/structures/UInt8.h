/* 
 * File:   UInt8.h
 * Author: ms6401
 *
 * Created on April 25, 2012, 1:36 PM
 */

#ifndef UINT8_H
#define	UINT8_H

#include <string>

#include "gdal_priv.h"
#include "cpl_conv.h"

#include "PixelType.h"

class UInt8 : public PixelType {
public:
    
    PixelType*& clone()const;
    
    GDALDataType get_gdal_type()const;
    
    std::string get_name()const;
    
    int convert( cv::Mat const& image, const int x, const int y);
        
private:
    std::string name;

};


#endif	/* UINT8_H */

