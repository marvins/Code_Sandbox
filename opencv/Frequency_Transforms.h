//Image Processing Library
#ifndef __FREQUENCY_TRANSFORMS_H__
#define __FREQUENCY_TRANSFORMS_H__

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <complex>
#include <iomanip>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>

using namespace cv;
using namespace std;

typedef unsigned long ulong;

enum shifting { SHIFT = 0, NO_SHIFT = 1 };

/******************************************/
/*  Templated Swap Function               */
/*  - swaps the positions of items passed */
/******************************************/
template <typename TP> 
void _swap( TP& a, TP& b ){
   TP temp = a;
   a = b;
   b = temp;
}
/******************************************/


void fft(vector<float>& data, unsigned long nn, int isign);

Mat_<complex<float> > fft_2D(Mat const& imagein,const int shift);

Mat fft_2D_inverse(Mat_<complex<float> >const& imagein,const int shift);

#endif
