#ifndef __SRC_GEOGRAPHIC_SRTM_HEADER_H__
#define __SRC_GEOGRAPHIC_SRTM_HEADER_H__

#include <string>

#include "DEMHeader.h"

namespace GEO{

class SRTMHeader_Info : public DEMHeader_Info{

   public:

      SRTMHeader_Info();

      virtual ~SRTMHeader_Info();

      void copy_header_info( GeoHeader_Info*  other );

      GeoHeader_Info*& clone()const;
      
      std::string get_driver_format()const;
      
   private:

       

};
}

#endif
