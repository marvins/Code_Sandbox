#! /usr/bin/env bash


# Return the directory contents
get_contents(){
    CONTENTS=`find . -mindepth 1 -maxdepth 1 -type d`
}



delete_recursively(){
# Enter the directory
cd $1

# Grab all contents of the directory
FILES=`ls`
ABSNAME=$(readlink -f $1)

for _FILE in ${FILES}
do
    
    # Check if a directory
    if [ -d "${_FILE}" ]; then
        
        # Lets recursively delete the directory contents
        delete_recursively ${_FILE} 
    
    else # Must be a file
        rm -r ${_FILE}
    fi

done

cd ..
rm -r $1
echo "entering ${ABSNAME}"

}




###############################################
#                Main Function                #
###############################################
if [ "$1" == "" ]; then
    echo "Cannot run the script without a directory to delete"
    exit
fi

delete_recursively $1 

exit

