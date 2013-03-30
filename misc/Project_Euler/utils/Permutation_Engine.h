#include <iostream>
#include <stdio.h>
#include <vector>

using namespace std;

//FUNCTION PROTOTYPES
//compute pandigital numbers and load into Vector
template<typename _Tp1, typename _Tp2,typename _FuncT>   
void permute_pandigital(vector<_Tp1>&,vector<vector<_Tp1> >&,_Tp2,_Tp2,const bool,_FuncT);

//swap items - internal to permute_pandigital()
template<typename _Tp1,typename _Tp2>
void swap(vector<_Tp1>& , const _Tp2 , const _Tp2);

//rotate vector to left
template<typename _Tp1,typename _Tp2>
void rotate_left(vector<_Tp1>& , const _Tp2 , const _Tp2 );

//print the list if flag set
template<typename _Tp1,typename _Tp2>
void print( vector<_Tp1>& );




//FUNCTION DEFINITIONS
template<typename _Tp1, typename _Tp2, typename _FuncT>
void permute_pandigital( 
      vector<_Tp1>& list , vector<vector<_Tp1> >& results, const _Tp2 start, const _Tp2 end , const bool flag , _FuncT func){
   
   if(flag) print<_Tp1,_Tp2>(list);
   if(func(list))results.push_back(list);

   for(_Tp2 i = end - 2; i >= start ; i-- ){
     for(_Tp2 j = i + 1; j < end; j++ ){
       swap( list , i , j );
       permute_pandigital<_Tp1,_Tp2,_FuncT>( list , results , i+1 , end , flag , func);
     }
     rotate_left<_Tp1,_Tp2>( list , i , end );
   }
}



template<typename _Tp1 , typename _Tp2>
void swap(vector<_Tp1>& list, const _Tp2 pt1 , const _Tp2 pt2 )
{
    _Tp1 item;
    item = list[pt1];
    list[pt1] = list[pt2];
    list[pt2] = item;
}

template<typename _Tp1 , typename _Tp2>
void rotate_left(vector<_Tp1>& list, const _Tp2 start , const _Tp2 end )
{
  _Tp2 item = list[start];
  
  for( _Tp2 i = start ; i < end-1 ; i++ )
     list[i] = list[i+1];
  list[ end-1 ] = item;
}

template<typename _Tp1 , typename _Tp2>
void print( vector<_Tp1>& list )
{
   for( _Tp2 i = list.size()-1; i >= 0 ; i-- )
     cout << list[i];
  cout << " - " << endl;
}
