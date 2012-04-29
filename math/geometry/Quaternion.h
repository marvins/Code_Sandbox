/* 
 * File:   Quaternion.h
 * Author: marvin_smith1
 *
 * Created on March 15, 2012, 9:24 AM
 */

#ifndef QUATERNION_H
#define	QUATERNION_H

#include <cmath>

#include "Vector.h"

using namespace std;

class Quaternion {
public:
    
    Quaternion();
    Quaternion(const double& a, const double& b, const double& c, const double& d);
    Quaternion( const vec& q);
    
    Quaternion( const double theta, const vec& axis );
    
    
    double& operator[](const int i);
    double  operator[](const int i)const;
    
    double real()const;
    double& real();
    
    vec imag()const; 
    vec& imag();
    
    Quaternion norm()const;
    double mag()const;
    double mag2()const;
    Quaternion conj()const;
    
private:
    double d;
    vec i;
    
};

Quaternion operator* ( const Quaternion& qa, const Quaternion& qb );

Quaternion operator* (const Quaternion& qa, vec const& qb );
Quaternion operator* (const vec& qa, Quaternion const& qb );

Quaternion operator* (const Quaternion& qa, double const& s );
Quaternion operator* (double const& s, const Quaternion& qa );

Quaternion operator + ( const Quaternion& qa, const Quaternion& qb );

ostream& operator << ( ostream& ostr, const Quaternion& out);



#endif	/* QUATERNION_H */

