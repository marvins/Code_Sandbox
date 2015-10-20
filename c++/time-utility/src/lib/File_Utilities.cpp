/**
 * @file    File_Utilities.cpp
 * @author  Marvin Smith
 * @date    10/19/2015
 */
#include "File_Utilities.hpp"

// C++ Libraries
#include <fstream>
#include <iostream>
#include <streambuf>

namespace TM{

/********************************/
/*       Load Binary File       */
/********************************/
std::string Load_Binary_File( const std::string& pathname )
{
    // Open File
    std::ifstream fin;
    fin.open(pathname.c_str());

    // Get the file size
    std::string output;

    // Get the file size
    fin.seekg(0, std::ios::end);   
    output.reserve(fin.tellg());
    fin.seekg(0, std::ios::beg);

    // Assign contents
    output.assign((std::istreambuf_iterator<char>(fin)),
                   std::istreambuf_iterator<char>());

    // Close File
    fin.close();

    // return contents
    return output;
}


/*****************************/
/*       Parse CSV File      */
/*****************************/
std::vector<std::vector<std::string>> Parse_CSV_File( const std::string&        pathname,
                                                      std::vector<std::string>& headers,
                                                      const bool&               load_header )
{
    // Load data
    std::string data = Load_Binary_File(pathname);
    
    // Initialize Flags and Temp Variables
    bool inQuote(false);
    bool new_line(false);
    std::string field;
    std::vector<std::string> line;

    // Create output
    std::vector<std::vector<std::string>> output;


    // Start the iterator
    bool first_line_complete = false;
    std::string::const_iterator aChar = data.begin();
    while (aChar != data.end())
    {
        // Check for newline
        switch (*aChar)
        {
            // Look for Quotes
            case '"':
                new_line = false;
                inQuote = !inQuote;
                break;

            // Look for Commas
            case ',':
                new_line = false;
                if (inQuote == true)
                {
                    field += *aChar;
                }
                else
                {
                    line.push_back(field);
                    field.clear();
                }
                break;

            // Look for newline / Carriage Return
            case '\n':
            case '\r':

                // Process data inside the quote
                if (inQuote == true)
                {
                    field += *aChar;
                }

                // Otherwise, we have a newline
                else
                {
                    if (new_line == false)
                    {
                        line.push_back(field);
                        if( load_header         == true &&
                            first_line_complete == false )
                        {
                            headers = line;
                            first_line_complete = true;
                        }
                        else
                        {
                            output.push_back(line);
                        }
                        field.clear();
                        line.clear();
                        new_line = true;
                    }
                }
                break;

            // Default Condition, Add to current field
            default:
                new_line = false;
                field.push_back(*aChar);
                break;
        }
        
        // Increment the Iterator
        aChar++;

        // Check the field
        if (field.size() && new_line == true )
        {
            line.push_back(field);
        }

        // Add the entry to the output
        if (line.size() && new_line == true )
        {
            if( first_line_complete == true ){
                output.push_back(line);
            }
            else{
                headers = line;
            }
        }

        // Check if the first line has been finished yet
        if( new_line            == true && 
            first_line_complete == false )
        {
            first_line_complete = true;
        }

    }

    // Return results
    return output;
}

} // End of TM Namespace
