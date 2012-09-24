#include "Profiler.hpp"

#include <algorithm>
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

class Rect{

    public:
        Rect(){
            xmin = 0;
            xmax = 0;
            ymin = 0;
            ymax = 0;
        }
        
        double xmin, xmax, ymin, ymax;
};

Rect compute_range( vector<vector<double> > const& plot2d ){
    
    // compute the xrange
    Rect output;
    output.xmin = plot2d[0][0];
    output.xmax = plot2d[0][0];
    output.ymin = plot2d[0][1];
    output.ymax = plot2d[0][1];

    for( size_t i=1; i<plot2d[0].size(); i++ ){
        
        //do the main index
        output.xmin = std::min( plot2d[0][i], output.xmin );
        output.xmax = std::max( plot2d[0][i], output.xmax );
        
        output.ymin = std::min( plot2d[1][i], output.ymin );
        output.ymin = std::min( plot2d[1][i] + plot2d[2][i], output.ymin );
        output.ymin = std::min( plot2d[1][i] - plot2d[2][i], output.ymin );
        output.ymin = std::min( plot2d[1][3], output.ymin );
        
    
        output.ymax = std::max( plot2d[1][i], output.ymax );
        output.ymax = std::max( plot2d[1][i] + plot2d[2][i], output.ymax );
        output.ymax = std::max( plot2d[1][i] - plot2d[2][i], output.ymax );
        output.ymax = std::max( plot2d[1][3], output.ymax );
    
    }
    
    //add a buffer for the y values
    double ybuffer = (output.ymax - output.ymin)/5.0;
    output.ymax += ybuffer;
    

    return output;
}

void compute_mean( vector<double>const& values, double& median,  double& mean, double& stddev ){
    
    //set the default values
    mean = 0;
    stddev = 0;

    //don't process if the list is empty
    if( values.size() <= 0 ) return;

    //build the mean
    for( size_t i=0; i<values.size(); i++)
        mean += values[i];
    mean = mean/values.size();
    
    //build the std deviation
    for( size_t i=0; i<values.size(); i++)
        stddev += (values[i] - mean)*(values[i] - mean);
     stddev = sqrt(stddev / values.size());

    //compute the median
    vector<double> tvalues = values;
    std::sort( tvalues.begin(), tvalues.end() );
    median = tvalues[tvalues.size()/2];

}

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
    
    max_interval_length = 0;
    current_interval = 0;
    intervals.resize(10);
    interval_lengths.resize(10, 0);
}

bool Interval::is_empty()const{
    if( current_interval == 0 && max_interval_length == 0 )
        return true;
    return false;
}


/**
 * Sets the current clock time in utc for the next available step on the current interval.
*/
void Interval::tick( ){
    
    //grab the current time and add it to the stack
    ptime current_time = microsec_clock::universal_time();
    intervals[current_interval].push_back( current_time );
    
    //increment the interval length index
    interval_lengths[current_interval]++;
    
    //check if the max interval is less than the current interval
    if( interval_lengths[current_interval] > max_interval_length )
        max_interval_length = interval_lengths[current_interval] + 1;


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
    
    //increment the interval length index
    interval_lengths[current_interval] = std::max( interval_lengths[current_interval], step);

    //check if the max interval is less than the current interval
    if( interval_lengths[current_interval] > max_interval_length )
        max_interval_length = interval_lengths[current_interval] + 1;

}


