#include "TACID.h"

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
        day = "00";
        month = "NON";
        year = "00";
        program_code = "2Z";
        filename = "_NONE_SELECTED_";
        sortie_number = "00";
    }

    /** TACID Parameterized Constructor */
    TACID::TACID( std::string const& tacid ){
        
        //set the filename
        filename = tacid;
        
        //set the day (Acquisition Date)
        day   = tacid.substr(0,2);
        
        //set the month (Acquisition Date)
        month = tacid.substr(2,3);
        
        //set the year (Acquisition Date)
        year = tacid.substr(5,2);
        
        //set the Program Code
        program_code = tacid.substr(7,2);

        //set the sortie number
        sortie_number = tacid.substr(9,2);
        
        //set the scene number
        scene_number  = tacid.substr(11, 5);
        
        //set DoD producer code
		producer_code = tacid.substr(16, 2);

		//set product number
		product_number = tacid.substr(18, 6);
		
		//set the NGA Project Code
		project_code = tacid.substr( 24, 2);

		//set the replay (reprocessed or retransmitted state flag)
		replay_code = tacid.substr(27, 3);
		
        //set the producer serial number
		producer_sn = tacid.substr(30, 3);
	
		//set the production datim
		production_datim = tacid.substr( 33, 8);
    
    }

    ostream& operator << ( ostream& ostr, const TACID& tacid ){

        ostr << "TACID: " << endl;
        ostr << "     Filename        : " << tacid.filename << endl;
        ostr << "     Acq Date        : " << tacid.day << tacid.month << tacid.year << endl;
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


}// end of GEO Namespace

