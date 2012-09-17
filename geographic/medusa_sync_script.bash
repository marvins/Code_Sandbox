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

MEDUSA_HOME=/home/ms6401/projects/Medusa

# Where to find the Projects
GEOIMAGE_HOME=/home/ms6401/projects/Code_Sandbox/geographic/geo_image
ORTHO_HOME=/home/ms6401/projects/Code_Sandbox/geographic/ortho_project

# Where to place the projects
GEOIMAGE_DEST=${MEDUSA_HOME}/GeoImage
ORTHO_DEST=${MEDUSA_HOME}/OrthoProject

# Place your files here which you want to keep current in your tree
SOURCE=( ${GEOIMAGE_HOME}
         ${GEOIMAGE_HOME}/bin
         ${GEOIMAGE_HOME}/build
         ${GEOIMAGE_HOME}/data
         ${GEOIMAGE_HOME}/src
         ${GEOIMAGE_HOME}/src/coordinates
         ${GEOIMAGE_HOME}/src/core
         ${GEOIMAGE_HOME}/src/core/Enumerations.h
         ${GEOIMAGE_HOME}/src/dem
         ${GEOIMAGE_HOME}/src/extensions
         ${GEOIMAGE_HOME}/src/image
         ${GEOIMAGE_HOME}/src/ortho
         ${GEOIMAGE_HOME}/src/tools
         ${GEOIMAGE_HOME}/src/utilities
         ${GEOIMAGE_HOME}/src/GeoImage.h
         ${GEOIMAGE_HOME}/src/GeoImage.dox
         ${GEOIMAGE_HOME}/Makefile 
         ${ORTHO_HOME}
         ${ORTHO_HOME}/bin
         ${ORTHO_HOME}/build
         ${ORTHO_HOME}/data
         ${ORTHO_HOME}/data/options.cfg
         ${ORTHO_HOME}/docs
         ${ORTHO_HOME}/src
         ${ORTHO_HOME}/src/core
         ${ORTHO_HOME}/src/core/Configuration.cpp
         ${ORTHO_HOME}/src/core/Configuration.hpp
         ${ORTHO_HOME}/src/core/dem.hpp
         ${ORTHO_HOME}/src/core/dem.cpp
         ${ORTHO_HOME}/src/core/Logger.hpp
         ${ORTHO_HOME}/src/core/Logger.cpp
         ${ORTHO_HOME}/src/core/Parser.hpp
         ${ORTHO_HOME}/src/core/Parser.cpp
         ${ORTHO_HOME}/src/core/ProgressBar.hpp
         ${ORTHO_HOME}/src/core/ProgressBar.cpp
         ${ORTHO_HOME}/src/core/Utilities.hpp
         ${ORTHO_HOME}/src/core/Utilities.cpp
         ${ORTHO_HOME}/src/math
         ${ORTHO_HOME}/src/math/Geometry.hpp
         ${ORTHO_HOME}/src/math/Geometry.cpp
         ${ORTHO_HOME}/src/math/Quaternion.hpp
         ${ORTHO_HOME}/src/math/Quaternion.cpp
         ${ORTHO_HOME}/src/math/Vector.hpp
         ${ORTHO_HOME}/src/math/Vector.cpp
         ${ORTHO_HOME}/src/ortho
         ${ORTHO_HOME}/src/ortho/Orthorectify.hpp
         ${ORTHO_HOME}/src/ortho/Orthorectify.cpp
         ${ORTHO_HOME}/src/ortho/Perspective.hpp
         ${ORTHO_HOME}/src/ortho/Perspective.cpp
         ${ORTHO_HOME}/src/orthoproject.cpp
         ${ORTHO_HOME}/Makefile    
       )

# Place the directory where these files originate from
DESTINATION=( ${GEOIMAGE_DEST}
              ${GEOIMAGE_DEST}/bin
              ${GEOIMAGE_DEST}/build
              ${GEOIMAGE_DEST}/data
              ${GEOIMAGE_DEST}/src
              ${GEOIMAGE_DEST}/src/coordinates
              ${GEOIMAGE_DEST}/src/core
              ${GEOIMAGE_DEST}/src/core/Enumerations.h
              ${GEOIMAGE_DEST}/src/dem
              ${GEOIMAGE_DEST}/src/extensions
              ${GEOIMAGE_DEST}/src/image
              ${GEOIMAGE_DEST}/src/ortho
              ${GEOIMAGE_DEST}/src/tools
              ${GEOIMAGE_DEST}/src/utilities
              ${GEOIMAGE_DEST}/src/GeoImage.h
              ${GEOIMAGE_DEST}/src/GeoImage.dox
              ${GEOIMAGE_DEST}/Makefile
              ${ORTHO_DEST}
              ${ORTHO_DEST}/bin
              ${ORTHO_DEST}/build
              ${ORTHO_DEST}/data
              ${ORTHO_DEST}/data/options.cfg
              ${ORTHO_DEST}/docs
              ${ORTHO_DEST}/src
              ${ORTHO_DEST}/src/core
              ${ORTHO_DEST}/src/core/Configuration.cpp
              ${ORTHO_DEST}/src/core/Configuration.hpp
              ${ORTHO_DEST}/src/core/dem.hpp
              ${ORTHO_DEST}/src/core/dem.cpp
              ${ORTHO_DEST}/src/core/Logger.hpp
              ${ORTHO_DEST}/src/core/Logger.cpp
              ${ORTHO_DEST}/src/core/Parser.hpp
              ${ORTHO_DEST}/src/core/Parser.cpp
              ${ORTHO_DEST}/src/core/ProgressBar.hpp
              ${ORTHO_DEST}/src/core/ProgressBar.cpp
              ${ORTHO_DEST}/src/core/Utilities.hpp
              ${ORTHO_DEST}/src/core/Utilities.cpp
              ${ORTHO_DEST}/src/math
              ${ORTHO_DEST}/src/math/Geometry.hpp
              ${ORTHO_DEST}/src/math/Geometry.cpp
              ${ORTHO_DEST}/src/math/Quaternion.hpp
              ${ORTHO_DEST}/src/math/Quaternion.cpp
              ${ORTHO_DEST}/src/math/Vector.hpp
              ${ORTHO_DEST}/src/math/Vector.cpp
              ${ORTHO_DEST}/src/ortho
              ${ORTHO_DEST}/src/ortho/Orthorectify.hpp
              ${ORTHO_DEST}/src/ortho/Orthorectify.cpp
              ${ORTHO_DEST}/src/ortho/Perspective.hpp
              ${ORTHO_DEST}/src/ortho/Perspective.cpp
              ${ORTHO_DEST}/src/orthoproject.cpp
              ${ORTHO_DEST}/Makefile
            )


