#include "Math_Utilities.hpp"

#include <cmath>

namespace GEO{

/**
 * Round the value to the nearest integer
*/
int math_round( const double& val ){
    return std::floor( val+0.5);
}


}

