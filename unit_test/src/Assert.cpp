#include "Assert.h"

bool ASSERT_EQUAL( double const& valA, double const& valB, double const& diff ){

    if( fabs( valA - valB ) > diff ) 
        return false;
    return true;
}


