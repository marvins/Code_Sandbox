#ifndef __SRC_GEOGRAPHIC_NITF_HEADER_H__
#define __SRC_GEOGRAPHIC_NITF_HEADER_H__

#include <string>

#include "ImageHeader.h"

namespace GEO{

class GS2NITFHeader_Info : public NITFHeader_Info{

   public:

      GS2NITFHeader_Info();

      virtual ~GS2NITFHeader_Info();

      void copy_header_info( GeoHeader_Info*  other );

      GeoHeader_Info*& clone()const;
      
      std::string get_driver_format()const;
      
   private:

       

};
}

#endif