void Interval::reset_interval( ){
    current_interval++;
    if( intervals.size() <= (current_interval+1)){
        intervals.resize( current_interval+10 ); 
        interval_lengths.resize( current_interval+10, 0 );
    }


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
   
    //create temporary interval object to modify
    std::vector< std::vector< boost::posix_time::ptime> > tintervals = intervals;
    
    //create and size temporary difference list
    vector<vector<double> > interval_diffs(tintervals.size());

    //start iterating through each list to compute the times duration between steps
    for( size_t i=0; i<tintervals.size(); i++){
        
        //skip if the interval has no elements
        if( interval_lengths[i] <= 0 )continue;

        interval_diffs[i].resize(interval_lengths[i]+1);
        interval_diffs[i][0] = 0;

        for( size_t j=1; j<interval_lengths[i]+1; j++){
            
            //check if the interval actually exists
            if( tintervals[i][j].is_not_a_date_time() ){
                tintervals[i][j] = tintervals[i][j-1];
                interval_diffs[i][j] = (tintervals[i][j]-tintervals[i][j-1]).total_microseconds();
            }
            else
                interval_diffs[i][j] = (tintervals[i][j]-tintervals[i][j-1]).total_microseconds();
        }
    }

    //create and size output object
    vector<vector<double> > output_data(4);
    for( int i=0; i<4; i++)
        output_data[i].resize(max_interval_length);
    
    //compute the necessary statistics of the steps
    int cidx = 0, cnt;
    double sum, mean, stddev, median;
    bool valset;
    vector<double> values;
    while( true ){
        
        //set the index value
        output_data[0][cidx] = cidx;

        //reset the sum, counter, value list, and stop flag
        sum = 0;
        cnt = 0;
        valset = false;
        values.clear();

        //iterate through each interval
        for( size_t i=0; i<interval_diffs.size(); i++){

            //make sure that we have enough elements
            if( interval_diffs[i].size() > cidx ){
                values.push_back( interval_diffs[i][cidx] );
                valset = true;
            }
        }

        compute_mean( values, median, mean, stddev );
        if( valset == false )
            break;
        else{
            output_data[1][cidx] = mean;
            output_data[2][cidx] = stddev;
            output_data[3][cidx] = median;
        }
        cidx++;
    }

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
        fout << "import numpy as np" << endl;
        fout << "import matplotlib as mpl" << endl;
        fout << "import matplotlib.pyplot as plt" << endl;
        fout << endl;
    
        //write a header
        fout << "print( 'Python plotting tool' )" << endl;
        fout << endl;
        
        //iterate through each interval
        for( map<string,Interval>::iterator it=interval_list.begin(); it != interval_list.end(); it++){
            
            if( it->second.is_empty() == true ) continue;

            //create the data arrays
            vector<vector<double> > plots2d = it->second.plot();
            
            //iterate through each set of axis
            for( size_t i=0; i<plots2d.size(); i++){
                
                //don't continue unless the axis has actual data
                if( plots2d[i].size() > 0 ){
                    fout << "a" << i << " = np.array([ ";
                    fout << plots2d[i][0];

                    for( size_t j=1; j<plots2d[i].size(); j++)
                        fout << ", " << plots2d[i][j];

                    fout << "])" << endl;
                }
            }
            fout << endl;


            //now that the data points are created, go ahead and create the plot call
            fout << "plt.plot( a0, a1   , 'b-', label='Mean')" << endl;
            fout << "plt.plot( a0, a1+a2, 'b--', label='1 Standard Deviation' )" << endl;
            fout << "plt.plot( a0, a1-a2, 'b--' )" << endl;
            fout << "plt.plot( a0, a1+2*a2, 'b:', label='2 Standard Deviations' )" << endl;
            fout << "plt.plot( a0, a1-2*a2, 'b:' )" << endl;
            fout << endl;

            Rect bbox = compute_range( plots2d );
            //print the ranges of the plots
            fout << "plt.xlim( " << bbox.xmin+1 << ", " << bbox.xmax << ")" << endl;
            fout << "plt.ylim( " << bbox.ymin << ", " << bbox.ymax << ")" << endl;
            fout << endl;

            // show the plot
            fout << "plt.legend( loc = \"lower right\")" << endl;
            fout << "plt.show()" << endl;
            fout << endl;

        }

        //make the script runnable
        system((string("chmod 777 ")+output_name).c_str());

    }

    fout.close();

}
