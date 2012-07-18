#include "ui.h"
#include "geo_forms.h"

#include <string>

#include <ncurses.h>


using namespace std;

const int program_header_row = 0;
const int menu_header_row    = 2;

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


void main_menu( const vector<string>& args, Options const& configuration ){
    
    int con_size_x, con_size_y;
    int input = 0;
    int skey = 0;
    bool exit_program = false;

    //set timeout function
    timeout(25);

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
        input = getch();

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

    }

}

void process_arguments( std::vector<std::string>const& args, Options& configuration ){

    //iterate through arguments evaluating for flags
    for( size_t i=1; i<args.size(); i++){

        if( args[i][0] == '-' ){ //process item as flag

        }
        else{ //process item as a filename

        }

    }

}
