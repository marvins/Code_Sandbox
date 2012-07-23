#include "vector.h"
      
vec2::vec2( GLfloat s ){
   x = s;
   y = s;
}

vec2::vec2( GLfloat xx, GLfloat yy ){
   x = xx;
   y = yy;
}
      
vec2::vec2( const vec2& v ){
   x = v.x;  
   y = v.y;  
}

GLfloat   vec2::operator [] ( int i ) const { return *(&x + i); }

GLfloat&  vec2::operator [] ( int i ){        return *(&x + i); }


vec2 vec2::operator - () const{
   return vec2( -x, -y ); 
}
   
vec2 vec2::operator + ( const vec2& v ) const{ 
   return vec2( x + v.x, y + v.y ); 
}

vec2 vec2::operator - ( const vec2& v ) const
{ 
   return vec2( x - v.x, y - v.y ); 
}

vec2 vec2::operator * ( const GLfloat s ) const{
   return vec2( s*x, s*y ); 
}
     
vec2 vec2::operator * ( const vec2& v ) const{ 
   return vec2( x*v.x, y*v.y ); 
}

vec2 operator * ( const GLfloat s, const vec2& v ){  
   return v * s; 
}

vec2 vec2::operator / ( const GLfloat s ) const{
   if ( std::fabs(s) < DivideByZeroTolerance ) {
      std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
         << "Division by zero" << std::endl;
      return vec2();
   }
   GLfloat r = GLfloat(1.0) / s;
   return *this * r;
}

vec2& vec2::operator += ( const vec2& v ){ 
   x += v.x;  
   y += v.y;   \
   return *this; 
}

vec2& vec2::operator -= ( const vec2& v ){ 
   x -= v.x;  
   y -= v.y;  
   return *this; 
}

//////////////////////////////////////////////////////////////////////////////
//
//  vec3.h - 3D vector
//
//////////////////////////////////////////////////////////////////////////////

//parameterized constructors
vec3::vec3( GLfloat s                       )        : x(s), y(s), z(s) {}
vec3::vec3( GLfloat x, GLfloat y, GLfloat z ) : x(x), y(y), z(z) {}

//copy constructors
vec3::vec3( const vec3& v ) {                x = v.x;  y = v.y;  z = v.z; }
vec3::vec3( const vec2& v, const float f ) { x = v.x;  y = v.y;  z = f; }
   

//indexing operators
GLfloat&      vec3::operator [] ( int i )       { return *(&x + i); }
const GLfloat vec3::operator [] ( int i ) const { return *(&x + i); }


//negation operator
vec3 vec3::operator - () const{ return vec3( -x, -y, -z ); }

//addition operator
vec3 vec3::operator + ( const vec3& v ) const { return vec3( x + v.x, y + v.y, z + v.z ); }

//subtraction operator
vec3 vec3::operator - ( const vec3& v ) const { return vec3( x - v.x, y - v.y, z - v.z ); }

//multiplication operators   
vec3 vec3::operator * ( const GLfloat s ) const { return vec3( s*x, s*y, s*z ); }
vec3 vec3::operator * ( const vec3& v )   const { return vec3( x*v.x, y*v.y, z*v.z ); }
vec3 operator * ( const GLfloat s,  const vec3& v ){ return v * s; }

//division operators
vec3 vec3::operator / ( const GLfloat s ) const {
    if ( std::fabs(s) < DivideByZeroTolerance ) {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
            << "Division by zero" << std::endl;
        return vec3();
    }

    GLfloat r = GLfloat(1.0) / s;
    return *this * r;
}


//
//  --- (modifying) Arithematic Operators ---
//

vec3& vec3::operator += ( const vec3& v )
{ x += v.x;  y += v.y;  z += v.z;  return *this; }

vec3& vec3::operator -= ( const vec3& v )
{ x -= v.x;  y -= v.y;  z -= v.z;  return *this; }

vec3& vec3::operator *= ( const GLfloat s )
{ x *= s;  y *= s;  z *= s;  return *this; }


/**
 * Vector Times Equal Operator
 * 
 * @param[in] v Vector to be multiplied
 * @return multiplied vector
 */
vec3& vec3::operator *= ( const vec3& v )
{ x *= v.x;  y *= v.y;  z *= v.z;  return *this; }

vec3& vec3::operator /= ( const GLfloat s ) {
    if ( std::fabs(s) < DivideByZeroTolerance ) {
        std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
            << "Division by zero" << std::endl;
    }

    GLfloat r = GLfloat(1.0) / s;
    *this *= r;

    return *this;
}

/** Extraction Operator
 * @param[in] os output stream
 * @param[in] v Vector to output
 * @return output stream
 */
std::ostream& operator << ( std::ostream& os, const vec3& v ) {
    return os << "( " << v.x << ", " << v.y << ", " << v.z <<  " )";
}

/** Insertion Operator
 * @param[in] is input stream
 * @param[in] v Vector to input
 * @return input stream
 */
std::istream& operator >> ( std::istream& is, vec3& v )
{ return is >> v.x >> v.y >> v.z ; }



//----------------------------------------------------------------------------
//
//  Non-class vec3 Methods
//


GLfloat dot( const vec3& u, const vec3& v ) {
    return (u.x*v.x + u.y*v.y + u.z*v.z);
}


vec3 cross(const vec3& a, const vec3& b )
{
    return vec3( a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x );
}

//////////////////////////////////////////////////////////////////////////////
//
//  vec4 - 4D vector
//
//////////////////////////////////////////////////////////////////////////////

//copy constructor
vec4::vec4( const vec3& v, const float w ) : w(w)
{ x = v.x;  y = v.y;  z = v.z; }

