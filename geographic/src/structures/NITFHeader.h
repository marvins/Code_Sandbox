#ifndef __SRC_GEOGRAPHIC_NITF_HEADER_H__
#define __SRC_GEOGRAPHIC_NITF_HEADER_H__

#include "GeoHeader.h"

class NITFHeader_Info : public GeoHeader_Info{

   public:

      NITFHeader_Info();

      virtual ~NITFHeader_Info();

      void copy_header_info( GeoHeader_Info*  other );

   private:



};

#endif
