#include "OrthoExceptions.hpp"
#include "Utilities.hpp"

using namespace std;
        
//default constructor        
ParserTagNotFoundException::ParserTagNotFoundException(){
    message = "ERROR: Parser Tag Not Found";
}

//parameterized constructor
ParserTagNotFoundException::ParserTagNotFoundException( string const& tag_name, string const& filename, int const& line_number ){
    message = string("ERROR: Parser Tag (") + tag_name + string(") not found, thrown in ") + filename + string(" on line ") + num2str(line_number);
}

//parameterized constructor
ParserTagNotFoundException::ParserTagNotFoundException( string const& tag_name, string const& run_type, string const& filename, int const& line_number ){
    message = string("ERROR: Parser Tag (") + tag_name + string(") not found, thrown in ") + filename + string(" on line ") + num2str(line_number) + string(" with RUN_TYPE=") + run_type;
}

//get message
string ParserTagNotFoundException::getMessage()const{
    return message;
}
        

        
FileNotFoundException::FileNotFoundException(){
    message = "ERROR: File was not found";
}

FileNotFoundException::FileNotFoundException( std::string const& missing_file_name, std::string const& filename, int const& line_number ){
    message = string("ERROR: File (") + missing_file_name + string(") was not found, thrown in ") + filename + string(" on line ") + num2str(line_number);
}

FileNotFoundException::FileNotFoundException( std::string const& missing_file_name, std::string run_type,  std::string const& filename, int const& line_number ){
    message = string("ERROR: File (") + missing_file_name + string(") was not found, thrown in ") + filename + string(" on line ") + num2str(line_number) + string(" with RUN_TYPE=") + run_type;
}

string FileNotFoundException::getMessage()const{
    return message;
}
        
        
ParserInvalidTagValueException::ParserInvalidTagValueException(){
    message = "ERROR: Parser tag value was incorrect";
}

ParserInvalidTagValueException::ParserInvalidTagValueException( string const& tag_name, string const& tag_value, string const& run_type, string const& filename, int const& line_number ){

    message = string("ERROR: Parser Tag Value (") 
            + tag_value 
            + string(") is an invalid option for a tag of (") 
            + tag_name 
            + string("). Thrown in ") 
            + filename 
            + string(" on line ") 
            + num2str(line_number)
            + string(", with RUN_TYPE=")
            + run_type;
}
        

string ParserInvalidTagValueException::getMessage()const{
    return message;
}
        

//output stream operator
std::ostream& operator << ( std::ostream& ostr, OrthoException const& ex ){

    ostr << ex.getMessage() << endl;

    return ostr;
}
