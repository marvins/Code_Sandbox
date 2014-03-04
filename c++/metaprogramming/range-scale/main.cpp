/**
 * @file    main.cpp
 * @author  Marvin Smith
 * @date    3/3/2014
 */
#include <iostream>

using namespace std;


template <typename TP>
class RangeType{
    
    public:

        /// Type of value
        typedef TP rangetype;

        /// Max Value
        static const rangetype maxValue = 1;

        /// Min Value 
        static const rangetype minValue = 0;

}; /// End of RangeType class

template<>
class RangeType<double>{

    public:
        
        /// Type of value
        typedef double rangetype;

        /// Max value
        static const rangetype maxValue = 1;

        /// Min value
        static const rangetype minValue = 0;
}; 

template<>
class RangeType<unsigned char>{

    public:
        
        /// Type of value
        typedef unsigned char rangetype;

        /// Max value
        static const rangetype maxValue = 255;

        /// Min Value
        static const rangetype minValue = 0;

}; 

typedef RangeType<double> RangeDouble;
typedef RangeType<unsigned char> RangeUInt8;

/**
 * Range scale
 */
template <typename RType1, typename RType2>
typename RType1::rangetype range_scale( typename RType2::rangetype const& value ){
    return ((((static_cast<double>(value) - RType2::minValue)/(RType2::maxValue - RType2::minValue)) * ( RType1::maxValue - RType1::minValue)) + RType1::minValue);
}


/**
 * Main Function
 */
int main( int argc, char* argv[] ){

    unsigned char value = 127;
    double result01 = range_scale<RangeDouble, RangeUInt8>( value ); 
    
    unsigned char result02 = range_scale<RangeUInt8, RangeDouble>( result01 );

    cout << "Result01: " << result01 << endl;
    cout << "Result02: " << (int)result02 << endl;

    return 0;
}
