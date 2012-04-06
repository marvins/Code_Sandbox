#ifndef __PROGRESS_BAR_H__
#define __PROGRESS_BAR_H__

#include <iomanip>
#include <string>

using namespace std;

class ProgressBar{

   public:

      ProgressBar();

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

ostream& operator << ( ostream& ostr, const ProgressBar& bar );

#endif
