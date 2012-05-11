#ifndef __SRC_GEOGRAPHIC_NITF_HEADER_H__
#define __SRC_GEOGRAPHIC_NITF_HEADER_H__

#include <string>

#include "GeoHeader.h"

class NITFHeader_Info : public GeoHeader_Info{

   public:

      NITFHeader_Info();

      virtual ~NITFHeader_Info();

      void copy_header_info( GeoHeader_Info*  other );

      GeoHeader_Info*& clone()const;
      
      std::string get_driver_format()const;
      
   private:

       

};

#endif
