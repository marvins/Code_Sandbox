#ifndef __PRIME_SIEVE_H__
#define __PRIME_SIEVE_H__

#include <iostream>
#include <cmath>
#include <vector>
#include <sstream>

using namespace std;

/**
 *  @class Prime Sieve Class
 *  
 *  Implements a basic prime sieve
 */
class Primes{

    public:
        
        /**
         * Default Constructor
         */
        Primes( long int maxval, bool debug = false);

    void prime_start() { pos = 2; }
    long int prime_next(bool&);
    bool is_prime(const long int&) const;

  private:
    string status(long int x, long int y)const;
    vector<bool> data;
    const long int MAX;
    long int pos;
};

Primes::Primes(long int m,bool flag) : MAX(m)
{
      long int root = static_cast<long int>(ceil(sqrt(static_cast<double>(MAX))));
      pos = 1;
      if(flag)cout << "Starting Library Construction" << endl;

      data.begin();
      for(long int i=0;i<MAX;i++)
        data.push_back(true);
      
      data[0]=false;
      for(long int i=1;i<=root;i++)
      {
        if(data[i-1])
          for( long int j = i*2; j <= MAX; j+= i)
            data[j-1] = false;
          if(flag) cout << status(i,root);
      }
      if(flag) cout << endl;
}

bool Primes::is_prime(const long int& num) const
{
    if(num > 0 && num < MAX)
    {
        return data[num-1];
    }
    else
      return false;
}

long int Primes::prime_next( bool& end ) 
{
    bool point = false;
    long int value,num;
    
    if( pos < data.size() )
    {
      point = is_prime(value);
      while(!is_prime(pos) && (pos!=-1)){
        pos++;
        if( pos > MAX ){ end=true; return 0;}
      }

      pos++;

      if((pos-1) > data.size()||(pos == -1))
      {
        end = true;
        return 0;
      }
      else
      {
        end = false;
        return pos-1;
      }
    }
    end = true;
    return 0;
}

string Primes::status(long int x, long int y) const
{
    std::ostringstream buff;
    double z = x/(double)y;
    string output = " Building Library [";
    
    for(int i=0;i<40;i++)
      if((i/40.0)<(z))
        output += '*';
      else
        output += ' ';
    output += "] ";
    buff << ((x/(double)y)*100);
    output += buff.str();
    output += "% \r";
   
    return output;
}

#endif
