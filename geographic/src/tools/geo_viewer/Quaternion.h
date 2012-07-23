/* 
 * File:   Quaternion.h
 * Author: marvin_smith1
 *
 * Created on March 15, 2012, 9:24 AM
 */

#ifndef QUATERNION_H
#define	QUATERNION_H

#include <cmath>

#include "vector.h"

using namespace std;

class Quaternion {
public:
    
    Quaternion();
    Quaternion(const float& a, const float& b, const float& c, const float& d);
    Quaternion( const vec3& q);
    
    Quaternion( const float theta, const vec3& axis );
    
    
    float& operator[](const int i);
    float  operator[](const int i)const;
    
    float real()const;
    float& real();
    
    vec3 imag()const; 
    vec3& imag();
    
    Quaternion norm()const;
    float mag()const;
    float mag2()const;
    Quaternion conj()const;
    
private:
    float d;
    vec3 i;
    
};

Quaternion operator* ( const Quaternion& qa, const Quaternion& qb );

Quaternion operator* (const Quaternion& qa, vec3 const& qb );
Quaternion operator* (const vec3& qa, Quaternion const& qb );

Quaternion operator* (const Quaternion& qa, float const& s );
Quaternion operator* (float const& s, const Quaternion& qa );

Quaternion operator + ( const Quaternion& qa, const Quaternion& qb );

ostream& operator << ( ostream& ostr, const Quaternion& out);



#endif	/* QUATERNION_H */

