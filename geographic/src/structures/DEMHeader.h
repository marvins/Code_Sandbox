#ifndef __SRC_GEOGRAPHIC_DEM_HEADER_H__
#define __SRC_GEOGRAPHIC_DEM_HEADER_H__

#include <string>

#include "GeoHeader.h"

namespace GEO{

class DEMHeader_Info : public GeoHeader_Info{

   public:

      DEMHeader_Info();

      virtual ~DEMHeader_Info();

      void copy_header_info( GeoHeader_Info*  other );

      GeoHeader_Info*& clone()const;
      
      std::string get_driver_format()const;
      
   private:

       

};
}

#endif
