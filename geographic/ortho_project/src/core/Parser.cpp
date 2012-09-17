/**
 * @file Parser.cpp
 *
 * @Author Marvin Smith
 * @Date  09/15/2012
 *
 * Implementation of my basic parser.  Requires some boost. This is designed
 * to be extremely simple to use and provide only the baseline requirements 
 * such as command-line arguments and config file parsing. 
*/
#include "Parser.hpp"

///Boost Libraries
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>

///STL Requirements
#include <fstream>
#include <iostream>
#include <sstream>

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

using namespace std;

template <typename TP>
TP str2num( std::string const& value ){
    
    std::stringstream sin;
    TP result;
    sin << value;
    sin >> result;
    return result;
}

template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}

std::string bool2str( bool const& value ){

    if( value == false )
        return "false";
    else
        return "true";
}

bool str2bool( const string& value ){

    string temp;
    std::transform( value.begin(), value.end(), temp.begin(), ::tolower);

    if( value == "false")
        return false;
    else
        return true;
}


PSR::Parser::Parser( ){
    valid = false;
}


PSR::Parser::Parser( const string& filename ){

    //set to false just in case
    valid = false;
    
    //set some parameters
    config_filename = filename;

    //validate that the file we have is useable
    validate_file();

    //read all contents
    load_file();

}

/** Read the configuration file and command-line options */
PSR::Parser::Parser( const int argc, char ** argv, std::string const& filename ){
    
    vector<pair<string,string> > tlist;

    //set to false just in case
    valid = false;
    
    //set the program name
    command_name = argv[0];

    // set the config filename
    config_filename = filename;

    //read the command-line options
    size_t idx;
    string line, tag, val;
    for( int i=1; i<argc; i++){

        //load the string
        line = string(argv[i]);
        
        //split the line by the = sign
        idx = line.find_first_of("=");
        
        //if the index is past the length of the string, its a tag value not tag=value
        if( idx == string::npos ){

            //set the tag and val strings
            tag = line;
            
            if( argc <= i+1 ){ throw string("ERROR: Invalid number of arguments"); }
            val = argv[i+1];
            i++;
        }

        else{
            //set the tag and val strings
            tag = line.substr(0,idx);
            val = line.substr(idx+1);
        }

        // check all config options
        if( tag == "-c" || tag == "--config_file" ){
            config_filename = val;
        }
        
        //get rid of the starting dashes
        while( tag[0] == '-')
            tag = tag.substr(1);

        //add all remaining items to the temporary list
        tlist.push_back( pair<string,string>(tag, val));
    }
    
    //validate that the file we have is usable
    validate_file();

    //read all of the file contents
    load_file();
    
    //add all remaining items in the list to the item list
    for( size_t i=0; i<tlist.size(); i++)
        setItem_string( tlist[i].first, tlist[i].second, true);


}


/** Read the configuration file and command-line options */
void PSR::Parser::init( const int argc, char ** argv, std::string const& filename ){
    
    items.clear();

    vector<pair<string,string> > tlist;

    //set to false just in case
    valid = false;
    
    //set the program name
    command_name = argv[0];

    // set the config filename
    config_filename = filename;

    //read the command-line options
    size_t idx;
    string line, tag, val;
    for( int i=1; i<argc; i++){

        //load the string
        line = string(argv[i]);
        
        //split the line by the = sign
        idx = line.find_first_of("=");
        
        //if the index is past the length of the string, its a tag value not tag=value
        if( idx == string::npos ){

            //set the tag and val strings
            tag = line;
            
            if( argc <= i+1 ){ throw string("ERROR: Invalid number of arguments"); }
            val = argv[i+1];
            i++;
        }

        else{
            //set the tag and val strings
            tag = line.substr(0,idx);
            val = line.substr(idx+1);
        }

        // check all config options
        if( tag == "-c" || tag == "--config_file" ){
            config_filename = val;
        }
        
        //get rid of the starting dashes
        while( tag[0] == '-')
            tag = tag.substr(1);

        //add all remaining items to the temporary list
        tlist.push_back( pair<string,string>(tag, val));
    }
    
    //validate that the file we have is usable
    validate_file();

    //read all of the file contents
    load_file();
    
    //add all remaining items in the list to the item list
    for( size_t i=0; i<tlist.size(); i++)
        setItem_string( tlist[i].first, tlist[i].second, true);


}


