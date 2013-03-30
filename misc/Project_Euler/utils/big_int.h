#ifndef __BIG_INT_H__
#define __BIG_INT_H__

#include <iostream>
#include <vector>

using namespace std;

class big_int{

  public:
    //constructors
    big_int():array(0){}
    big_int( const string& data ):array(data.size()){
        //input data into string
        for(int i=data.size()-1; i>=0; i--)
          array[i] = data[i] - 48;
    }

    void Print( ){
      for(int i=array.size()-1; i>=0; i++)
        cout << array[i];
      cout << endl;
    }

  private:
    vector<int> array;      


};

#endif
