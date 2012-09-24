#ifndef __SRC_PROFILER_HPP__
#define __SRC_PROFILER_HPP__

#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>

enum PROFILER_OUTPUT_FORMATS {
    PROFILER_SCIPY,
    PROFILER_MATLAB,
};

class Interval{

    public:
        
        /** 
         * Default constructor
        */
        Interval( );
    
        void tick();
        void tick( const int& step );
        void reset_interval();
        void tick_and_reset_interval( );
        
        std::string toString()const;

        std::vector<std::vector<double> > plot()const;
        
        bool is_empty()const;

    private:
        
        int max_interval_length;
        int current_interval;
        std::vector< std::vector< boost::posix_time::ptime> > intervals;
        std::vector<int> interval_lengths;

};

typedef std::pair<std::string, Interval> IntervalPair;


class Profiler{

    public:
    
        /** 
         * Add an interval to the profiler
        */
        void add_interval( std::string const& interval_name );

        /** 
         * Move the specified interval to the next time period
        */
        void restart_interval( std::string const& interval_name ); 

        void tick( std::string const& interval_name);
        void tick( std::string const& interval_name, const int& interval_step );
        
        void print_all();
        void build_chart( const int& output_format, const std::string& output_name );

    private:

        std::map< std::string,Interval> interval_list;

};


#endif