/** 
 * Validate the given configuration file
*/
void PSR::Parser::validate_file( ){
    
    //make sure the file exists
    fs::path config_file( config_filename );
    if( fs::exists( config_file ) == false ){
        throw string(string("Error: config file ") + config_filename + string(" does not exist"));
    }

    //check the extension for config file type
    string extension = fs::extension( config_file );
    
    if( extension == ".cfg" ){

    }
    else if( extension == ".xml" ){
        throw string("Error: xml files are not yet supported. Please contact me to support it.");
    }
    else{
        throw (string("Error: ")+extension+string(" type files are not supported"));
    }


}

void PSR::Parser::load_file( ){

    //create an input stream object
    ifstream fin;
    
    //open the file
    fin.open(config_filename.c_str());
    
    //read the first line and start the loop
    string line;
    getline( fin, line);

    while( !fin.eof()){
       
        // trim out the leading spaces
        boost::algorithm::trim( line );
        
        // ignore if the first character is a comment or the length is 0
        if( line.size() <= 0 || line[0] == '#' ){
            getline( fin, line);
            continue;
        }

        //split the line according to the = sign
        size_t idx = line.find_first_of("=");
        items.push_back( pair<string,string>( line.substr(0, idx), line.substr(idx+1))); 
        
        //pull the next line
        getline( fin, line);
    }

    //close the file and clean up
    fin.close();

}

string PSR::Parser::getItem_string( const string& tag_name, bool& found )const{
    
    found = false;

    //first we need to search through the item list and look for the right tag
    for( size_t i=0; i<items.size(); i++)
        if( tag_name == items[i].first ){
            found = true;
            return items[i].second;
        }

    //if nothing is found, return nothing
    return "";
}

void  PSR::Parser::setItem_string( const string& tag_name, const string& value ){
    setItem_string( tag_name, value, false );
}


void  PSR::Parser::setItem_string( const string& tag_name, const string& value, const bool& create ){

    //iterate through the list looking for a matching tag
    for( size_t i=0; i<items.size(); i++ )
        if( tag_name == items[i].first ){
            items[i].second = value;
            return;
        }

    
    //if the create flag is set, then add the item
    items.push_back( pair<string,string>( tag_name, value));


}


double PSR::Parser::getItem_double( const string& tag_name, bool& found )const{
    
    found = false;

    //first we need to search through the item list and look for the right tag
    for( size_t i=0; i<items.size(); i++)
        if( tag_name == items[i].first ){
            found = true;
            return str2num<double>(items[i].second);
        }

    //if nothing is found, return nothing
    return 0;
}

void  PSR::Parser::setItem_double( const string& tag_name, const double& value ){
    setItem_double( tag_name, value, false );
}


void  PSR::Parser::setItem_double( const string& tag_name, const double& value, const bool& create ){

    //iterate through the list looking for a matching tag
    for( size_t i=0; i<items.size(); i++ )
        if( tag_name == items[i].first ){
            items[i].second = num2str<double>(value);
            return;
        }

    
    //if the create flag is set, then add the item
    items.push_back( pair<string,string>( tag_name, num2str<double>(value)));


}

int PSR::Parser::getItem_int( const string& tag_name, bool& found )const{
    
    found = false;

    //first we need to search through the item list and look for the right tag
    for( size_t i=0; i<items.size(); i++)
        if( tag_name == items[i].first ){
            found = true;
            return str2num<int>(items[i].second);
        }

    //if nothing is found, return nothing
    return 0;
}

void  PSR::Parser::setItem_int( const string& tag_name, const int& value ){
    setItem_int( tag_name, value, false );
}


void  PSR::Parser::setItem_int( const string& tag_name, const int& value, const bool& create ){

    //iterate through the list looking for a matching tag
    for( size_t i=0; i<items.size(); i++ )
        if( tag_name == items[i].first ){
            items[i].second = num2str<int>(value);
            return;
        }

    
    //if the create flag is set, then add the item
    items.push_back( pair<string,string>( tag_name, num2str<int>(value)));

}

