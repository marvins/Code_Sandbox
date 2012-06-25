#include "GS2NITFHeader.h"

#include <iostream>
#include <sstream>
#include <string>

using namespace std;

static int str2int( string const& input ){
    int out;
    stringstream(input) >> out;
    return out;
}

namespace GEO{

    string months[] = {"NONE","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
   
    /** TACID Default Constructor */
    TACID::TACID(){
        day = 0;
        month = 1900;
        year = 0;
        program_code = "2Z";
        filename = "_NONE_SELECTED_";
    }

    /** TACID Parameterized Constructor */
    TACID::TACID( std::string const& tacid ){
        
        //set the filename
        filename = tacid;
        
        //set the day (Acquisition Date)
        day   = str2int( tacid.substr(0,2));
        
        //set the month (Acquisition Date)
        month = 0;
        for( size_t i=1; i<13; i++ )
            if( tacid.substr(2,3) == months[i] )
                month = i;
        
        //set the year (Acquisition Date)
        year = 2000 + str2int( tacid.substr(5,2));
        
        //set the Program Code
        program_code = tacid.substr(7,2);
        
    }

    ostream& operator << ( ostream& ostr, const TACID& tacid ){

        ostr << "TACID: " << endl;
        ostr << "     Filename        : " << tacid.filename << endl;
        ostr << "     Acq Date        : " << tacid.day << months[tacid.month] << tacid.year << endl;
        ostr << "     Prog Code       : " << tacid.program_code << endl;
        ostr << "     Sortie Number   : " << tacid.sortie_number << endl;
        ostr << "     Scene Number    : " << tacid.scene_number << endl;
        ostr << "     Producer Code   : " << tacid.producer_code << endl;
        ostr << "     Product Number  : " << tacid.product_number << endl;
        ostr << "     Project Code    : " << tacid.project_code << endl;
        ostr << "     Replay Code     : " << tacid.replay_code << endl;
        ostr << "     Producer SN     : " << tacid.producer_sn << endl;
        ostr << "     Production DATIM: " << tacid.production_datim << endl;
        return ostr;
    }


    GS2NITFHeader_Info::GS2NITFHeader_Info(){

        image_filename = "_NO_IMAGE_SELECTED_";    
       
        cout << "HERE" << endl;

    }

    GS2NITFHeader_Info::GS2NITFHeader_Info( NITFHeader_Info const& header ){
        
        m_TACID = TACID( header.get_image_filename());

    }

    GS2NITFHeader_Info::~GS2NITFHeader_Info(){ }

    void GS2NITFHeader_Info::copy_header_info( GeoHeader_Info*  other ){

        image_filename = other->get_image_filename();

        setValid( other->isValid() );

        //move over header data
        set_header_data( other->get_header_data());

    }

    GeoHeader_Info*& GS2NITFHeader_Info::clone() const{

        GeoHeader_Info* output = new GS2NITFHeader_Info();

        //set valid flag
        output->setValid( isValid() );

        //move over pixel type
        output->set_pixel_type( get_pixel_type());

        //move over image name
        output->set_image_filename(get_image_filename());

        //move over header data
        output->set_header_data( get_header_data());

        return output;
    }


    std::string GS2NITFHeader_Info::get_driver_format() const{
        return "IMG.NITF.GS2";
    }

    TACID GS2NITFHeader_Info::getTACID()const{
        cout << "getTACID: " << m_TACID.filename << endl;
        return m_TACID;
    }

    void GS2NITFHeader_Info::setTACID( const TACID& tacid ){
        m_TACID = tacid;
    }

}// end of GEO Namespace

