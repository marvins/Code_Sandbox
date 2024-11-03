#include "Utilities.hpp"


using namespace std;

/***********************************************/
/*             Load the J2K Data.              */
/***********************************************/
void Load_J2K_Data( const std::string&  input_path,
                    uint8_t*&           input_buffer,
                    int64_t&            input_size )
{
    
    // Open the file
    ifstream fin;
    fin.open( input_path.c_str(), ios::binary );
    
    // Get the file size
    fin.seekg(0,ios_base::end);
    input_size = fin.tellg();
    fin.seekg(0,ios_base::beg);

    // Allocate Memory
    input_buffer = new uint8_t[input_size];
    char* cbuffer = (char*)input_buffer;

    // Read the data
    fin.read( (char*)cbuffer, 
              input_size );

    // Close the file
    fin.close();

}


