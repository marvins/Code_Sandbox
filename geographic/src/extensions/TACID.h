#ifndef __SRC_GEOGRAPHIC_TACID_H__
#define __SRC_GEOGRAPHIC_TACID_H__

#include <iostream>
#include <string>

#ifdef DEPLOYED
#include "NITFHeader.h"
#else
#include <NITFHeader.h>
#endif

namespace GEO{
namespace GS2{

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
        
            /** 
             * Return TACID as a complete string
            */
            std::string toString()const;
            
            /**
             * Check to see if a possible TACID is valid 
             *
             * @param[in] tacid TACID to test
             * 
             * @return True if TACID is valid, false otherwise
            */
            static bool isValidTACID( const std::string& tacid );


            /** 
             * Output stream operator
             *
             * ostr   Output stream
             * tacid  TACID class
             */
            friend std::ostream& operator << ( std::ostream& ostr, const TACID& tacid );

        private:
            std::string day;                /*< Day of the month  */
            std::string month;              /*< Month of the year */
            std::string year;               /*< Year              */
            std::string scene_number;       /*< Scene Number      */
            std::string program_code;       /*< Program Code      */
            std::string filename;           /*< Filename of image */
            std::string sortie_number;      /*< Sortie Number     */
            std::string producer_code;      /*< Producer Code     */
            std::string product_number;     /*< Product Number    */
            std::string project_code;       /*< Project Code      */
            std::string replay_code;        /*< Replay Code       */
            std::string producer_sn;        /*< Producer SN       */
            std::string production_datim;   /*< Production DATIM  */

    };


} // end of GS2 namespace 
} // end of geo namespace 


#endif
