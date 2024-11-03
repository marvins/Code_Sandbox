#include "Frequency_Transforms.h"

/*********************************************************/
/*   1D  Fast Fourier Transform                          */
/*   - Performs a 1 dimensional FFT on the input vector. */
/*     Be aware that it uses the initial index from 1.   */
/*********************************************************/
void fft(vector<float>& data, unsigned long nn, int isign)
{
	unsigned long n,mmax,m,j,istep,i;
	double wtemp,wr,wpr,wpi,wi,theta;
	float tempr,tempi;

	n=nn << 1;
	j=1;
	for (i=1;i<n;i+=2) {
		if (j > i) {
			_swap(data[j],data[i]);
			_swap(data[j+1],data[i+1]);
		}
		m=n >> 1;
		while (m >= 2 && j > m) {
			j -= m;
			m >>= 1;
		}
		j += m;
	}
	mmax=2;
	while (n > mmax) {
		istep=mmax << 1;
		theta=isign*(6.28318530717959/mmax);
		wtemp=sin(0.5*theta);
		wpr = -2.0*wtemp*wtemp;
		wpi=sin(theta);
		wr=1.0;
		wi=0.0;
		for (m=1;m<mmax;m+=2) {
			for (i=m;i<=n;i+=istep) {
				j=i+mmax;
				tempr=wr*data[j]-wi*data[j+1];
				tempi=wr*data[j+1]+wi*data[j];
				data[j]=data[i]-tempr;
				data[j+1]=data[i+1]-tempi;
				data[i] += tempr;
				data[i+1] += tempi;
			}
			wr=(wtemp=wr)*wpr-wi*wpi+wr;
			wi=wi*wpr+wtemp*wpi+wi;
		}
		mmax=istep;
	}
}

/*********************************************************/
/*   2D Fast Fourier Transform                           */
/*   - Resizes the image to the proper dimensions, then  */
/*     calls the 1D version on each row then column      */
/*********************************************************/
Mat_<complex<float> > fft_2D(Mat const& imagein,const int shift){

  //Misc. Variables
  int COLS = imagein.cols;
  int ROWS = imagein.rows;
  
  //Check for Even Multiple of 2
  int n = pow(2.0,ceil(log2(COLS)));
  int m = pow(2.0,ceil(log2(ROWS)));
  
  //Resize if necessary
  if(n != COLS)
    COLS = n;
  if(m != ROWS)
    ROWS = m;
  
  //Create Complex Image  
  Mat_<complex<float> > image(ROWS,COLS);
  Mat_<complex<float> > temp1(ROWS,COLS);
  Mat_<complex<float> > temp2(ROWS,COLS);

  //InMavert Image
  for(int i=0;i<COLS;i++)
    for(int j=0;j<ROWS;j++){
      if(i < imagein.cols && j < imagein.rows && shift == SHIFT)
        image(Point(i,j)) = complex<float>(imagein.at<float>(Point(i,j))*pow(-1.0,i+j),0);
      else if(i < imagein.cols && j < imagein.rows)
        image(Point(i,j)) = complex<float>(imagein.at<float>(Point(i,j)),0);
      else
        image(Point(i,j)) = complex<float>(0,0);
     }
   
  vector<float> row(2*COLS);
  vector<float> col(2*ROWS);
  
  //FFT Columns
  //Loop for every V in F(u,v)
  for(int v=0;v<ROWS;v++){      
    //Copy every row into vector
    row.clear();
    row.push_back(0);
    for(int x=0;x<COLS;x++){
      row.push_back(image(Point(x,v)).real());
      row.push_back(image(Point(x,v)).imag());}

    //call fft
    fft(row,(ulong)COLS,-1);

    for(int x=0;x<COLS;x++)
       temp1(Point(x,v)) = complex<float>(row[2*x+1],row[2*x+2]);
    }
    
  //FFT Rows
  //Loop for every U in F(u,v)
  for(int u=0;u<ROWS;u++){      
    //Copy every row into vector
    col.clear();
    col.push_back(0);
    for(int y=0;y<ROWS;y++){
      col.push_back(temp1(Point(u,y)).real());
      col.push_back(temp1(Point(u,y)).imag());}

    //call fft
    fft(col,(ulong)ROWS,-1);

    for(int y=0;y<ROWS;y++)
       temp2(Point(u,y)) = complex<float>(col[2*y+1],col[2*y+2]);
  }    
  
  return temp2;
}


/*****************************************************************/
/*                             FFT_2D_INVERSE                    */
/*****************************************************************/
Mat fft_2D_inverse(Mat_<complex<float> >const& imagein,const int shift){
   
   //Ouput Image
   Mat imageout( Size( imagein.cols, imagein.rows), CV_32FC1);

   //Misc. Variables
   int COLS = imagein.cols;
   int ROWS = imagein.rows;

   //Check for Even Multiple of 2
   int n = pow(2.0,ceil(log2(COLS)));
   int m = pow(2.0,ceil(log2(ROWS)));
  
   //Resize if necessary
   if(n != COLS)
     COLS = n;
   if(m != ROWS)
     ROWS = m;
  
  //Create Complex Images
  Mat_<complex<float> > temp(ROWS,COLS);
  vector<float> row(2*COLS);
  vector<float> col(2*ROWS);
  
  
  //FFT Columns
  //Loop for every V in F(u,v)
  for(int v=0;v<ROWS;v++){      
    //Copy every row into vector
    row.clear();
    row.push_back(0);
    for(int x=0;x<COLS;x++){
      row.push_back(imagein(Point(x,v)).real());
      row.push_back(imagein(Point(x,v)).imag());}

    //call fft
    fft(row,(ulong)COLS,1);

    for(int x=0;x<COLS;x++)
       temp(Point(x,v)) = complex<float>(row[2*x+1],row[2*x+2]);
    }
    
  //FFT Rows
  //Loop for every U in F(u,v)
  for(int u=0;u<COLS;u++){      
    //Copy every row into vector
    col.clear();
    col.push_back(0);
    for(int y=0;y<ROWS;y++){
      col.push_back(temp(Point(u,y)).real());
      col.push_back(temp(Point(u,y)).imag());}

    //call fft
    fft(col,(ulong)ROWS,1);

    //Insert into return image, round, and remove inversion
    for(int y=0;y<ROWS;y++){
      if(shift == SHIFT)
        imageout.at<float>(Point(u,y)) = (col[2*y+1])/((float)(COLS*ROWS))*pow(-1.0,u+y);
      else
        imageout.at<float>(Point(u,y)) = (col[2*y+1])/((float)(COLS*ROWS));
    }
  }    
  
  return imageout;
}

