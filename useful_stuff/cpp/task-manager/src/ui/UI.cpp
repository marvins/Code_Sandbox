#include "UI.hpp"

#include <ncurses.h>

#include <cmath>

#include "../core/FileUtilities.hpp"

void start_ui( ){
    
    initscr();
    keypad(stdscr, TRUE);
    noecho();

}

void stop_ui( ){
    
    endwin();

}

void user_interface( ){
    
    // window dimensions
    int maxX, maxY;

    // iterate until its time to exit
    while( true ){
        
        // get the window size
        maxX = getmaxx(stdscr);
        maxY = getmaxy(stdscr);

        // clear the screen
        clear();
        
        // print the header
        print_header("Task Listing", maxX);

        // print the table
        print_table( maxX, maxY, 4 );
        
        // print the footer
        print_footer( GUI, maxX, maxY );

        // get the input
        int arg = getch();

        // exit the program
        if( arg == 27 )
            break;
        // create a new task
        else if( arg == 'c' || arg == 'C' ){
            create_task( );
        }


    }

}

void print_header( const string& title, const int& maxX ){

    int startx = (maxX / 2) - 6;
    int startxx= (maxX / 2) - 5;
    mvaddstr( 0, startx, "Task-Manager");
    mvaddstr( 1, startxx, " Task List");
    string bar;
    for( int i=0; i<maxX; i++ )
        bar+= '-';
    mvaddstr( 2, 0, bar.c_str());


}


void print_table( const int& maxX, const int& maxY, const int& offY ){
    
    int maxPos = std::min( maxY, (int)options.tasks.size());

    // print each row
    for( size_t i=0; i<maxPos; i++ ){
        string entry = num2str(i+1) + string(": ") + options.tasks[i].name.c_str();        
        mvaddstr( i+offY, 3, entry.c_str() );
    }

}

void print_footer( const int& type ,int const& maxX, int const& maxY ){

    // print the bottom line
    string bar;
    for( int i=0; i<maxX; i++ )
        bar+= '-';
    mvaddstr( maxY-4, 0, bar.c_str());
    
    if( type == GUI )
        mvaddstr( maxY-3, 2, "Options: ESC-Quit, C-Create Task, D-Delete Tasks, V-View Tasks");
    else if( type == ADD_TASK )
        mvaddstr( maxY-3, 2, "Options: ESC-Exit (Save Prompted)");

}


void create_task( const bool& GUI ){

    if( GUI == true ){

        // get the window size
        int maxX = getmaxx(stdscr);
        int maxY = getmaxy(stdscr);
        
        // generate an empty task
        Task newTask;
        bool exitLoop = false;
        int cursor = 0;
        int ncursor = 0;
        int cpos[3] = {0, 0, 0};
        vector<int> npos(1,0);

        // print the new task ui
        while( !exitLoop ){
            
            // clear the console
            clear();

            // print the header
            print_header("New Task", maxX);
            
            // print the requred fields
            if( cursor == 0 )
                attron( A_STANDOUT );
            mvaddstr( 4, 1, (string("Name:   ") + newTask.name).c_str() );
            if( cursor == 0 )
                attroff( A_STANDOUT );
            
            if( cursor == 1 )
                attron( A_STANDOUT );
            mvaddstr( 5, 1, string("Groups: ").c_str());
            if( cursor == 1 )
                attroff( A_STANDOUT );
            
            if( cursor == 2 )
                attron( A_STANDOUT );
            mvaddstr( 6, 1, string("Notes: ").c_str());
            if( cursor == 2 )
                attroff( A_STANDOUT );
            for( size_t i=0; i<newTask.notes.size(); i++ ){
                mvaddstr( 7+i, 2, newTask.notes[i].c_str());
            }

            // print the footer
            print_footer( ADD_TASK, maxX, maxY );

            // get user input
            int selection = getch();
            string tempS;

            /// Quit
            if( selection == 27 ){
                exitLoop = true;
            }
            else if( selection == KEY_UP ){
            
                // make sure we are not in a note
                if( cursor == 0 || cursor == 1 ){
                    cursor--;
                    if( cursor < 0 ){
                        cursor = 0;
                    }
                }
                else if( cursor == 2 ){
                    ncursor--;
                    if( ncursor < 0 ){
                        ncursor = 0;
                        cursor--;
                    }
                }
                else throw string("WHAT!");
            }
            else if( selection == KEY_DOWN ){
            
                // make sure we are not in a note
                if( cursor == 0 || cursor == 1 ){
                    cursor++;
                    if( cursor == 2 ){    
                        ncursor = 0;
                    }

                }
                else if( cursor == 2 ){
                    ncursor++;
                    if( ncursor >= newTask.notes.size() ){
                        ncursor = newTask.notes.size()-1;
                    }
                }
                else throw string("WHAT!");
            } 
            
            /// User enters text
            else if( (selection >= 'a' && selection <= 'z') ||
                     (selection >= 'A' && selection <= 'Z') ||
                      selection == '.' || selection == ',' || selection == ' '){
                
                tempS = (char)selection;
                if( cursor == 0 ){ newTask.name.insert(cpos[0], tempS); cpos[0]++; }
                    
            }
            /// User Presses Enter
            else if( selection == 13 ){
                
                if(cursor == 0 || cursor == 1){
                    cursor++;
                }

            }

        }
    }
    else{

       // create an empty task
       Task newTask;

       // load the entries
       while( (int)options.args.size() > 0 ){
            if( options.args[0] == "-name" ){

                if( options.args.size() < 1 )
                    throw string("Error: no filename given after -name");
                else{
                    newTask.name = options.args[1];
                    options.args.pop_front();
                }
                options.args.pop_front();
            } else if( options.args[0] == "-groups" ){

                if( options.args.size() < 1 )
                    throw string("Error: no groups given after -groups");
                else{
                    newTask.groups.push_back(options.args[1]);
                    options.args.pop_front();
                }
                options.args.pop_front();
            } else if( options.args[0] == "-notes" ){
                options.args.pop_front();

                if( options.args.size() < 1 )
                    throw string("Error: no filename given after -name");
                else{
                    while( options.args.size() > 0 ){
                         newTask.notes.push_back(options.args[0]);
                        options.args.pop_front();
                    }
                }
            }
       }


    }

}


