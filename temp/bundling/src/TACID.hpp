#ifndef __SRC_TACID_HPP__
#define __SRC_TACID_HPP__

#include <deque>
#include <string>

using namespace std;


/**
 * A class which represents the GS2 TACID 
 *
 * Decomposes the filename into various parts. 
*/
class TACID{

    public:
        
        /**
         * Default Constructor
        */
        TACID( );

        /** 
         * Parameterized Constructor
        */
        TACID( string const& filename, const int& collectType );
         
        /**
         * Retrieve the full pathname
        */
        string get_pathname()const;

        /**
         * Return the scene number.
         *
         * This is a static function, so feel free to use it anywhere. 
        */
        static int scene_number( string const& filename, const int& scene_type, bool& isValid );
        
        /**
         * Return the scene number
        */
        int scene_number( )const;

        /**
         * Return the producer serial number
        */
        string producer_sn( )const;

        /**
         * Checks if a file is a valid TACID. 
         *
         * The static version is much better.  This is a safeguard in case you forget
         * to use that. The class will merely check its internal flag. It calls the 
         * static version. 
        */
        bool is_valid()const;

        /**
         * Checks if a file is a valid TACID.
         *
         * This is the preferred method before you create a list of TACIDs as you will
         * waste the construction and destruction of the TACID object. 
        */
        static bool is_valid( const string& filename, const int& collect_type );
        
        
        /**
         * Checks if both filenames have an equal time
        */
        static bool equal( const string& fileA, const string& fileB );

    private:

        bool   isValid;   // validity flag

        string pathname;  // full path
        string basename;  // base file name

        int m_scene_number; // scene number
        string m_producer_serial_number;
        
        int collect_type;

};

/** 
 * A basic functor useful for sorting TACIDs.
*/
class TACID_scene_func{

    public:
    
        bool operator( )( TACID const& a, TACID const& b);
};


/**
 * Sort the list of images by their TACIDs.  
 *
*/
void sort_TACID_list( deque<string>& image_list, const int& collect_type );

#endif

