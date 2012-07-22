#include "geo_forms.h"
#include "utils.h"

#include <string> 

#include <ncurses.h>

using namespace std;

geo_header_item::geo_header_item( const int& idx, const string& type, const string& val ){
    header_type = idx;
    header_tag  = type;
    header_val  = val;
}

void print_header( std::string const& input_message, const int& start_y, const int& width, const int& height ){

    int start_x_pos = (width/2) - ((input_message.size())/2);

    mvprintw( start_y, start_x_pos, input_message.c_str()); 

}

void print_tag( const string& data, const int& window_start, const int& length, const int& row ){
    
    //compute text start
    int text_start = 0;
    if( data.size() <= length )
        text_start = length - data.size();
    else
        text_start = 0;

    //move cursor to start of field
    move( row, window_start );
    
    //print until length is reached
    for( size_t i=0; i<length; i++){
        if( i < text_start )
            addch(' ');
        else
            addch(data[i-text_start]);
    }
}

void print_field( const string& data, const int& window_start, const int& length, const int& row ){
    
    //move cursor to start of field
    move( row, window_start );

    //print until length is reached
    for( size_t i=0; i<length; i++){
        if( i < data.size() )
            addch(data[i]);
        else
            addch(' ');
    }
}


/**
 *  Find the largest width of the headers to ensure that we select sizes for the field parameters which maximize
 *  space usage.
*/
void find_max_header_widths( vector<geo_header_item> const& header_metadata, int& tag_width_max, int& val_width_max ){

    //set defaults
    tag_width_max = 1;
    val_width_max = 1;
    
    //just in case we get an empty list
    if( header_metadata.size() <= 0 ){  return;  }
    
    //iterate through the list
    for( size_t i=0; i<header_metadata.size(); i++ ){
        
        //compare the tag item
        if( header_metadata[i].header_tag.size() > tag_width_max ) 
            tag_width_max = header_metadata[i].header_tag.size();
        
        //compare the value item
        if( header_metadata[i].header_val.size() > val_width_max ) 
            val_width_max = header_metadata[i].header_val.size();
    }

}


void print_metadata( vector<geo_header_item>const& header_metadata, const int& start_y, 
        const int& con_size_x, const int& con_size_y, const int& cursor_pos, int& window_top  ){

    int idx;
    int WINDOW_GAP = 4;
    int BORDER_WIDTH = 4;
    int WINDOW_HEIGHT  = ( con_size_y - start_y );
    int WINDOW_Y_START = BORDER_WIDTH + start_y;

    //compute the max widths of both the tag/value pairs
    int tag_width_max, val_width_max;
    find_max_header_widths( header_metadata, tag_width_max, val_width_max );

    //use this to compute the start of the windows
    tag_width_max += 2; //add two for buffering
    val_width_max += 2; //add two for buffering

    size_t TAG_X_START = BORDER_WIDTH;
    size_t VAL_X_START = min( (int)(TAG_X_START + tag_width_max), (int)(con_size_x/2)) + (WINDOW_GAP/2);

    size_t TAG_WIDTH   = VAL_X_START - TAG_X_START - WINDOW_GAP;
    size_t VAL_WIDTH   = con_size_x - VAL_X_START - BORDER_WIDTH;

    /*
       begin printing out rows
     */
    attron( COLOR_PAIR( active_window_pair ) | A_BOLD );
    attroff( A_STANDOUT );

    for( size_t i=0; i<WINDOW_HEIGHT; i++ ){
        
        //compute the current index
        idx = window_top + i;
        if( idx >= header_metadata.size() )
            idx = idx - header_metadata.size();

        if( idx == cursor_pos ){
            mvprintw( start_y + i, 1, "->");
            attron( A_STANDOUT );
        }

        //iterate over the var variable, printing the data
        print_tag  ( header_metadata[idx].header_tag, TAG_X_START, TAG_WIDTH, start_y + i );
        print_field( header_metadata[idx].header_val, VAL_X_START, VAL_WIDTH, start_y + i );
        
        attroff( A_STANDOUT );
    
    }
    attroff( A_BOLD );
    attron ( COLOR_PAIR( backg_window_pair )  | A_NORMAL);

}


void print_footer( const int& con_width, const int& con_height ){

    attron( COLOR_PAIR( backg_window_pair ));
    mvprintw( con_height - 2, 2, "Press [up/down] or [,/.] - navigate,  [q/ESC] - quit immediately"); 
    mvprintw( con_height - 1, 2, "Press [c] - Modify Parameter");

}
