#include "TACID.hpp"

#include "IO.hpp"

#include <algorithm>
#include <iostream>

using namespace std;


/**
 * Default constructor
*/
TACID::TACID( ){
    m_scene_number = 0;
    pathname = "";
    basename = "";
}

/**
 * Parameterized constructor
*/
TACID::TACID( string const& filename ){

    //set the pathname
    pathname = filename;

    //grab the base file name
    basename = file_basename( filename );

    //make sure the file is valid
    isValid = is_valid( filename );
    if( !isValid )
        return;

    //create a temporary variable to strip characters from
    string temp = basename;
    
    //pointers
    int len;

    //[0,7]- DATE (DDMMMYY) DD and YY are numeric, MMM is a string
    len=7;
    temp = temp.substr(len);
    
    //[7,2]- PROGRAM CODE: 9Z
    len = 2;
    temp = temp.substr(len);

    //[9,2]- SORTIE NUMBER: 00
    len = 2;
    temp = temp.substr(len);

    //[11,5]- SCENE NUMBER: 00001
    len = 5;
    m_scene_number = str2num<int>(temp.substr(0,len));
    temp = temp.substr(len);
    
    //[13,2]- PRODUCER CODE: ZX
    len = 2;
    temp = temp.substr(len);

    //[15,6]- PRODUCT NUMBER: GEO000
    len = 6;
    temp = temp.substr(len);

    //[21,2]- PROJECT CODE: GS
    len = 2;
    temp = temp.substr(len);

    //[23,3]- REPLAY CODE: 000
    len = 3;
    temp = temp.substr(len);
    
    //[26,3]- PRODUCER SERIAL NUMBER: 000
    len = 3;
    m_producer_serial_number = temp.substr(0,len); 
    temp = temp.substr(len);
    

    /*
		# Add the extension
		TACID += EXT

		# Create the file
		create_file( BASE_PATH + '/' + TACID )
        */

        
}


/**
 * Return the full pathname
*/
string TACID::get_pathname()const{
    return pathname;
}

/**
 * Return the scene number.
 *
*/
int TACID::scene_number( string const& filename, bool& isValid ){

    //set to false just in case
    isValid = false;

    //grab basename
    string basename = file_basename( filename ); 
    
    //make sure extension is nitf
    string ext = file_extension( basename );
    if( ext != ".ntf" && ext != ".nitf" )
        return -1;
    
    isValid = true;

    return  str2num<int>(basename.substr(11,5));
}

/**
 * Return the scene number
*/
int TACID::scene_number( )const{
    return m_scene_number;
}

/**
 * Return the producer serial number
*/
string TACID::producer_sn( )const{
    return m_producer_serial_number;
}


/** 
 * Non-static isValid validity checking function
*/
bool TACID::is_valid( )const{
    return isValid;
}

/**
 * Static validity checking flag. 
 * 
 * This checks the following conditions.
 * 1.  The extension is any of the following
 *     .nitf
 *     .ntf
 *     .NITF
 *     .NTF
 * 
 * 2.  The length of the file is 40 characters without the extension
*/
bool TACID::is_valid( const string& filename ){

    
    //set to false just in case
    bool isValid = true;

    //grab basename
    string basename = file_basename( filename ); 
    
    //make sure extension is nitf
    string ext = file_extension( basename );
    if( ext != ".ntf" && ext != ".nitf" )
        return false;

    return isValid;
}


/**
 * This is a function operator used in the TACID functor which returns
 * true if the first tacid is older in time than the second. 
*/
bool TACID_scene_func::operator( )( TACID const& a, TACID const& b){

    if( a.producer_sn() < b.producer_sn() )
        return true;
    
    if( a.producer_sn() > b.producer_sn() )
        return false;

    if( a.scene_number() < b.scene_number() ) 
        return true;

     return false;
}

/** 
 * Sort the list of images by their TACIDs
 *
 * This is a rather important function because it accomplishes 
 * several cleanup tasks while managing some difficult sorting tasks.
 *
 * Algorithm
 * - Remove all items which are not valid TACID entries
 * - Create a list of structures which contain the full path, the basename, and the scene number
 * - Sort that list using a functor and the sort function in stl library
*/
void sort_TACID_list( deque<string>& image_list ){
    
    //create a list of TACID containers
    deque<TACID> tacid_list;
    for( deque<string>::iterator it = image_list.begin(); it != image_list.end(); it++ ){

        //make sure the file is a valid TACID
        if( TACID::is_valid(*it) == true )
            tacid_list.push_back( TACID(*it));
    }

    //sort the image list
    sort( tacid_list.begin(), tacid_list.end(), TACID_scene_func() );

    //decompose the list back into strings
    image_list.clear();
    for( deque<TACID>::iterator it = tacid_list.begin(); it != tacid_list.end(); it++ )
        image_list.push_back( it->get_pathname() );

}


