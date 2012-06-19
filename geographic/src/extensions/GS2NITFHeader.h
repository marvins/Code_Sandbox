#ifndef __SRC_GEOGRAPHIC_GS2NITF_HEADER_H__
#define __SRC_GEOGRAPHIC_GS2NITF_HEADER_H__

#include <iostream>
#include <string>

#include <NITFHeader.h>


namespace GEO{

    class TACID{

        public:

            TACID( );
            TACID( std::string const& tacid );

            int day;
            int month;
            int year;

            char program_code[2];

            std::string filename;

    };

    std::ostream& operator << ( std::ostream& ostr, const TACID& tacid );

    class GS2NITFHeader_Info : public NITFHeader_Info {

        public:

            GS2NITFHeader_Info();
            GS2NITFHeader_Info( NITFHeader_Info const& header );

            virtual ~GS2NITFHeader_Info();

            void copy_header_info( GeoHeader_Info*  other );

            GeoHeader_Info*& clone()const;

            std::string get_driver_format()const;

            TACID getTACID()const;
            void setTACID( const TACID& tacid );

        private:

            TACID m_TACID;

    };
}

#endif
