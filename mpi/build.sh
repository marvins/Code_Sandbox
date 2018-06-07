#!/bin/sh

#  Globally accessible values
BUILD_TYPE='release'
CMAKE_EXE='cmake'
CMAKE_ARGS=''

function find_cmake()
{
    #  Check the CMake Version
    CMAKE_VERSION=`cmake --version | head -n 1 | cut -d ' ' -f 3`
    
    #  Split the major
    CMAKE_MAJOR=`echo $CMAKE_VERSION | cut -d . -f 1`
    if [ "$CMAKE_MAJOR" -lt '3' ]; then
        
        #  Look for CMake 3
        CMAKE3_EXE=`which cmake3`
        if [ -e "$CMAKE3_EXE" ]; then
            CMAKE_EXE=$CMAKE3_EXE
        fi
    fi

    echo "Using CMake Binary: $CMAKE_EXE"
}


function clean_project()
{
    echo "-> Cleaning Project"

    if [ -e 'release' ]; then
        rm -r release
    fi
    if [ -e 'debug' ]; then
        rm -r debug 
    fi
}


function build_project()
{
    echo "-> Building Project"

    #  Navigate to build-dir
    if [ ! -e "$BUILD_TYPE" ]; then
        mkdir -p $BUILD_TYPE
    fi

    #  Get project base
    pushd $BUILD_TYPE
    
    #  Run CMake
    ${CMAKE_EXE} ${CMAKE_ARGS} ..
    if [ ! "$?" = '0' ]; then
        echo "CMake failed with errors"
        exit 1
    fi

    #  Run Make
    make -j4 
    if [ ! "$?" = '0' ]; then
        echo "Make failed with errors"
        exit 1
    fi

    echo "   -> Created cmake project in $PWD"

    #  Back up
    popd

}


RUN_BUILD=0
RUN_CLEAN=0

BUILD_TYPE_SET=0

#  Resolve CMake
find_cmake

#  Iterate over arguments
for ARG in "$@"; do
    case $ARG in
        -h)
            usage
            exit 1
            ;;
        -m)
            RUN_BUILD=1
            ;;
        -c)
            RUN_CLEAN=1
            ;;

        --debug)
            BUILD_TYPE='debug'
            ;;

        *)
            if [ "$BUILD_TYPE_SET" = '1' ]; then
                BUILD_TYPE=$ARG
                BUILD_TYPE_SET=0
            else
                echo "Unsupported Command-Line Argument ${ARG}"
                exit 1;
            fi
    esac
done


#  Clean if requested
if [ "$RUN_CLEAN" = 1 ]; then
    clean_project
fi

#  Build if requested
if [ "$RUN_BUILD" = 1 ]; then
    build_project
fi


