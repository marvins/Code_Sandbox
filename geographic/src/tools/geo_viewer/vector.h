#ifndef __SRC_VECTOR_H__
#define __SRC_VECTOR_H__

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include <cmath>
#include <iostream>

using namespace std;

const GLfloat  DivideByZeroTolerance = GLfloat(1.0e-07);


/**
   @class vec2
   @brief 2 Dimensional Vector
   
   A simple 2D vector accessed by x, y
*/
class vec2 {

   public:

      //member variables
      GLfloat  x;
      GLfloat  y;

      /**
        * vec2 Parameterized Constructor
        *
        * Builds the vector using 1 input arguement
        *
        * @param s floating point value which sets both
      */
      vec2( GLfloat s = GLfloat(0.0) );
      
      /**
        * vec2 Parameterized Constructor
        *
        * Builds the vector using 2 input arguements
        *
        * @param xx x parameter
        * @param yy y parameter
      */
      vec2( GLfloat xx, GLfloat yy );

      /**
        * vec2 Copy Constructor
        *
        * Makes a vector using a vector
        *
        * @param v Input vector clone
      */
      vec2( const vec2& v );


      /**
        * Index Operator (Square Brackets)
        *
        * allows indexing of vectors
        *
        * @param i item in array to access
        * @return value of index
      */
      GLfloat& operator [] ( int i );

      /**
        * Const Index Operator (Square Brackets)
        *
        * allows indexing of vectors
        *
        * @param i item in array to access
        * @return const value of index
      */
      GLfloat operator [] ( int i ) const;

      /**
        * Unary Subtraction Operator
        *
        * allows negation of vectors
      */
      vec2 operator - () const; // unary minus operator

      /**
        * Vec2 Addition Operator
        *
        * allows addition of vectors
        * 
        * @param v input vector to be added
        * @return vector added to this
      */
      vec2 operator + ( const vec2& v ) const;

      /**
        * Vec2 Subtraction Operator
        *
        * allows subtraction of vectors
        * 
        * @param v input vector to be subtracted
        * @return vector subtracted from this
      */
      vec2 operator - ( const vec2& v ) const;

      /**
        * Scalar Multiplication Operator
        *
        * allows Multiplication with scalars
        * 
        * @param s input scalar to be multiplied
        * @return vector multiplied with this
      */
      vec2 operator * ( const GLfloat s ) const;

      /**
        * Vectors Multiplication Operator
        *
        * allows Multiplication with Vectors
        * 
        * @param v input vector to be multiplied
        * @return vector multiplied with this
      */
      vec2 operator * ( const vec2& v ) const;

      /**
        * Other Multiplication Operator
        *
        * allows Multiplication with Vectors and scalars
        * 
        * @param s input scalar to be multiplied
        * @param v input vector to be multiplied
        * @return vector multiplied with this
      */
      friend vec2 operator * ( const GLfloat s, const vec2& v );

      /**
        * Scalar Division Operator
        *
        * allows vector division by a scalar
        *
        * @param s Scalar as the divisor
        * @return vector output
      */
      vec2 operator / ( const GLfloat s ) const;

      /**
        * Vector Add Equal Operator
        *
        * allows a vector to be added to it
        *
        * @param v vector to be added to it
        * @param returns this vector
      */
      vec2& operator += ( const vec2& v );

      /**
        * Vector Minus Equal Operator
        *
        * allows a vector to be subtracted from it
        *
        * @param v vector to be subtracted from it
        * @param returns this vector
      */
      vec2& operator -= ( const vec2& v );


      /**
        * Vector Times Equal Operator
        * 
        * @param[in] s Scalar to be multiplied
        * @return multiplied vector
      */
      vec2& operator *= ( const GLfloat s )
      { x *= s;  y *= s;   return *this; }

      /**
        * Vector Times Equal Operator
        * 
        * @param[in] v Vector to be multiplied
        * @return multiplied vector
      */
      vec2& operator *= ( const vec2& v )
      { x *= v.x;  y *= v.y; return *this; }

      /**
        * Vector Division Equal Operator
        * 
        * @param[in] s Scalar to be divided
        * @return divided vector
      */
      vec2& operator /= ( const GLfloat s ) {
         if ( std::fabs(s) < DivideByZeroTolerance ) {
            std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
               << "Division by zero" << std::endl;
         }

         GLfloat r = GLfloat(1.0) / s;
         *this *= r;

         return *this;
      }

      //
      //  --- Insertion and Extraction Operators ---
      //

