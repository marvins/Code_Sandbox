#ifndef __SRC_GEOGRAPHIC_GS2NITF_HEADER_H__
#define __SRC_GEOGRAPHIC_GS2NITF_HEADER_H__

#include <iostream>
#include <string>

#include <NITFHeader.h>


namespace GEO{
    
    /**
     * @class TACID
     *
     * Abstraction for the GS2 TACID designator.
    */
    class TACID{

        public:
            
            /**
             * TACID Default Constructor
            */
            TACID( );

            /**
             *  TACID Parameterized Constructor
             *
             * @param[in] tacid input string
            */
            TACID( std::string const& tacid );
        
        private:
            int day;                /*< Day of the month  */
            int month;              /*< Month of the year */
            int year;               /*< Year              */

            std::string program_code;    /*< Program Code      */
            std::string filename;   /*< Filename of image */

    };

    /** 
      * Output stream operator
      *
      * ostr   Output stream
      * tacid  TACID class
    */
    std::ostream& operator << ( std::ostream& ostr, const TACID& tacid );

    /**
     *  @class GS2NITFHeader_Info
     *
     *  GS2 NITF Image Header
    */
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
