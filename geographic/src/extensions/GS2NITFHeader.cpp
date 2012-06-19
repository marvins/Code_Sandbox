#include "GS2NITFHeader.h"

#include <iostream>
#include <string>

using namespace std;

namespace GEO{

    string months[] = {"NONE","JAN","FEB","MAR","APR","MAY","JUN","JUL","AUG","SEP","OCT","NOV","DEC"};
    
    TACID::TACID(){
        day = 0;
        month = 1900;
        year = 0;
        program_code[0] = '2';
        program_code[1] = 'Z';
        filename = "_NONE_SELECTED_";
    }

    TACID::TACID( std::string const& tacid ){

        day = 0;
        month = 0;
        year = 0;
        filename = tacid;
        cout << "filename for tacid: " << tacid << endl;
    }

    ostream& operator << ( ostream& ostr, const TACID& tacid ){

        ostr << "TACID: " << endl;
        ostr << "     Filename : " << tacid.filename << endl;
        ostr << "     Acq Date : " << tacid.day << months[tacid.month] << tacid.year << endl;
        ostr << "     Prog Code: " << tacid.program_code << endl;
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

