#include "ui.h"
#include "geo_forms.h"
#include "utils.h"

#include <string>

#include <ncurses.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <GeoImage.h>

using namespace cv;
using namespace std;

const int program_header_row = 0;
const int menu_header_row    = 2;
const int meta_header_row    = 3;

const int footer_height      = 4;

const int MAIN_MENU_TIMEOUT  = 100;


vector<geo_header_item> pull_header_metadata( GEO::GeoImage const& img );

void init_gui( Options const& configuration ){

    //take the console
    initscr();

    //disable line buffering
    raw();
    
    //enable the keypad
    keypad( stdscr, true);
    
    //disable echo
    noecho();

    //enable color console
    start_color();

}

void stop_gui( Options const& configuration ){

    //return the console back to bash
    endwin();

}

void resolve_window_range( int& top_win, int& bot_win, int const& cur_pos , const int& size ){

    //if the cursor is within the top and bottom range, then simply exit
    if( cur_pos >= top_win && cur_pos <= bot_win )
        return;

    //compute the difference between the cursor and each marker
    int distTopCurs = cur_pos - top_win;
    int distBotCurs = cur_pos - bot_win;
    
    int diff;
    if( fabs(distTopCurs) < fabs(distBotCurs) )
        diff = distTopCurs;
    else
        diff = distBotCurs;


    //apply results
    top_win += diff;
    bot_win += diff;

}


/**
  * Return true if the string is a double
*/
bool isDouble( const string& value ){

    double output;
    stringstream( value ) >> output;
    return output;
}

void main_menu( Options& configuration ){
    
    int con_size_x, con_size_y;
    int input = 0;
    int skey = 0;
    bool exit_program = false;
    int window_height;
    bool change_result = false;
    int  save_result   = 0;

    //initialize GeoImage object
    configuration.current_image.set_init( true );
    if( configuration.current_image.get_init() == false ){

        //NOTE: Need to comment this out as we want blank images to load
        //throw string(string("Error: image ")+configuration.filename+string(" did not load properly"));
    }

    /* Pull Header Metadata */
    vector<geo_header_item>  header_metadata = pull_header_metadata( configuration.current_image );
    
    //set timeout function
    timeout( MAIN_MENU_TIMEOUT );
    fflush(stdout);

    while( exit_program == false ){

        //retrieve window size
        get_console_size( con_size_x, con_size_y);

        //fix the window_top and cursor position variables
        window_height = (con_size_y-footer_height) - meta_header_row;

        //we already have the window top, now lets find the window bottom
        int win_bot = configuration.window_top + window_height - 1;
        resolve_window_range( configuration.window_top, win_bot, configuration.cursor_pos, header_metadata.size() );


        //clear console screen
        clear( );
        attron(COLOR_PAIR(backg_window_pair));

        //print program header
        print_header( "Geographic Meta Parser",  program_header_row, con_size_x, con_size_y); 

        //print menu header
        print_header( "Main Menu", menu_header_row, con_size_x, con_size_y);
        wnoutrefresh(stdscr);

        //print information
        print_metadata( header_metadata, meta_header_row, con_size_x, con_size_y-footer_height, configuration.cursor_pos, configuration.window_top );
        wnoutrefresh(stdscr);

        //print footer
        print_footer( con_size_x, con_size_y );

        //draw screen
        wnoutrefresh(stdscr);
        doupdate();

        //wait on user input and take action
        input  = getch();
        
        string old_tag, new_tag, old_val, new_val;
        geo_header_item tmp;
        bool systemWorks = true;

        switch (input){

            case 27: // ALT OR ESCAPE

                skey = getch();
                if( skey != ERR){  // You have alt key

                }
                else{ // You have escape key
                    exit_program = true;
                }
                break;
            
            /****************************************************************/
            /*              Change the current metadata item                */
            /****************************************************************/
            case 'c':
            case 'C':
               
                old_tag = header_metadata[configuration.cursor_pos].header_tag;
                old_val = header_metadata[configuration.cursor_pos].header_val;
                tmp     = header_metadata[configuration.cursor_pos];
                
                change_result = change_screen( tmp, con_size_x, con_size_y );
                
                new_tag = tmp.header_tag;
                new_val = tmp.header_val;
                
                if( header_metadata[configuration.cursor_pos].header_type == 2 ){ //since we are dealing with a coordinate, do the appropriate check
                    
                    //check for a double
                    if( isDouble( tmp.header_val ) == false ){
                        systemWorks = false;
                    }
                }
                
                //if the change screen function returns true, then we need to go ahead and add it back to the image
                if( change_result == true && systemWorks == true ){
                    if( old_tag == new_tag && old_val != new_val ){
                        
                        header_metadata[configuration.cursor_pos].header_val = new_val;
                        configuration.current_image.modify_header_metadata( header_metadata[configuration.cursor_pos].header_tag,
                                                                            header_metadata[configuration.cursor_pos].header_val, 1);
                    }   
                }


                break;
            
            case 's':
            case 'S':
                
                save_result = save_screen( con_size_x, con_size_y );
                if( save_result == 1 ){
                    configuration.current_image.write_image();
                }
                if( save_result == 2 ){
                    throw string("ERROR: feature not supported");
                }

                break;

            case 'q':
            case 'Q':
                exit_program = true;
                break;

                /**  KEY VALUES  */
            case KEY_UP:

                //make sure that the screen isn't locked for various parameters

                //decrement cursor position
                configuration.cursor_pos--;
                if( configuration.cursor_pos < 0 )
                    configuration.cursor_pos = ((int)header_metadata.size()) + configuration.cursor_pos;
                break;

            case KEY_DOWN:

                //make sure that the screen isn't locked for various parameters

                //decrement cursor position
                configuration.cursor_pos++;
                if( configuration.cursor_pos >= header_metadata.size())
                    configuration.cursor_pos = ((int)header_metadata.size()) - configuration.cursor_pos;
                break;

            default:;

        }
        fflush(stdout);

    }

}

