#include "geo_forms.h"
#include "utils.h"

#include <boost/filesystem.hpp>

#include <string> 

#include <ncurses.h>

using namespace std;
    
namespace fs = boost::filesystem;
    
const int modify_offset = 5;
const string tag_hdr = "Tag: ";
const string val_hdr = "Val: ";

geo_header_item::geo_header_item( ){
    header_type = -1;
    header_tag  = "";
    header_val  = "";
}

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
    int WINDOW_HEIGHT  = std::min( con_size_y - start_y, (int)header_metadata.size() );
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

        //skip the space for the gap
        if( header_metadata[idx].header_type == META_GAP ){
            attron ( COLOR_PAIR( backg_window_pair )  | A_NORMAL);
        }
        //use the proper color if its a header
        else if( header_metadata[idx].header_type == META_INFORMATIONAL ){
            attron ( COLOR_PAIR( backg_window_pair )  | A_NORMAL);
        }
        else if( idx == cursor_pos ){
            mvprintw( start_y + i, 1, "->");
            attron( A_STANDOUT );
        }

        //iterate over the var variable, printing the data
        print_tag  ( header_metadata[idx].header_tag, TAG_X_START, TAG_WIDTH, start_y + i );
        print_field( header_metadata[idx].header_val, VAL_X_START, VAL_WIDTH, start_y + i );

        attroff( A_STANDOUT );
        
        //skip the space for the gap
        if( header_metadata[idx].header_type == META_GAP || 
            header_metadata[idx].header_type == META_INFORMATIONAL ){
            attroff ( COLOR_PAIR( backg_window_pair )  | A_NORMAL);
        }
    }
    attroff( A_BOLD );
    attron ( COLOR_PAIR( backg_window_pair )  | A_NORMAL);

}


void print_footer( const int& con_width, const int& con_height ){

    attron( COLOR_PAIR( backg_window_pair ));
    mvprintw( con_height - 2, 2, "Press [up/down] or [,/.] - navigate,  [q/ESC] - quit immediately"); 
    mvprintw( con_height - 1, 2, "Press [c] - Modify Parameter,  [s] - Save Current Image,  [L] - Load New Image");

}


/** Clear the bottom 4 lines of the ui */
void clear_footer( const int& width, const int& height ){

    for( size_t i=0; i<width; i++)
        for( size_t j=(height-5); j<height; j++)
            mvaddch(j,i,' ');

}

/** Print item to screen */
void print_line( string const& tag, string const& val, const int& width, const int& height, const int cx, const int cy ){

    //
    int cursor_line = height - 4;

    int curs_y = cy + (cursor_line);
    int curs_x = tag_hdr.size() + modify_offset + cx;

    mvprintw( height-1, modify_offset+5, "Options:  [ESC]: exit, [ENTER]: save and exit");
    mvprintw( cursor_line+0, modify_offset, tag_hdr.c_str());  mvprintw( cursor_line+0, modify_offset + 5, tag.c_str());
    mvprintw( cursor_line+1, modify_offset, val_hdr.c_str());  mvprintw( cursor_line+1, modify_offset + 5, val.c_str());
    move( curs_y, curs_x);
    

}


/**
 * Change the value of a geo header item
*/
bool change_screen( geo_header_item& item, const int& width, const int& height ){

    int input1;
    int input2;
    bool exit_modify = false;

    int cursor_x = 0;
    int cursor_y = 0;
    string temp_tag = item.header_tag;
    string temp_val = item.header_val;

    /*  Print the change screen ui */
    clear_footer( width, height );


    while( exit_modify == false ){

        //print the line to the screen
        print_line( temp_tag, temp_val, width, height, cursor_x, cursor_y );

        refresh(); //refresh the screen

        //pull the character
        input1 = getch();

        switch( input1 ){

            //escape key 
            case 27:

                //pull the secondary character
                input2 = getch();
                if( input2 != ERR){  // You have alt key

                }
                else{ // You have escape key
                    exit_modify = true;
                }
                break;
            
            //enter key
            case 10:
                item.header_tag = temp_tag;
                item.header_val = temp_val;
                return true;
                break;

            //backspace
            case 127:
            case 263:
                if(      cursor_y == 0 && cursor_x > 0 ){ 
                    temp_tag.erase( --cursor_x, 1); 
                    clear_footer( width, height);
                } 
                else if( cursor_y == 1 && cursor_x > 0 ){ 
                    temp_val.erase( --cursor_x, 1); 
                    clear_footer( width, height);
                }
                break;

            //key_up
            case KEY_UP:
                if(      cursor_y == 0 && cursor_x <= temp_val.size() ) cursor_y = 1;
                else if( cursor_y == 1 && cursor_x <= temp_tag.size() ) cursor_y = 0;
                break;

            case KEY_DOWN:
                if(      cursor_y == 0 && cursor_x <= temp_val.size() ) cursor_y = 1;
                else if( cursor_y == 1 && cursor_x <= temp_tag.size() ) cursor_y = 0;
                break;

            case KEY_LEFT:

                if( cursor_x > 0 )
                    cursor_x--;
                break;

            case KEY_RIGHT:
                if( cursor_y == 0 && cursor_x < temp_tag.size() ) cursor_x++;
                if( cursor_y == 1 && cursor_x < temp_val.size() ) cursor_x++;
                break;

            /** Default behavior will be to insert a character */
            default:

                if( ( input1 >= 'a' && input1 <= 'z' ) ||
                    ( input1 >= 'A' && input1 <= 'Z' ) ||
                    ( input1 >= '0' && input1 <= '9' ) ||
                    ( input1 == ' ' || input1 == '.' ) ||
                    ( input1 == ',' || input1 == '/' )   ){
                    if( cursor_y == 0 ){ 
                        temp_tag.insert( cursor_x, 1, (char)input1 ); 
                        cursor_x++;
                    }
                    if( cursor_y == 1 ){ 
                        temp_val.insert( cursor_x, 1, (char)input1 ); 
                        cursor_x++;
                    }
                }
                break;


        }

    }
    return false;
}


