#ifndef __PROGRESS_BAR_H__
#define __PROGRESS_BAR_H__

#include <iomanip>
#include <iostream>
#include <string>


/**
 * @class ProgressBar 
 *
 * A very simple progress bar for outputting status
*/
class ProgressBar{

   public:

      ProgressBar();

      ProgressBar( const double& _min, const double& _max, const int& _bar_width=50 );

      void update( const double& val );
      
      /** 
       * Return the progress bar as a string
      */
      std::string toString();
      
      /**
        * Move the current position of the counter by 1 step
      */
      void increment();

      /** 
        * Move the current position of the counter by 1 step
      */
      void decrement();

      double min;
      double max;
      double step;

      double current;

      std::string output;
      std::string prev_output;

      size_t bar_width;
      char here;
      char not_here;

};

std::ostream& operator << ( std::ostream& ostr, ProgressBar& bar );

#endif
