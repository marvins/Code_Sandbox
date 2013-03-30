#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

int range = 12;

void reset_array(vector<long long>&,int);
void calc_pow(vector<long long>&,int);
void add_array(vector<long long>&,vector<long long> const&);
void print(vector<long long>const&);

int main()
{
    vector<long long> array1(range);
    vector<long long> array2(range);
    long long sum=1;
    
    reset_array(array2,1);
    for(int i=2;i<=1000;i++){
      reset_array(array1,i);
      calc_pow(array1,i);
      add_array(array2,array1);
    }
    print(array2);
    return 0;
}
void reset_array(vector<long long>& val,int s)
{
  val[0]=s;
  for(int i=1;i<range;i++)
    val[i]=0;
}
void calc_pow(vector<long long>& val,int s){
  for(int i=1;i<s;i++){
    val[0] *= s;
    for(int j=1;j<range;j++){
      val[j] *= s;
      if(val[j-1]>=10){
        val[j] += val[j-1]/10;
        val[j-1]= val[j-1]%10;
      }
    }
  } 
}
void add_array(vector<long long>& val, vector<long long> const& array){
  for(int i=0;i<range;i++){
    val[i] += array[i];
    if(i!=0){
      if(val[i-1]>=10){
        val[i] += val[i-1]/10;
        val[i-1]= val[i-1]%10;
      }
    }
  }
}
void print(vector<long long>const& val)
{
  for(int i=9;i>=0;i--)
    cout << val[i];
  cout << endl;
}

