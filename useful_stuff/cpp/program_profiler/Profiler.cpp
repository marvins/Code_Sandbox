#include "Profiler.hpp"

#include <iostream>
#include <iterator>
#include <sstream>

#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::posix_time;
using namespace std;

template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}



Interval::Interval( string const& _name ){

    name = _name;
    current_interval = 0;
    intervals.resize(10);
}

void Interval::tick( ){
    
    //grab the current time and add it to the stack
    ptime current_time = microsec_clock::universal_time();
    intervals[current_interval].push_back( current_time );

}

void Interval::reset_interval( ){
    current_interval++;
    if( intervals.size() <= (current_interval+1))
        intervals.resize( current_interval+10 ); 
}

void Interval::tick_and_reset_interval( ){
    
    //tick the clock
    tick();

    //resize and increment the counter
    current_interval++;
    reset_interval();
}

string Interval::toString()const{
    
    //create a temporary stringstream
    stringstream sin;

    //create the output string
    string output = "";
    string ENDL = string("\n");
    
    output += string("Interval Name: ") + name + ENDL;
    output += ENDL;
    output += "Interval Periods" + ENDL;
    for( size_t i=0; i<current_interval; i++){
        output += string("p") + num2str(i) + string(": ");
        for( size_t j=0; j<intervals[i].size(); j++ ){
            sin.clear();
            sin.str("");
            sin << intervals[i][j];
            output += sin.str() + ", ";
        }
        output += ENDL;
    }

    return output;
}


void Profiler::add_interval( const string& interval_name ){

    //create a new interval object and add it to map
    interval_list.insert( IntervalPair( interval_name, Interval(interval_name)));  
    
}

void Profiler::tick( string const& interval_name ){
    
    //find the interval
    map<string, Interval>::iterator it = interval_list.find(interval_name);

    if( it != interval_list.end() ){
        it->second.tick();
    }

}

void Profiler::print_all( ){

    map<string, Interval>::iterator it = interval_list.begin();

    cout << "Results" << endl;
    while( it != interval_list.end() ){
        cout << it->second.toString() << endl;
        it++;
    }

}

void Profiler::restart_interval( string const& interval_name ){
    
    //find the interval
    map<string, Interval>::iterator it = interval_list.find(interval_name);
    if( it != interval_list.end() ){
        
        //tell the interval to tick and reset
        it->second.reset_interval();

    }


}