      /**
        * Extraction Operator
        * 
        * @param[in] os output stream
        * @param[in] v Vector to output
        * @return output stream
      */
      friend std::ostream& operator << ( std::ostream& os, const vec2& v ) {
         return os << "( " << v.x << ", " << v.y <<  " )";
      }

      /** 
        * Insertion Operator
        * 
        * @param[in] is input stream
        * @param[in] vector to input
        * @return input stream
      */
      friend std::istream& operator >> ( std::istream& is, vec2& v )
      { return is >> v.x >> v.y ; }

      
      /** Conversion Operator
        * @return GLfloat pointer
      */
      operator const GLfloat* () const
      { return static_cast<const GLfloat*>( &x ); }

      /** Conversion Operator
        * @return GLfloat pointer
      */
      operator GLfloat* ()
      { return static_cast<GLfloat*>( &x ); }
};

/**
  * Dot Product between two vectors
  *
  * @param u First vector
  * @param v Second vector
  * @return Floating point output
*/
inline GLfloat dot( const vec2& u, const vec2& v ) { return u.x * v.x + u.y * v.y; }

/**
  * Length of a 2D vector
  *
  * @param v input vector
  * @return resulting length
*/
inline GLfloat length( const vec2& v ) { return std::sqrt( dot(v,v) ); }

/**
  * Normalize a vector to unit length
  *
  * @param input vector
  * @return unit length vector
*/
inline vec2 normalize( const vec2& v ) { return v / length(v); }

//////////////////////////////////////////////////////////////////////////////
//
//  vec3.h - 3D vector
//
//////////////////////////////////////////////////////////////////////////////

class vec3 {

    public:
   /**  x/First paramter  */
   GLfloat  x;
   /**  y/Second parameter */
   GLfloat  y;
   /**  z/Third paramter  */
   GLfloat  z;

   //
   //  --- Constructors and Destructors ---
   //

   /** Default/Parameterized Constructor
     * @param[in] s Default Constructor
   */
   vec3( GLfloat s = GLfloat(0.0) );



   /** Parameterized Constructor
     * @param[in] x x/First parameter 
     * @param[in] y y/Second parameter
     * @param[in] z z/Third parameter
   */
   vec3( GLfloat x, GLfloat y, GLfloat z );


   /** Copy Constructor
     * @param[in] v Vector to be copied
   */
   vec3( const vec3& v );

   /** Copy Constructor
     * @param[in] v Vector to be copied
     * @param[in] f value of z
   */
   vec3( const vec2& v, const float f );


   /**
     * Indexing Operator
     * 
     * @param[in] i Index to be accessed
     * @return value of the indexed array 
   */
   GLfloat& operator [] ( int i );
   
   /**
     * Indexing Operator
     * 
     * @param[in] i Index to be accessed
     * @return value of the indexed array 
   */
   const GLfloat operator [] ( int i ) const;


   //
   //  --- (non-modifying) Arithematic Operators ---
   //

   /**
     * Unary Negation Operator
     * 
     * @return negated vector
   */
   vec3 operator - () const;  // unary minus operator


   /**
     * Addition Operator
     * 
     * @param[in] v vector to be added
     * @return added vector
   */
   vec3 operator + ( const vec3& v ) const;


   /**
     * Subtraction Operator
     * 
     * @param[in] v vector to be subtracted
     * @return subtracted vector
   */
   vec3 operator - ( const vec3& v ) const;


   /**
     * Multiplication Operator
     *
     * @param[in] s scalar to be multiplied
     * @return Multiplied Vector
   */
   vec3 operator * ( const GLfloat s ) const;


   /**
     * Multiplication Operator
     *
     * @param[in] v Vector to be multiplied
     * @return Multiplied Vector
   */
   vec3 operator * ( const vec3& v ) const;


   /**
     * Multiplication Operator
     *
     * @param[in] s Scalar to be multiplied
     * @param[in] v Vector to be multiplied
     * @return Multiplied Vector
   */
   friend vec3 operator * ( const GLfloat s, const vec3& v );


   /**
     * Division Operator
     * 
     * @param[in] s scalar to be divided 
     * @return return vector
   */
   vec3 operator / ( const GLfloat s ) const;
   
   //
   //  --- (modifying) Arithematic Operators ---
   //

   vec3& operator += ( const vec3& v );
   vec3& operator -= ( const vec3& v );
   vec3& operator *= ( const GLfloat s );

   /**
     * Vector Times Equal Operator
     * 
     * @param[in] v Vector to be multiplied
     * @return multiplied vector
   */
   vec3& operator *= ( const vec3& v );
   vec3& operator /= ( const GLfloat s );