/**
 *  Process command-line arguments into tangible flags and options.
 * 
 * @param[in] args Command-Line arguments
 * @param[in/out] configuration Config flag container
 */
void process_arguments( std::vector<pair<std::string,std::string> >const& args, Options& configuration ){

    //iterate through arguments evaluating for flags
    for( size_t i=1; i<args.size(); i++){

        //Process File
        if( args[i].first.substr(0, 6) == "-file=" ){
            configuration.filename = args[i].second.substr(6);
            configuration.file_set = true;
            configuration.current_image.set_filename(configuration.filename);

        }

        //Process Debug Level
        else if( args[i].first.substr(0, 5) == "-dbg=" )
            configuration.debug_level = str2int(args[i].second.substr(5));

        //Process Help Flags
        else if( args[i].first.substr(0,5) == "-help" ){
            usage();
        }

        //throw exception
        else
            throw string(string("Error: unknown flag -> ") + args[i].second );
    }


}

void usage(){
    cout << " usage:  " << endl;
    throw string("");
}


/** 
  Extract the metadata from the header and geoimage to enable us to present
  it to the user.
*/
vector<geo_header_item>  pull_header_metadata( GEO::GeoImage const& img ){

    //create output data container
    vector<geo_header_item>  output;
    
    //check to see if the image is even loaded
    if( img.gdal_load() == false ){
        output.clear();
        
        //load a blank header object    
        output.push_back( geo_header_item(-1, "",""));
        output.push_back( geo_header_item( 0, "Image Information", "Value" ));

        //get image filename
        output.push_back( geo_header_item( 4, "Image Filename", img.get_filename() )); 
        output.push_back( geo_header_item( 3, "Image Type", img.getImageTypeName() ));

        output.push_back( geo_header_item(-1, "",""));
        output.push_back( geo_header_item( 0, "Header Information", "Value" ));


        return output;
    }

    //pull the GeoHeader
    vector<pair<string,string> >  hdr_dat = img.get_header()->get_header_data(); 

    //create header value 
    output.push_back( geo_header_item(-1, "",""));
    output.push_back( geo_header_item( 0, "Image Information", "Value" ));

    //get image filename
    output.push_back( geo_header_item( 4, "Image Filename", img.get_filename() )); 

    //get image type
    output.push_back( geo_header_item( 3, "Image Type", img.getImageTypeName() ));

    //get corner information
    Point2f ul, br;
    img.get_corner_coordinates( ul, br );
    output.push_back( geo_header_item(-1, "",""));

    output.push_back( geo_header_item( 2, "UPPER_LEFT_LAT", float2str(ul.y)));
    output.push_back( geo_header_item( 2, "UPPER_LEFT_LON", float2str(ul.x)));
    output.push_back( geo_header_item( 2, "LOWER_RIGHT_LAT", float2str(br.y)));
    output.push_back( geo_header_item( 2, "LOWER_RIGHT_LON", float2str(br.x)));

    output.push_back( geo_header_item(-1, "",""));
    output.push_back( geo_header_item( 0, "Header Information", "Value" ));

    //get header metadata
    for( size_t i=0; i<hdr_dat.size(); i++ ){
        output.push_back( geo_header_item( 1, hdr_dat[i].first, hdr_dat[i].second));
    }


    return output;
}


