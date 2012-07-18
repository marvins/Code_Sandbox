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

GLfloat& vec2::operator [] ( int i ){ 
   return *(&x + i); 
}

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


