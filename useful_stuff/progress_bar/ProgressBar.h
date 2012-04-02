#ifndef __PROGRESS_BAR_H__
#define __PROGRESS_BAR_H__

#include <iomanip>
#include <string>

using namespace std;

class ProgressBar{

   public:

      ProgressBar(){
         min = 0;
         max = 1;
         current = 0;
         bar_width = 50;
         here = '*';
         not_here = ' ';
      }

      double min;
      double max;
      double step;

      double current;

      string output;
      string prev_output;

      size_t bar_width;
      char here;
      char not_here;

};

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

#endif