   /** Extraction Operator
     * @param[in] os output stream
     * @param[in] v Vector to output
     * @return output stream
   */
   friend std::ostream& operator << ( std::ostream& os, const vec3& v );

   /** Insertion Operator
     * @param[in] is input stream
     * @param[in] v Vector to input
     * @return input stream
   */
   friend std::istream& operator >> ( std::istream& is, vec3& v );


};

//----------------------------------------------------------------------------
//
//  Non-class vec3 Methods
//

/**
  * Dot Product
  * @param[in] u first vector
  * @param[in] v second vector
  * @return dot product of the two vectors
*/
GLfloat dot( const vec3& u, const vec3& v );


/**
  * Vector magnitude
  * @param[in] v vector to be measured
  * @return length of vector
*/
inline GLfloat length( const vec3& v ) {
   return std::sqrt( dot(v,v) );
}

/**
  * Normalize Vector
  * @param[in] v Vector to be normalized
  * @return normalized vector
*/
inline vec3 normalize( const vec3& v ) {
   return v / length(v);
}


/**
  * Cross Product
  * @param[in] a first vector
  * @param[in] b second vector
  * @return result from cross product
*/
inline vec3 cross(const vec3& a, const vec3& b );


//////////////////////////////////////////////////////////////////////////////
//
//  vec4 - 4D vector
//
//////////////////////////////////////////////////////////////////////////////

struct vec4 {

   /** x coordinate */
   GLfloat  x;
   /** y coordinate */
   GLfloat  y;
   /** z coordinate */
   GLfloat  z;
   /** w or last coordinate */
   GLfloat  w;

   //
   //  --- Constructors and Destructors ---
   //

   /** Default/ Parameterized Constructor 
     * @param[in] s initial value
   */
   vec4( GLfloat s = GLfloat(0.0) ) :
      x(s), y(s), z(s), w(s) {}

   /** Default/ Parameterized Constructor
     * @param[in] x X/first coordinate
     * @param[in] y Y/second coordinate
     * @param[in] z Z/third coordinate
     * @param[in] w W/fourth coordinate
   */
   vec4( GLfloat x, GLfloat y, GLfloat z, GLfloat w ) :
      x(x), y(y), z(z), w(w) {}
   
   /**
     * Copy Constructor
     * @param[in] v Vector to be copied
   */
   vec4( const vec4& v ) { x = v.x;  y = v.y;  z = v.z;  w = v.w; }

   /** 
     * Copy Constructor
     * @param[in] v Vector to be copied
     * @param[in] w homogenous coordinate
   */
   vec4( const vec3& v, const float w = 1.0 );

   /**
     * Copy Constructor
     * @param[in] v Vector to be copied
     * @param[in] z third coordinate
     * @param[in] w fourth coordinate
   */
   vec4( const vec2& v, const float z, const float w ) : z(z), w(w)
   { x = v.x;  y = v.y; }

   /**
     * Indexing Operator
     * 
     * @param[in] i Index to be accessed
     * @return value of the indexed array 
   */
   GLfloat& operator [] ( int i ) { return *(&x + i); }
   /**
     * Const Index Operator
     *
     * @param[in] i Index to be accessed
     * @return value of the indexed array
   */
   const GLfloat operator [] ( int i ) const { return *(&x + i); }

   /**
     * Unary negation operator
     *
     * @return negated vector
   */
   vec4 operator - () const  // unary minus operator
   { return vec4( -x, -y, -z, -w ); }

   /**
     * Addition Operator
     *
     * @param[in] v vector to be added
     * @return vector added with the input
   */
   vec4 operator + ( const vec4& v ) const
   { return vec4( x + v.x, y + v.y, z + v.z, w + v.w ); }

   /**
     * Subtraction Operator
     *
     * @param[in] v vector to be subtracted
     * @return vector subtracted with the input
   */
   vec4 operator - ( const vec4& v ) const
   { return vec4( x - v.x, y - v.y, z - v.z, w - v.w ); }

   /**
     * Multiplication Operator
     *
     * @param[in] s scalar to be multiplied
     * @return Multiplied Vector
   */
   vec4 operator * ( const GLfloat s ) const
   { return vec4( s*x, s*y, s*z, s*w ); }

   /**
     * Multiplication Operator
     *
     * @param[in] v Vector to be multiplied
     * @return vector multiplied
   */
   vec4 operator * ( const vec4& v ) const
   { return vec4( x*v.x, y*v.y, z*v.z, w*v.w ); }

