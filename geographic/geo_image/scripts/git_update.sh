FILES=( "src/tools/geo_meta_parser/File_Utilities.hpp"
        "src/tools/geo_meta_parser/File_Utilities.cpp"
        "src/tools/geo_meta_parser/string_utilities.hpp"
        "src/tools/geo_meta_parser/string_utilities.cpp"
      )

HOMES=( "../../useful_stuff/cpp/filesystem/File_Utilities.hpp"
        "../../useful_stuff/cpp/filesystem/File_Utilities.cpp" 
        "../../useful_stuff/cpp/string_functions/string_utilities.hpp"
        "../../useful_stuff/cpp/string_functions/string_utilities.cpp"
      )

for IDX in 0 1 2 3
    do

    DIFFS=`diff ${FILES[${IDX}]} ${HOMES[${IDX}]}`
    
    if [ "${DIFFS}" ]; then 
        echo "${FILES[${IDX}]} <--> is different from <-->  ${HOMES[${IDX}]}"
    fi
done

