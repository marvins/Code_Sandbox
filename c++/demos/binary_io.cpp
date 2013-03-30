#include <cv.h>
#include <cvaux.h>
#include <highgui.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

using namespace cv;
using namespace std;

void binary_write(Mat a){

  float* array = new float[a.cols * a.rows];
  for(int i=0;i<a.rows;i++)
    for(int j=0;j<a.cols;j++)
      array[i*a.cols+j] = a.at<float>(Point(j,i));
  int MAX = a.cols * a.rows;

  ofstream ofp;

  ofp.open("image.bny", ios::out | ios::binary);
  ofp << a.cols << " " << a.rows << endl;
  

  ofp.write( (char *)array, MAX*sizeof(float));
  ofp.close();

}

void binary_read(Mat& a){

  ifstream ifp;
  ifp.open("image.bny", ios::in | ios::binary);

  int row, col;
  char* names = new char[100];
  char* ptr;
 
  ifp.getline(names,100,'\n');
  
  col = strtol(names,&ptr,0);
  row = atoi(ptr);
  int MAX = row * col;
  a.create(row,col,CV_32FC1);

  float* array;
  array = new float[row*col];

  ifp.read( (char *)array, MAX*sizeof(float));

  ifp.close();

  for(int i=0;i<row;i++)
    for(int j=0;j<col; j++)
      a.at<float>(Point(j,i)) = array[i*col + j];
}


int main(int argc, char* argv[]){

  Mat imgD = imread("photo.jpg",0); 
  Mat imgA;

  imgD.convertTo(imgA,CV_32FC1);

  binary_write(imgA);

  Mat imgB;
  binary_read(imgB);
  
  Mat imgC;
  imgB.convertTo(imgC, CV_8UC1);

  namedWindow("AA");
  imshow("AA",imgC);
  waitKey(0);

  return 0;
}
