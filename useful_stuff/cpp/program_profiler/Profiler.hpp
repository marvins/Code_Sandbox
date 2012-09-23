#ifndef __SRC_PROFILER_HPP__
#define __SRC_PROFILER_HPP__

#include <map>
#include <string>
#include <utility>
#include <vector>

#include <boost/date_time/posix_time/posix_time.hpp>



class Interval{

    public:
        
        /** 
         * Parameterized constructor given the name of the time interval.
        */
        Interval( std::string const& _name );
    
        void tick();
        void reset_interval();
        void tick_and_reset_interval( );
        
        std::string toString()const;
    private:
        
        std::string name;    /*< Name of the interval */
        
        int current_interval;
        std::vector< std::vector< boost::posix_time::ptime> > intervals;

};

typedef std::pair<std::string, Interval> IntervalPair;


class Profiler{

    public:

        void add_interval( std::string const& interval_name );

        void restart_interval( std::string const& interval_name ); 

        void tick( std::string const& interval_name );
        
        void print_all();

    private:

        std::map< std::string,Interval> interval_list;

};


#endif

