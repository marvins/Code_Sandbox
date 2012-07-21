#include "ui.h"
#include "geo_forms.h"

#include <string>

#include <ncurses.h>

#include <cv.h>
#include <highgui.h>
#include <cvaux.h>

using namespace cv;

using namespace std;

const int program_header_row = 0;
const int menu_header_row    = 2;

const int MAIN_MENU_TIMEOUT  = 5;

void init_gui( Options const& configuration ){

    //take the console
    initscr();

    //disable line buffering
    raw();
    
    //enable the keypad
    keypad( stdscr, true);
    
    //disable echo
    noecho();

}

void stop_gui( Options const& configuration ){

    //return the console back to bash
    endwin();

}


void main_menu( Options& configuration ){
    
    int con_size_x, con_size_y;
    int input = 0;
    int skey = 0;
    bool exit_program = false;
    
    //initialize GeoImage object
    configuration.current_image.set_init( true );
    if( configuration.current_image.get_init() == false ){
        throw string(string("Error: image ")+configuration.filename+string(" did not load properly"));
    }
    
    namedWindow("WOW");
    imshow("WOW",configuration.current_image.get_image());
    waitKey(0);

    //set timeout function
    timeout( MAIN_MENU_TIMEOUT );
    fflush(stdout);

    while( exit_program == false ){

        //retrieve window size
        get_console_size( con_size_x, con_size_y);

        //clear console screen
        clear( );

        //print program header
        print_header( "Geographic Meta Parser",  program_header_row, con_size_x, con_size_y); 

        //print menu header
        print_header( "Main Menu", menu_header_row, con_size_x, con_size_y);

        //print information


        //print command options


        //print footer
        //print_footer( 

        //draw screen
        refresh();

        //wait on user input and take action
        input = getchar();

        switch (input){

            case 27: // ALT OR ESCAPE

                //check for second character
                skey = getch();
                if( skey != ERR){  // You have alt key

                }
                else{ // You have escape key
                    exit_program = true;
                }
                break;

            case 'q':
            case 'Q':
                exit_program = true;
                break;
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

    if( configuration.file_set == false )
        throw string("Error: no filename set on image");

}

void usage(){
    cout << " usage:  " << endl;
    throw string("");
}
