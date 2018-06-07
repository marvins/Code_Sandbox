#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    6/6/2018


#  Add C++11x Support
SET(CMAKE_CXX_STANDARD 11)

#  Add some nice GCC flags
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type -rdynamic -ldl -fPIC -g -Wall" )
endif()


#  Set the project
project( MPI_OTHER_EXAMPLES )


#  Find MPI
find_package( MPI )
if( NOT MPI_CXX_FOUND )
    message( FATAL_ERROR "Unable to find MPI.  Check your installation.")
else()
    message("MPI Found")
    message("MPI_CXX_COMPILER: ${MPI_CXX_COMPILER}")
    message("MPI_INCLUDE_PATH: ${MPI_INCLUDE_PATH}")
    include_directories(${MPI_INCLUDE_PATH})
endif()


#  Create each mpi dynamic window demo
add_executable( mpi-denio-comm-connect-demo
                denio_comm_connect_demo.cpp )

target_link_libraries( mpi-denio-comm-connect-demo
                       ${MPI_LIBRARIES} )


