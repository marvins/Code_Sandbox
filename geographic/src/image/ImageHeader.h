#ifndef __SRC_GEOGRAPHIC_IMAGE_HEADER_H__
#define __SRC_GEOGRAPHIC_IMAGE_HEADER_H__

#include <string>

#include "GeoHeader.h"

namespace GEO{

class ImageHeader_Info : public GeoHeader_Info{

   public:

      ImageHeader_Info();

      virtual ~ImageHeader_Info();

      void copy_header_info( GeoHeader_Info*  other );

      GeoHeader_Info*& clone()const;
      
      std::string get_driver_format()const;
      
   private:

       

};
}

#endif
