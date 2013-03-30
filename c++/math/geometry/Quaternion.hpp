/* 
 * File:   Quaternion.h
 * Author: marvin_smith1
 *
 * Created on March 15, 2012, 9:24 AM
 */

#ifndef QUATERNION_H
#define	QUATERNION_H

#include <cmath>

#include "Vector.hpp"

#define QUATERNION_REQUIRES_OPENCV

#ifdef QUATERNION_REQUIRES_OPENCV
#include <opencv2/core/core.hpp>
#endif 

using namespace std;

/**
 * A generic Quaternion class for managing rotations of points and vectors.
*/
class Quaternion {
public:
    
    /**
     * Default Constructor
    */
    Quaternion();
    
    /**
     * Parameterized constructor 
     * 
     * @param[in] w Real Value
     * @param[in] x X component of imaginary component
     * @param[in] y Y component of imaginary component
     * @param[in] z Z component of imaginary component
    */
    Quaternion(const double& w, const double& x, const double& y, const double& z);
    
    /** 
     * Parameterized constructor.  Helps convert a vector to a quaternion 
     * with a real value of 0.
     * 
     * @param[in] q Vector to convert.
    */
    Quaternion( const vec& q);
    
    /** 
     * Quaternion constructor.  Axis-Angle to Quaternion.
     *
     * @param[in] theta Angle in radians.
     * @param[in] axis  Axis of rotation.
    */
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
    
#ifdef QUATERNION_REQUIRES_OPENCV
    cv::Mat get_rotation_matrix()const;
#endif

    double get_angle( const bool& inRadians = true )const;
    vec get_axis()const;

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

