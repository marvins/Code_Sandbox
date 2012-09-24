#include "PlotUtils.hpp"
#include <boost/algorithm/string.hpp>

using namespace boost;

vector<pair<string,string> > parse_plot_tokens( string const& input_str ){
    
    vector<string> substrs, parts;
    //break string by semicolon
    split( substrs, input_str, is_any_of(";") );

    //for every token pair, split by the = sign and trim
    vector<pair<string,string> > output;
    pair<string,string> tempPair;
    for( size_t i=0; i<substrs.size(); i++){
        
        split( parts, substrs[i], is_any_of("="));
        //trim each end
        trim(parts[0]);
        trim(parts[1]);

        output.push_back(pair<string,string>(parts[0],parts[1]));
    }
    
    return output;
}