/****************************************************************/
/*                  Save GeoImage Details                       */
/****************************************************************/
int save_screen( const int& width, const int& height ){

    int input1;
    int input2;
    bool exit_modify = false;

    string line1 = "Would you like to save to";
    string line2 = "the current file or write";
    string line3 = "     a new file?         ";
    string line4 = "                         ";
    string line5 = "[N]: new  [C]: current   ";
    string line6 = "[any other key to cancel]";

    int xmin = (width/2)  - std::min((width/8),  (int)line1.size()/2);
    int xmax = (width/2)  + std::min((width/8),  (int)line1.size()/2);
    int ymin = (height/2) - std::min((height/8), 3);
    int ymax = (height/2) + std::min((height/8), 3);

    while( exit_modify == false ){

        //print the line to the screen
        for( size_t i=xmin-2; i<=xmax+2; i++) 
        for( size_t j=ymin-2; j<=ymax+2; j++) 
            mvaddch(j,i,' ');
        
        attron( COLOR_PAIR(13) | A_BOLD ); 
        mvprintw(ymin+0, xmin, line1.c_str());
        mvprintw(ymin+1, xmin, line2.c_str());
        mvprintw(ymin+2, xmin, line3.c_str());
        mvprintw(ymin+3, xmin, line4.c_str());
        mvprintw(ymin+4, xmin, line5.c_str());
        mvprintw(ymin+5, xmin, line6.c_str());
        attron( COLOR_PAIR(11) | A_BOLD ); 

        refresh(); //refresh the screen

        //pull the character
        input1 = getchar();

        switch( input1 ){

            case 'N':
            case 'n':
                throw string("ERROR: feature not supported");
                return 2;
                break;

            case 'C':
            case 'c':
                return 1;


            /** Default behavior will be to insert a character */
            default:
                return 0;
                break;


        }

    }
    return false;
}


vector<fs::path>  get_directory_contents( const fs::path& pth ){

    vector<fs::path> output;
    fs::directory_iterator end_iter;

    if( fs::exists( pth ) && fs::is_directory( pth ) ){

        for( fs::directory_iterator dir_iter(pth) ; dir_iter != end_iter ; ++dir_iter)
        {
            output.push_back( *dir_iter );
        }

        sort( output.begin(), output.end() );
    }
    return output;
}


/***********************************************************/
/*                    Load Image Screen                    */
/***********************************************************/
string load_screen(  ){

    string output = "";
    int con_size_x, con_size_y;
    const int MAIN_MENU_TIMEOUT  = 100;
    const int program_header_row = 0;
    const int menu_header_row    = 2;
    const int meta_header_row    = 3;
    bool exitLoop = false;
    int input;
    int max_range, start_row;
    string arrow = "->";
    int idx = 0;

    //extract the current directory
    fs::path current_directory(".");

    current_directory = fs::absolute( current_directory );
    vector<fs::path> contents = get_directory_contents( current_directory );

    //loop, printing screen
    while( !exitLoop ){

        //retrieve window size
        get_console_size( con_size_x, con_size_y);

        //clear the screen
        clear();
        attron(COLOR_PAIR(backg_window_pair));

        //print header
        print_header( "Geographic Meta Parser",  program_header_row, con_size_x, con_size_y); 

        //print menu header
        print_header( "Image Loading Screen", menu_header_row, con_size_x, con_size_y);
        wnoutrefresh(stdscr);

        //print file contents
        attron(A_STANDOUT);
        print_field( string("Current Directory: ") + fs::absolute(current_directory).string(), 2, con_size_x-4, 5 );
        attroff(A_STANDOUT);

        //compute max range
        max_range = std::min( (int)contents.size(), con_size_y - 7);

        /** List contents */
        start_row = 7;
        for( int a=0; a<max_range; a++ ){
            
            if( a == idx ){
                print_field( arrow, 1, arrow.size(), start_row+a );
            }

            if( fs::is_directory( contents[a] ) == true )
                attron( A_BOLD );

            print_field( contents[a].filename().string(), 2+arrow.size()+1, con_size_x-4, start_row+a );
        
            if( fs::is_directory( contents[a] ) == true )
                attroff( A_BOLD );
        }

        //draw screen
        wnoutrefresh(stdscr);
        doupdate();

        //wait on user input and take action
        input  = getch();

        switch( input ){
            
            /** Move cursor */
            case KEY_DOWN:
                idx++;
                if( idx >= contents.size() ) idx = 0;
                break;

            case KEY_UP:
                idx--;
                if( idx < 0 ) idx = contents.size()-1;
                break;
            
            /** If directory, then enter */
            case KEY_RIGHT:
                if( fs::is_directory( contents[idx] ) == true ){
                    current_directory /= contents[idx].filename();
                    contents = get_directory_contents( current_directory );
                    idx = 0;
                }
                else
                    return contents[idx].string();
                break;
            
            case KEY_LEFT:
                current_directory = current_directory.parent_path( );
                contents = get_directory_contents( current_directory );
                idx = 0;
                break;

            /** Exit Now */
            case 'q':
            case 'Q':
                exitLoop = true;
                break;



        }


    }

}