vector<double> PSR::Parser::getItem_vec_double( const std::string& tag_name, bool& found )const{
    
    vector<double> output;
    vector<string> substrs;
    string pre_split;

    //iterate through list looking for the matching tag
    for( size_t i=0; i<items.size(); i++)
        if( tag_name == items[i].first ){
            pre_split = items[i].second;
            found=true;
            break;
        }

    if( found == true ){
        ba::split( substrs, pre_split, ba::is_any_of(", "));
        for( size_t i=0; i<substrs.size(); i++){
            ba::trim(substrs[i]);
            if( substrs[i].size() > 0 ){
                output.push_back(str2num<double>(substrs[i]));
            }
        }
    }
    
    return output;

}

void  PSR::Parser::setItem_vec_double( const std::string& tag_name, const vector<double>& value ){
    setItem_vec_double( tag_name, value);
}

void  PSR::Parser::setItem_vec_double( const std::string& tag_name, const vector<double>& value, const bool& create ){

    //iterate through the list, looking for a match
    for( size_t i=0; i<items.size(); i++)
        if( tag_name == items[i].first ){
            //clear the item
            string output = "";
            for( size_t j=0; j<value.size()-1; j++)
                output += num2str<double>(value[j]) + ",";
            output += num2str<double>(value.back());
            items[i].second = output;
            return;
        }
    
    if( create == true ){

        string output = "";
        for( size_t i=0; i<value.size()-1; i++)
            output += num2str<double>(value[i]) + ",";
        output += num2str<double>(value.back());
        items.push_back( pair<string,string>( tag_name, output));
    }

}

std::vector<string> PSR::Parser::getItem_vec_string( const std::string& tag_name, bool& found )const{
    
    vector<string> output;
    vector<string> substrs;
    string pre_split;

    //iterate through list looking for the matching tag
    for( size_t i=0; i<items.size(); i++)
        if( tag_name == items[i].first ){
            pre_split = items[i].second;
            found=true;
            break;
        }

    if( found == true ){
        ba::split( substrs, pre_split, ba::is_any_of(", "));
        for( size_t i=0; i<substrs.size(); i++){
            ba::trim(substrs[i]);
            if( substrs[i].size() > 0 ){
                output.push_back(substrs[i]);
            }
        }
    }
    
    return output;

}

void  PSR::Parser::setItem_vec_string( const std::string& tag_name, const std::vector<string>& value ){
    setItem_vec_string( tag_name, value, false );
}

void  PSR::Parser::setItem_vec_string( const std::string& tag_name, const std::vector<string>& value, const bool& create ){

    //iterate through the list, looking for a match
    for( size_t i=0; i<items.size(); i++)
        if( tag_name == items[i].first ){
            //clear the item
            string output = "";
            for( size_t j=0; j<value.size()-1; j++)
                output += value[j] + ",";
            output += value.back();
            items[i].second = output;
            return;
        }
    
    if( create == true ){

        string output = "";
        for( size_t i=0; i<value.size()-1; i++)
            output += value[i] + ",";
        output += value.back();
        items.push_back( pair<string,string>( tag_name, output));
    }

}




bool PSR::Parser::getItem_bool( const string& tag_name, bool& found )const{
    
    found = false;
    string temp;

    //first we need to search through the item list and look for the right tag
    for( size_t i=0; i<items.size(); i++)
        if( tag_name == items[i].first ){
            found = true;
            
            temp = items[i].second;
            std::transform( temp.begin(), temp.end(), temp.begin(), ::tolower);
            
            //check for true
            if( temp == "true" || temp == "1" ){
                return true;
            }
            else if( temp == "false" || temp == "0" ){
                return false;
            }
            else{
                throw string(string("ERROR: Unknown parameter: ") + items[i].second);
            }

        }

    //if nothing is found, return nothing
    return false;
}

void  PSR::Parser::setItem_bool( const string& tag_name, const bool& value ){
    setItem_bool( tag_name, value, false );
}


void  PSR::Parser::setItem_bool( const string& tag_name, const bool& value, const bool& create ){

    //iterate through the list looking for a matching tag
    for( size_t i=0; i<items.size(); i++ )
        if( tag_name == items[i].first ){
            items[i].second = bool2str(value);
            return;
        }

    
    //if the create flag is set, then add the item
    items.push_back( pair<string,string>( tag_name, bool2str(value)));


}


/**
 * Return true if the configuration file exists.
*/
bool PSR::Parser::fileExists( const string& filename ){
    return fs::exists( fs::path( filename ));
}

