/* 
 * File:   Vector.h
 * Author: marvin_smith1
 *
 * Created on March 15, 2012, 9:34 AM
 */

#ifndef VECTOR_H
#define	VECTOR_H

#include <math.h>
#include <iostream>
#include <vector>

using namespace std;

template<typename TP, int N>
class Vector {
public:
    
    /**
     * vector constructor
     */
    explicit Vector(){
        data.resize(N);
        for( size_t i=0; i<N; i++)
            data[i] = 0;
    }
    
    /** 
     * Parameterized Constructor
     */
    explicit Vector(vector<TP> list){
        if( list.size() > N )
            throw string("Error: Invalid input size");
        
        data.resize(N);
        for(size_t i=0; i<list.size(); i++)
            data[i] = list[i];
    }
    
    explicit Vector( const TP& a, const TP& b, const TP& c ){
        data.resize(N);
        data[0] = a;
        data[1] = b;
        data[2] = c;
    }
    
    /**
     * Compute the dot product
     */
    TP dot( const Vector<TP,N>& a)const{
        
        TP sum = 0;
        for(size_t i=0; i<N; i++)
            sum += (*this)[i]*a[i];
        return sum;
    }
    
    /**
     * Compute the magnitude of the vector
     * @return magnitude (sqrt(mag2))
     */
    TP mag() const{
        return sqrt(mag2());
    }
    
    /**
     * Compute the squared sum of vector elements
     * @return 
     */
    TP mag2()const{
        TP val = 0;
        for(size_t i=0; i<N; i++)
            val += (*this)[i]*(*this)[i];
    }
    
    
    /**
     * Read Only Accessor Operator
     * @param idx index
     * @return copy of data
     */
    TP operator[](const size_t& idx)const{
        if( idx >= N )
            throw string("Error: invalid index");
        
        return data[idx];
    }
    
    /**
     * Read/Write Accessor Operator
     * @param[in] idx index
     * @return reference to data
     */
    TP& operator[](const size_t& idx){
        return data[idx];
    }
    
    
    size_t size()const{
        return N;
    }
    
private:
    vector<TP> data;
    
};

//common typedef for 3D vector
typedef Vector<double,3> vec;


template<typename TP, int N>
ostream& operator << (ostream& ostr, const Vector<TP,N>& vec){
    ostr << "vec( ";
    for(size_t i=0; i<N; i++){
        ostr << vec[i];
        if( i+1 < N)
            ostr << ", ";
    }
    ostr << " )";
    return ostr;
}


vec cross( const vec& a, const vec& b );

/**
 * Compute the dot product
 * @param vA First vector
 * @param vB Second vector
 * @return Dot product
 */
template<typename TP, int N>
double dot( const Vector<TP,N>& va, const Vector<TP,N>& vb ){
    
    double sum = 0;
    for(size_t i=0; i<va.size(); i++)
        sum += va[i]*vb[i];
    
    return sum;
}

/**
 * Add two equally-sized vectors
 * @param vA First vector
 * @param vB Second vector
 * @return Sum of both vectors
 */
template<typename TP, int N>
Vector<TP,N> operator + ( const Vector<TP,N>& vA, const Vector<TP,N>& vB ){
    
    Vector<TP,N> outVec;
    for(size_t i=0; i<N; i++)
        outVec[i] = vA[i]+vB[i];
    return outVec;
}

/**
 * Subtract two equally-sized vectors
 * @param vA First vector
 * @param vB Second vector
 * @return Difference of both vectors
 */
template<typename TP, int N>
Vector<TP,N> operator - ( const Vector<TP,N>& vA, const Vector<TP,N>& vB ){
    
    Vector<TP,N> outVec;
    for(size_t i=0; i<N; i++)
        outVec[i] = vA[i]-vB[i];
    return outVec;
}

/**
 * Compute product of vector with scalar
 * @param vA First vector
 * @param s  Scalar
 * @return Product of vector with scalar
 */
template<typename TP, int N>
Vector<TP,N> operator* ( const Vector<TP,N>& vA, const TP& s ){
    
    Vector<TP,N> outVec;
    for(size_t i=0; i<N; i++)
        outVec[i] = vA[i]*s;
    return outVec;
}

template<typename TP, int N>
Vector<TP,N> operator* ( const TP& s, const Vector<TP,N>& vA ){
    
    Vector<TP,N> outVec;
    for(size_t i=0; i<N; i++)
        outVec[i] = vA[i]*s;
    return outVec;
}

template<typename TP, int N>
Vector<TP,N> operator/ ( const Vector<TP,N>& vA, const TP& s ){
    
    Vector<TP,N> outVec;
    for(size_t i=0; i<N; i++)
        outVec[i] = vA[i]/s;
    return outVec;
}

template<typename TP, int N>
double norm( const Vector<TP,N>& vA, const Vector<TP,N>& vB){

   double sum = 0;
   for(size_t i=0; i<N; i++)
      sum += (vA[i]-vB[i])*(vA[i]*vB[i]);
   
   return sqrt((double)sum);
}

#endif	/* VECTOR_H */

