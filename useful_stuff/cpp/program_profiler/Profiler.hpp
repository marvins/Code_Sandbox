#ifndef __SRC_PROFILER_HPP__
#define __SRC_PROFILER_HPP__

#include <map>
#include <string>

class Interval{

    public:

        Interval( std::string const& _name );

    private:

        std::string name;
        

}

class Profiler{

    public:

        void add_interval( std::string const& interval_name );

        void start_interval(   std::string const& interval_name );
        void restart_interval( std::string const& interval_name ); 
        void stop_interval(    std::string const& interval_name );

        void tick( std::string const& interval_name );

    private:

        std::map<string,Interval> interval_list;

};


#endif

