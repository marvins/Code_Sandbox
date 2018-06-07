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
project( MPI_FAULT_TOLERANCE )


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

#  Find Boost
find_package( Boost
                COMPONENTS
                    system )
include_directories( ${Boost_INCLUDE_DIRS} )
message("BOOST_ROOT         : ${BOOST_ROOT}")
message("Boost_LIBRARIES    : ${Boost_LIBRARIES}")
message("Boost_INCLUDE_DIRS : ${Boost_INCLUDE_DIRS}")
message("Boost_LIBRARY_DIRS : ${Boost_LIBRARY_DIRS}")


#  Create each mpi dynamic window demo
add_executable( mpi-fault-tolerance-demo-01-master
                fault-tolerance-demo-01-master.cpp
                Connection_Listener.cpp
                Connection_Listener.hpp
                ${COMMON_SOURCES} )

target_link_libraries( mpi-fault-tolerance-demo-01-master
                       ${MPI_LIBRARIES}
                       ${Boost_LIBRARIES} )


add_executable( mpi-fault-tolerance-demo-01-slave
                fault-tolerance-demo-01-slave.cpp
                Connection_Listener.cpp
                Connection_Listener.hpp
                ${COMMON_SOURCES} )

target_link_libraries( mpi-fault-tolerance-demo-01-slave
                       ${MPI_LIBRARIES}
                       ${Boost_LIBRARIES} )


