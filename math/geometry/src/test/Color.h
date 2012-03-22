#ifndef __UNIT_COLOR_H__
#define __UNIT_COLOR_H__

#include <iostream>
#include <math.h>

using namespace std;

enum COLORS  { RED = 31, YELLOW = 33, GREEN = 32, BLUE = 34 };

#define color_end "\033[0m"

class Color
{
public:
    Color(int v);
    
    friend ostream& operator<<( ostream& os, const Color& mp);

private:
    int val;
};

#endif
