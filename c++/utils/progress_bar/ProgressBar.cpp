#include "ProgressBar.hpp"

#include <sstream>

using namespace std;

/** 
 * Default Constructor which iterates from 0 until 1
*/
ProgressBar::ProgressBar(){
   min = 0;
   max = 1;
   current = 0;
   bar_width = 50;
   here = '*';
   not_here = ' ';
   step = 1;
}

/**
 * A more uniform progress bar which iterates from min until max 
*/
ProgressBar::ProgressBar( const double& _min, const double& _max, const int& _bar_width ){
    min = _min;
    max = _max;
    current = min;
    bar_width=_bar_width;
    here = '*';
    not_here = ' ';
    step = 1;
}

void ProgressBar::increment( ){
    current += step;
    if( current > max ) current=max;
}

void ProgressBar::decrement( ){
    current -= step;
    if( current < 0 ) current=0;
}

void ProgressBar::update( const double& val ){
    current = val;
    if( current > max ) current=max;
}

string ProgressBar::toString(){
    
    
    /*
    if( prev_output.size() > 0 ){
        cout << '\r';
        for( size_t i=0; i<prev_output.size(); i++ )
            cout << " " << flush;
    }*/

   int width = 5;
   double pct = (current - min)/(max - min)*100;
   
   //create a string stream
   stringstream sin;


    //load the stream object
   sin << "Progress: " << std::fixed  << std::setprecision(2) << pct  << "\%  ";
   sin << "[";
   
   double pos = 0;
   double step = (max - min)/(double)bar_width;
   for( size_t i=0; i<bar_width; i++){
      pos = i*step;
      if( pos < current )
         sin << here;
      else
         sin << not_here;
   }
   sin << "]";
   output = sin.str();
    
    prev_output = output;
   
   return output;
}

ostream& operator << ( ostream& ostr, const ProgressBar& bar ){

   int width = 5;
   double pct = (bar.current-bar.min)/(bar.max-bar.min)*100;
   ostr << "Progress: " << setiosflags(ios::left) << setw(width) << pct << resetiosflags(ios::left) << "\%  ";
   ostr << "[";
   double pos = 0;
   double step = (bar.max-bar.min)/(double)bar.bar_width;
   for( size_t i=0; i<bar.bar_width; i++){
      pos = i*step;
      if( pos < bar.current )
         ostr << bar.here;
      else
         ostr << bar.not_here;
   }
   ostr << "]";
   return ostr;
}

