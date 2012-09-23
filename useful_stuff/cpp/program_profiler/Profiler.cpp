#include "Profiler.hpp"

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>


#include <boost/date_time/local_time/local_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace boost::gregorian;
using namespace boost::local_time;
using namespace boost::posix_time;
using namespace std;

ptime time_t_epoch(date(1970,1,1)); 

/** A simple function to convert numbers to strings */
template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}


/**
 * Default contructor
 *
 * Initializes and creates the interval object
*/
Interval::Interval( ){

    current_interval = 0;
    intervals.resize(10);
}

/**
 * Sets the current clock time in utc for the next available step on the current interval.
*/
void Interval::tick( ){
    
    //grab the current time and add it to the stack
    ptime current_time = microsec_clock::universal_time();
    intervals[current_interval].push_back( current_time );

}

/**
 * Sets the current clock time in utc for the specified step on the current interval.
 *
 * @param[in] step Current interval position to set time for.
*/
void Interval::tick( const int& step ){
    
    //grab the current time and add it to the stack
    ptime current_time = microsec_clock::universal_time();
    
    if( intervals[current_interval].size() <= step+1 )
        intervals[current_interval].resize(step+2);

    intervals[current_interval][step] = current_time;

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


/**
 * The following rows will be build
 * [0] - list of indecies
 * [1] - list of means
 * [2] - list of standard deviations
*/
vector<vector<double> > Interval::plot( )const{
    
    //create output object
    vector<vector<double> > output_data;

    //start iterating through each list
    
    return output_data;
}


void Profiler::add_interval( const string& interval_name ){

    //create a new interval object and add it to map
    interval_list.insert( IntervalPair( interval_name, Interval()));  
    
}

void Profiler::tick( string const& interval_name ){
    
    //find the interval
    map<string, Interval>::iterator it = interval_list.find(interval_name);

    if( it != interval_list.end() ){
        it->second.tick();
    }

}

void Profiler::tick( string const& interval_name, const int& interval_step ){
    
    //find the interval
    map<string, Interval>::iterator it = interval_list.find(interval_name);

    if( it != interval_list.end() ){
        it->second.tick(interval_step);
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

void Profiler::build_chart( const int& output_format, const string& output_name ){

    ofstream fout;

    fout.open(output_name.c_str());
    
    if( output_format == PROFILER_SCIPY ){
        
        //write the run header
        fout << "#! /usr/bin/env python" << endl;
        fout << endl;
        
        //write the imports
        fout << "import matplotlib as mpl" << endl;
        fout << "import matplotlib.pyplot as plt" << endl;
        fout << endl;
    
        //write a header
        fout << "print( 'Python plotting tool' )" << endl;
        fout << endl;
        
        //iterate through each interval
        for( map<string,Interval>::iterator it=interval_list.begin(); it != interval_list.end(); it++){

            //create the data arrays
            vector<vector<double> > plots2d = it->second.plot();

        }

        //make the script runnable
        system((string("chmod 777 ")+output_name).c_str());

    }

    fout.close();

}
