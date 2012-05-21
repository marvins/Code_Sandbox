#ifndef __SRC_GEOGRAPHIC_DTED_HEADER_H__
#define __SRC_GEOGRAPHIC_DTED_HEADER_H__

#include <string>

#include "DEMHeader.h"

namespace GEO{

class DTEDHeader_Info : public DEMHeader_Info{

   public:

      DTEDHeader_Info();

      virtual ~DTEDHeader_Info();

      void copy_header_info( GeoHeader_Info*  other );

      GeoHeader_Info*& clone()const;
      
      std::string get_driver_format()const;
      
   private:

       

};
}

#endif
