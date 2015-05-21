/**
 * @file    String_Utilities.cpp
 * @author  Marvin Smith
 * @date    5/20/2015
 */
#include "String_Utilities.hpp"

// Boost Libraries
#include <boost/algorithm/string.hpp>

namespace UTILS{

/************************************/
/*         Strip A String           */
/************************************/
std::string String_Trim( const std::string&  data,
                         const std::string&  patterns,
                         const StringDirection& strip_direction )
{
    // Set our output
    std::string output = data;

    // trim from start
    if( strip_direction == StringDirection::LEFT ||
        strip_direction == StringDirection::BOTH )
    {
        boost::trim_left_if( output, boost::is_any_of(patterns));
    }

    // trim right
    if( strip_direction == StringDirection::RIGHT || 
        strip_direction == StringDirection::BOTH )
    {
        boost::trim_right_if( output, boost::is_any_of(patterns));
    }

    return output;
}


/************************************/
/*          Fill A String           */
/************************************/
std::string String_Fill( const std::string&      data,
                         const char&             fill_char,
                         const int&              fill_cnt,
                         const StringDirection&  fill_direction )
{
    // Create padding
    std::string padding( fill_cnt, fill_char);

    // If left
    if( fill_direction == StringDirection::LEFT ){
        return (padding + data);
    }

    // If Right
    if( fill_direction == StringDirection::RIGHT ){
        return (data + padding);
    }

    // Return nothing different
    return data;
}


/***************************************/
/*          Format a String            */
/***************************************/
std::string Format_String( const std::string&      data,
                           const int&              output_size,
                           const StringAlignment&  alignment,
                           const char&             fill_char )
{
    // Create output string
    std::string fixed_data = data;
    std::string output;

    // Fill difference
    const int fill_difference = output_size - data.size();

    // Check the data size, crop if necessary and return
    if( ((int)fixed_data.size()) > output_size ){
        fixed_data = fixed_data.substr(0,output_size);
        return fixed_data;
    }

    // If left alignment, this is pretty easy.  Just put the output down and fill left
    if( alignment == StringAlignment::LEFT ){
        output = String_Fill( fixed_data, 
                              fill_char, 
                              fill_difference, 
                              StringDirection::RIGHT );
        return output;
    }

    // If Right Alignment, put the string down and fill right
    if( alignment == StringAlignment::RIGHT ){
        output = String_Fill( fixed_data,
                              fill_char,
                              fill_difference,
                              StringDirection::LEFT );
        return output;
    }


    // If Center Alignment Fill with Half Each.  
    if( alignment == StringAlignment::CENTER ){

        // Fill Left First
        output = String_Fill( fixed_data,
                              fill_char,
                              fill_difference/2,
                              StringDirection::LEFT);
        
        // Fill Right and Return
        return String_Fill(  output,
                             fill_char,
                             output_size - output.size(),
                             StringDirection::RIGHT );

    }

    // Return junk if we get here
    return data;
}




}  // End of UTILS Namespace