   /**
     * Multiplication Operator
     *
     * @param[in] s Scalar to be multiplied
     * @param[in] v Vector to be multiplied
     * @return vector multiplied
   */
   friend vec4 operator * ( const GLfloat s, const vec4& v )
   { return v * s; }

   /**
     * Division Operator
     * 
     * @param[in] s scalar to be divided 
     * @return return vector
   */
   vec4 operator / ( const GLfloat s ) const {
      if ( std::fabs(s) < DivideByZeroTolerance ) {
         std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
            << "Division by zero" << std::endl;
         return vec4();
      }

      GLfloat r = GLfloat(1.0) / s;
      return *this * r;
   }

   //
   //  --- (modifying) Arithematic Operators ---
   //

   /** Addition Equal Operator
     * @param[in] v vector to be added
     * @return added vector
   */
   vec4& operator += ( const vec4& v )
   { x += v.x;  y += v.y;  z += v.z;  w += v.w;  return *this; }

   /**  Minus Equal Operator
     * @param[in] v Vector to be subtracted
     * @return subtracted vector
   */
   vec4& operator -= ( const vec4& v )
   { x -= v.x;  y -= v.y;  z -= v.z;  w -= v.w;  return *this; }

   /** Times Equal Operator
     * @param[in] s Scalar 
     * @return multiplied vector
   */
   vec4& operator *= ( const GLfloat s )
   { x *= s;  y *= s;  z *= s;  w *= s;  return *this; }

   /** Times Equal Operator
     * @param[in] v Vector
     * @return multiplied vector
   */
   vec4& operator *= ( const vec4& v )
   { x *= v.x, y *= v.y, z *= v.z, w *= v.w;  return *this; }

   /** Division Equal Operator
     * @param[in] s Scalar 
     * @return scalar divided vector
   */
   vec4& operator /= ( const GLfloat s ) {
      if ( std::fabs(s) < DivideByZeroTolerance ) {
         std::cerr << "[" << __FILE__ << ":" << __LINE__ << "] "
            << "Division by zero" << std::endl;
      }

      GLfloat r = GLfloat(1.0) / s;
      *this *= r;

      return *this;
   }

   //
   //  --- Insertion and Extraction Operators ---
   //

   /** Extraction Operator
     * @param[in] os output stream
     * @param[in] v Vector to output
     * @return output stream
   */
   friend std::ostream& operator << ( std::ostream& os, const vec4& v ) {
      return os << "( " << v.x << ", " << v.y
         << ", " << v.z << ", " << v.w << " )";
   }

   /** Insertion Operator
     * @param[in] is input stream
     * @param[in] v Vector to input
     * @return input stream
   */
   friend std::istream& operator >> ( std::istream& is, vec4& v )
   { return is >> v.x >> v.y >> v.z >> v.w; }

   //
   //  --- Conversion Operators ---
   //

   /** Conversion Operator
     * @return GLfloat pointer
   */
   operator const GLfloat* () const
   { return static_cast<const GLfloat*>( &x ); }

   /** Conversion Operator
     * @return GLfloat pointer
   */
   operator GLfloat* ()
   { return static_cast<GLfloat*>( &x ); }
};

//----------------------------------------------------------------------------
//
//  Non-class vec4 Methods
//

/**
  * Dot Product
  * @param[in] u first vector
  * @param[in] v second vector
  * @return dot product of the two vectors
*/
inline GLfloat dot( const vec4& u, const vec4& v ) {
   return u.x*v.x + u.y*v.y + u.z*v.z + u.w+v.w;
}

/**
  * Vector magnitude
  * @param[in] v vector to be measured
  * @return length of vector
*/
inline GLfloat length( const vec4& v ) {
   return std::sqrt( dot(v,v) );
}

/**
  * Normalize Vector
  * @param[in] v Vector to be normalized
  * @return normalized vector
*/
inline vec4 normalize( const vec4& v ) {
   return v / length(v);
}

/**
  * Cross Product
  * @param[in] a first vector
  * @param[in] b second vector
  * @return result from cross product
*/
inline vec3 cross(const vec4& a, const vec4& b ){
   return vec3( a.y * b.z - a.z * b.y,
         a.z * b.x - a.x * b.z,
         a.x * b.y - a.y * b.x );
}

//----------------------------------------------------------------------------
typedef vec4 color4;
typedef vec4 point4;

inline GLfloat vdistance( const vec4& u, const vec4& v ){
   return sqrt( ( u.x-v.x)*(u.x-v.x) + (u.y-v.y)*(u.y-v.y) + (u.z-v.z)*(u.z-v.z));
}

#endif