HEADER="\nMedusa Code Syncing Tool"

############################################################################
PASSED=0
FAILED=1

# Print Header
echo -e "${HEADER}"

# Make sure the list lengths are the same
if [ "${#SOURCE[@]}" != "${#DESTINATION[@]}" ];
    then
        echo "SOURCE and DESTINATION arrays must be equal"
        exit ${FAILED}
fi


# Compute the length of one of the arrays
flen=${#SOURCE[@]}

# Create a temp file for managing the diff operations
TEMP_FILE="/tmp/DiffLog.txt"

# Some temp variables 
SRC_EXISTS=1
DST_EXISTS=1

echo -e '\n'
# Iterate through the list and sync the files
for (( idx=0; idx<${flen}; idx++ ));
do
    
    # RESET FLAGS
    BASE_SRC=`( basename ${SOURCE[${idx}]})`
    BASE_DST=`( basename ${DESTINATION[${idx}]})`
    DIR_SRC=`( dirname ${SOURCE[${idx}]})`
    DIR_DST=`( dirname ${DESTINATION[${idx}]})`
    echo "Syncing ${SOURCE[${idx}]} and ${DESTINATION[${idx}]}"
    
    # Get Flags set
    SRC_EXISTS=1
    DST_EXISTS=1

    # Check to make sure the file exists
    if [ ! -f "${SOURCE[${idx}]}" ]; then
        if [ ! -d "${SOURCE[${idx}]}" ]; then
            SRC_EXISTS=0
        fi
    fi
    
    # Make sure the source directory exists
    if [ ! -d "${DESTINATION[${idx}]}" ]; then
        
        # Check if the destination file exists
        if [ ! -f "${DESTINATION[${idx}]}" ]; then
            DST_EXISTS=0
        fi
    fi
    
    ##########################################
    # Throw errors for the various conditions
    ##########################################
    # if source does not exist, then we must not process it
    if [ "${SRC_EXISTS}" == 0 ]; then 
        echo "  Source does not exist, cannot process"
        exit
    
    # if the destination does not exist, offer to copy it over
    elif [ "${DST_EXISTS}" == 0 ]; then

        # if the source is a directory, then create it
        if [ -d "${SOURCE[${idx}]}" ]; then
            echo "  Creating ${DESTINATION[${idx}]}"
            mkdir -p ${DESTINATION[${idx}]}

        # it must be a file and therefore copy it
        else
            # otherwise proceed
            echo -n "   Destination does not exist, would you like to copy src to dest? (y/n - default): "
            read INPUT

            # if the user selects y, then copy it over
            if [ "${INPUT}" == "y" ]; then
                echo "  Copying ${SOURCE[${idx}]} to  ${DIR_DST}"
                cp ${SOURCE[${idx}]}  ${DIR_DST}
            fi
        # end of if the destination does not exist
        fi

    # if both exist, then offer to copy either way, or run vimdiff
    else
        # Make sure the source is a file
        if [ ! -d "${SOURCE[${idx}]}" ]; then

            # Check if the files are different
            if diff ${SOURCE[${idx}]} ${DESTINATION[${idx}]} > /dev/null; then
                echo "  Files are not different, skipping"
            else
                # offer a list of options
                echo "Files are different, please select an option"
                echo "0 - Copy src to dest, this will overwrite dest"
                echo "1 - Copy dest to src, this will overwrite src"
                echo "2 - Run vimdiff to sync files"
                echo -n " Option: "
                read INPUT


                # process user input

                # Copy src to dest
                if [ "${INPUT}" == 0 ]; then
                    cp ${SOURCE[${idx}]}  ${DIR_DEST}
                elif [ "${INPUT}" == 1 ]; then
                    cp ${DESTINATION[${idx}]}  ${DIR_SRC}  
                elif [ "${INPUT}" == 2 ]; then
                    vimdiff ${SOURCE[${idx}]} ${DESTINATION[${idx}]}
                fi
            fi

        # Otherwise the destination is a directory
        else 
            echo "  Directory exists, skipping"
        fi
    fi
    

    # Merge the changes
    echo ""
done

echo "All changes finished, exiting program"
exit ${PASSED}
