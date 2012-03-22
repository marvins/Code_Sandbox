/* 
 * File:   Quaternion.cpp
 * Author: marvin_smith1
 * 
 * Created on March 15, 2012, 9:24 AM
 */

#include "Quaternion.h"

Quaternion::Quaternion(){
    real() = 0;
    imag() = vec(0,0,0);
}

Quaternion::Quaternion(const double& realVal, const double& i1, const double& i2, const double& i3){
    real() = realVal;
    imag() = vec( i1, i2, i3);
}

Quaternion::Quaternion( const vec& q){
    imag() = q;
    real() = 0;
}

Quaternion::Quaternion( const double theta, const vec& axis ){
    real() = cos(theta/2);
    imag() = sin(theta/2)*axis;
}

double& Quaternion::operator[](const int idx){
    if(idx==0)
        return d;
    else
        return i[idx];
}

double Quaternion::operator [](const int idx)const{
    if( idx == 0)
        return d;
    else
        return i[idx];
}

Quaternion operator* ( const Quaternion& a, const Quaternion& b ){
    
    Quaternion q;
    q.real() = a.real()*b.real() - dot(a.imag(),b.imag()); 
    q.imag() = a.real()*b.imag() + b.real()*a.imag() - cross(a.imag(), b.imag());
    
    return q;
}

double Quaternion::real()const{
    return d;
}

double& Quaternion::real(){
    return d;
}

vec Quaternion::imag()const{
    return i;
}

vec& Quaternion::imag(){
    return i;
}

Quaternion Quaternion::norm()const{
    double _mag = mag();
    return Quaternion(real()/_mag, imag()[0]/_mag, imag()[1]/_mag, imag()[2]/_mag);
}

double Quaternion::mag()const{
    return sqrt(mag2());
}

double Quaternion::mag2()const{
    return (real()*real() + dot(imag(),imag()));
}

Quaternion Quaternion::conj()const{
    
    Quaternion out;
    out.real() = real();
    out.imag() = imag() * (-1.0);
    return out;
}

Quaternion operator* (const Quaternion& qa, vec const& qb ){
    Quaternion other(qb);
    return (qa*other);
}
Quaternion operator* (const vec& qa, Quaternion const& qb ){
    Quaternion other(qa);
    return (other*qb);
}


Quaternion operator + ( const Quaternion& qa, const Quaternion& qb ){
    
    Quaternion out;
    out.real() = qa.real() + qb.real();
    out.imag() = qa.imag() + qb.imag();
    
    return out;
}

ostream& operator << ( ostream& ostr, const Quaternion& out){
    ostr << "quaternion( " << out.real() << ", " << out.imag() << " )";
    return ostr;
}

Quaternion operator* (const Quaternion& qa, double const& s ){
    Quaternion out;
    out.real() = qa.real()*s;
    out.imag() = qa.imag()*s;
    return out;
}
Quaternion operator* (double const& s, const Quaternion& qa ){
    Quaternion out;
    out.real() = qa.real()*s;
    out.imag() = qa.imag()*s;
    return out;
}