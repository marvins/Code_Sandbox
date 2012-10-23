#ifndef __SRC_GEOGRAPHIC_TACID_H__
#define __SRC_GEOGRAPHIC_TACID_H__

#include <iostream>
#include <string>
#include <utility>
#include <vector>

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
             *
             * @return tacid filename
            */
            std::string toString()const;
            
            /**
             * Return the modified input filename 
             *
             * @return tacid output filename
            */
            std::string toFilename()const;

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
            

            /**
             * Get the day (Acquisition Date)
             *
             * @return day
            */
            int getDay()const;

            /**
             * Set the day (Acquisition Date)
             *
             * @param[in] day
            */
            void setDay( const int& day );

            /**
             * Set the month (Acquisition Month)
             *
             * @param[in] month
            */
            void setMonth( const int& month );

            /**
             * Set the base path
             *
             * @param[in] basepath
            */
            void setBasepath( const std::string& path );
            
            /**
             * Parse the TACID data into key=value pairs for output
            */
            std::vector<std::pair<std::string,std::string> >  query_TACID_data()const;
            
            /**
             * Get the product number
            */
            std::string get_product_no( )const;

        private:
            std::string extension;          /*< Extension          */
            std::string dirname;            /*< Original Directory */
            std::string day;                /*< Day of the month   */
            std::string month;              /*< Month of the year  */
            std::string year;               /*< Year               */
            std::string scene_number;       /*< Scene Number       */
            std::string program_code;       /*< Program Code       */
            std::string filename;           /*< Filename of image  */
            std::string sortie_number;      /*< Sortie Number      */
            std::string producer_code;      /*< Producer Code      */
            std::string product_number;     /*< Product Number     */
            std::string project_code;       /*< Project Code       */
            std::string replay_code;        /*< Replay Code        */
            std::string producer_sn;        /*< Producer SN        */
            std::string production_datim;   /*< Production DATIM   */

    };


} // end of GS2 namespace 
} // end of geo namespace 


#endif
