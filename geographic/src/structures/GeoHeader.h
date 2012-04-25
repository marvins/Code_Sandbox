#ifndef __GEO_HEADER_H__
#define __GEO_HEADER_H__

#include <string>

#include "PixelType.h"

class GeoHeader_Info{

   public:
      GeoHeader_Info();

      virtual ~GeoHeader_Info();

      std::string get_image_filename()const;

      void set_image_filename( std::string const& filename );

      virtual void copy_header_info(GeoHeader_Info* other ) = 0;

      bool image_filename_exists()const;
      
      void set_pixel_type( PixelType* pix);
      
      PixelType*& get_pixel_type()const;
      
      virtual GeoHeader_Info*& clone()const = 0;
      
      virtual std::string get_driver_format()const = 0;
      

   protected:

      std::string image_filename;

      PixelType* pixeltype;

};


#endif
