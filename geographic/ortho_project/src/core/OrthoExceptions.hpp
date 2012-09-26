#ifndef __SRC_CORE_ORTHOEXCEPTIONS_HPP__
#define __SRC_CORE_ORTHOEXCEPTIONS_HPP__

#include <iostream>
#include <fstream>
#include <string>

class OrthoException {

    public: 

    virtual std::string getMessage()const=0;
    
};

std::ostream& operator << ( std::ostream& ostr, OrthoException const& ex );

/**
 * @class ParserTagNotFoundException
 *
 * A generic exception to throw for parser-related errors
*/
class ParserTagNotFoundException : public OrthoException {

    public:
        
        ParserTagNotFoundException();
        ParserTagNotFoundException( std::string const& tag_name, std::string const& filename, int const& line_number );
        ParserTagNotFoundException( std::string const& tag_name, std::string const& run_type, std::string const& filename, int const& line_number );
        
        virtual std::string getMessage()const;
    
    private:
        std::string message;
};


/** 
 * @class FileNotFoundException
 * 
 * A generic exception for when we can't file files
*/
class FileNotFoundException : public OrthoException {
    
    public:
        FileNotFoundException();
        FileNotFoundException( std::string const& missing_file_name, std::string const& filename, int const& line_number );
        FileNotFoundException( std::string const& missing_file_name, std::string run_type,  std::string const& filename, int const& line_number );
        
        virtual std::string getMessage()const;

    private: 
        std::string message;

};

class ParserInvalidTagValueException : public OrthoException {

    public: 
        ParserInvalidTagValueException();
        ParserInvalidTagValueException( std::string const& tag_name, std::string const& tag_value, std::string const& run_type, std::string const& filename, int const& line_number );
        
        virtual std::string getMessage()const;

    private:
        std::string message;
};

#endif

