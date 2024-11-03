
#include <cstring>
#include <iostream>

#include "Resource_Manager.hpp"
#include "SomeBlob.hpp"

int main( int argc, char* argv[] )
{
    Resource_Manager<SomeBlob> manager( 10, []{ return SomeBlob( 10, 'a', "Hello World" ); } );

    // Create the resources
    bool valid;
    int counter = 0;
    std::vector<std::shared_ptr<SomeBlob>> resources( manager.Get_Number_Resources() );
    for( size_t i = 0; i < manager.Get_Number_Resources(); i++ )
    {
        std::cout << "Processing Resource " << i << std::endl;
        resources[counter] = manager.Get_Resource( valid );
        if( !valid )
        {
            throw std::runtime_error("Unable to get resource: " + std::to_string(counter));
        }

        // Set the contents
        resources[counter]->Set_Message( "Resource " + std::to_string(counter) );
        memset( resources[counter]->Get_Buffer(),
                std::to_string(counter % 10)[0],
                resources[counter]->Get_Buffer_Size_Bytes() );

        counter++;
    }

    for( size_t i = 0; i < resources.size(); i++ )
    {
        // Release the resource
        manager.Release( resources[i] );
    }

    // Print the Resource-Manager
    std::cout << manager.To_Log_String() << std::endl;
    std::cout << std::endl;

    // Verify the resources
    counter = 0;
    for( size_t i = 0; i < manager.Get_Number_Resources(); i++ )
    {
        auto resource = manager.Get_Resource( valid );
        if( !valid )
        {
            throw std::runtime_error( "Unable to get resource: " + std::to_string( counter ));
        }
        counter++;
        std::cout << (*resource) << std::endl;
    }

    return 0;
}