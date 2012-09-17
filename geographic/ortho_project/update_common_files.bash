#! /bin/bash 

# Author: Marvin Smith
#
# This is a bash script which will update your source
# tree with the most current source code from code in
# another source tree.  This is designed to allow you
# to use code in a different location and keep both 
# copies current.  I use this for example in code for
# command-line progress bars, as one good copy should
# be used as frequently as possible. 


# Place your files here which you want to keep current in your tree
FILES=( src/core/Parser.hpp
        src/core/Parser.cpp 
        src/core/ProgressBar.cpp
        src/core/ProgressBar.hpp 
        src/core/Logger.cpp
        src/core/Logger.hpp 
        src/math/Quaternion.hpp
        src/math/Quaternion.cpp 
        src/math/Vector.hpp
        src/math/Vector.cpp
        )

# Place the directory where these files originate from
SOURCES=( /home/ms6401/projects/Code_Sandbox/useful_stuff/cpp/config_parser/
          /home/ms6401/projects/Code_Sandbox/useful_stuff/cpp/config_parser/ 
          /home/ms6401/projects/Code_Sandbox/useful_stuff/cpp/progress_bar/
          /home/ms6401/projects/Code_Sandbox/useful_stuff/cpp/progress_bar/
          /home/ms6401/projects/Code_Sandbox/useful_stuff/cpp/logger/ 
          /home/ms6401/projects/Code_Sandbox/useful_stuff/cpp/logger/ 
          /home/ms6401/projects/Code_Sandbox/math/geometry/
          /home/ms6401/projects/Code_Sandbox/math/geometry/
          /home/ms6401/projects/Code_Sandbox/math/core/
          /home/ms6401/projects/Code_Sandbox/math/core/
          )


############################################################################
PASSED=0
FAILED=1

TEMP_FILE="/tmp/DiffLog.txt"

# Make sure the list lengths are the same
if [ "${#FILES[@]}" != "${#SOURCES[@]}" ];
    then
        echo "FILES and SOURCE arrays must be equal"
        exit ${FAILED}
fi

flen=${#FILES[@]}

for (( idx=0; idx<${flen}; idx++ ));
do
    # Check to make sure the file exists
    if [ ! -f "${FILES[${idx}]}" ]; then
        echo "Error: ${FILES[${idx}]} does not exist"
        exit ${FAILED}
    fi

    # Make sure the source directory exists
    if [ ! -d "${SOURCES[${idx}]}" ]; then
        echo "Error: ${SOURCES[${idx}]} does not exist"
        exit ${FAILED}
    fi
    
    # If the directory exists, but the source file doesn't, exit
    SRC_FILE=${SOURCES[${idx}]}$( basename "${FILES[${idx}]}" )
    if [ ! -f "${SRC_FILE}" ]; then
        echo "${SRC_FILE} does not exist, please check your sources"
        exit ${FAILED}
    fi
    
    # Run vim diff on the files
    if diff ${FILES[${idx}]} ${SRC_FILE} > /dev/null; then
        echo "${FILES[${idx}]} and ${SRC_FILE} are not different"
    else 
        echo "${FILES[${idx}]} and ${SRC_FILE} are different"
        vimdiff ${FILES[${idx}]} ${SRC_FILE}
    fi


    # Merge the changes
    echo ""
done

echo "All changes finished, exiting program"
exit ${PASSED}
