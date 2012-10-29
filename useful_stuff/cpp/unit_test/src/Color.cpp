#include "Color.hpp"

Color::Color(int v) : val(v) {}
    
ostream& operator<<( ostream& os, const Color& mp) 
{
   if(mp.val >= 0){
      os << "\033[0;" << mp.val << "m";
      return os;
   }
   else{
      os << "\033[1;" << fabs(mp.val) << "m";
      return os;
   }
}
