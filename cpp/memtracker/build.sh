#!/usr/bin/env bash

set -e 

DO_CLEAN=0
DO_MAKE=0

#  Parse command-line args
for ARG in "$@"; do
    case ${ARG} in

        -c)
            DO_CLEAN='1'
            ;;

        -m)
            DO_MAKE='1'
            ;;
        
        *)
            echo "Unsupported flag: ${ARG}"
            exit 1
            ;;

    esac
done

#  Clean, if requested
if [ "${DO_CLEAN}" = '1' ]; then
    rm -rvf build
fi

#  Build, if requested
if [ "${DO_MAKE}" = '1' ]; then
    
    conan install . --output-folder=build --build=missing

    pushd build

    cmake -DCMAKE_BUILD_TYPE=debug -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" ..

    make -j4

    popd

fi