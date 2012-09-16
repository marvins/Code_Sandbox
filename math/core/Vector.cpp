#include "Vector.hpp"

/**
 * Cross product between two vectors
 * @param a first vector
 * @param b second vector
 * @return cross product vector
 */
vec cross( const vec& a, const vec& b ){
    
    return vec( a[1]*b[2] - a[2]*b[1], a[2]*b[0] - a[0]*b[2], a[0]*b[1] - a[1]*b[0]);
}


