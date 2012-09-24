#include "PlotMath.hpp"


Point2d compute_range( vector<double>const& data ){
    
    Point2d output;
    if( data.size() <= 0 )return output;

    output.x = data[0];
    output.y = data[0];
    for( size_t i=1; i<data.size(); i++){
        output.x = min( output.x, data[i] );
        output.y = max( output.y, data[i] );
    }
    
    return output;
}


