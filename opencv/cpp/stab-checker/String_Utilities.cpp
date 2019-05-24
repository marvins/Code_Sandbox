/**
 * @file    String_Utilities.cpp
 * @author  Marvin Smith
 * @date    1/21/2019
 */
#include "String_Utilities.hpp"

/// Default String Trim Pattern
const std::string DEFAULT_TRIM_PATTERN = " \n\0";


/***************************************/
/*             Split String            */
/***************************************/
std::vector<std::string> String_Split( const std::string& data,
                                       const std::string& pattern )
{
    // Create output
    std::vector<std::string>  output, result;

    // Perform split
    boost::split( output, data, boost::is_any_of(pattern));

    for( int i=0; i<(int)output.size(); i++ ){
        if( String_Trim(output[i]).size() > 0 ){
            result.push_back(String_Trim(output[i]));
        }
    }

    // return value
    return result;
}



/************************************/
/*         Strip A String           */
/************************************/
std::string String_Trim( const std::string&  data )
{
    return String_Trim( data,
                        DEFAULT_TRIM_PATTERN,
                        StringDirection::BOTH );
}


/************************************/
/*         Strip A String           */
/************************************/
std::string String_Trim( const std::string&  data,
                         const std::string&  strip_pattern )
{
    return String_Trim( data,
                        strip_pattern,
                        StringDirection::BOTH );
}

/************************************/
/*         Strip A String           */
/************************************/
std::string String_Trim( const std::string&  data,
                         const StringDirection& strip_direction )
{
    return String_Trim( data, DEFAULT_TRIM_PATTERN, strip_direction);
}

/************************************/
/*         Strip A String           */
/************************************/
std::string String_Trim( const std::string&  data,
                         const std::string&  patterns,
                         const StringDirection& strip_direction )
{
    // Skip empty strings
    if( data.size() <= 0 ){
        return data;
    }
    if( patterns.size() <= 0 ){
        return data;
    }

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
/*          Format a String         */
/************************************/
std::string Format_String( const std::string&  input_data,
                           const uint64_t&     max_size )
{
    // Check size
    if( input_data.size() == max_size ){
        return input_data;
    }

    if( input_data.size() > max_size )
    {
        return input_data.substr(0, max_size);
    }

    // Create output string
    int leftover = max_size - input_data.size();

    return (input_data + std::string(leftover, ' '));
}