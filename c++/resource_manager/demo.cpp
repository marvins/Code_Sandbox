
#include <iostream>

#include "Resource_Manager.hpp"
#include "SomeBlob.hpp"

int main( int argc, char* argv[] )
{
    Resource_Manager<SomeBlob> manager;

    // Iterate over each resource
    bool valid;
    int counter = 0;
    for( size_t i = 0; i < manager.Get_Number_Resources(); i++ )
    {
        auto resource = manager.Get_Resource( valid );
        if( !valid )
        {
            throw std::runtime_error("Unable to get resource: " + std::to_string(counter));
        }
        counter++;

        // Set the contents

    }

    return 0;
}