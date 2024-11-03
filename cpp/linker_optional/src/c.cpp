
#include "a.hpp"
#include <iostream>

int main( int argc, char* argv[] )
{
    int option = std::stoi(argv[1]);
    CALL_A(option);

    return 0;
}

