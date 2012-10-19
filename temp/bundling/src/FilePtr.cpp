#include "FilePtr.hpp"


FilePtr::FilePtr( ){

    initialized = false;
    scene_number = -1;
}

ostream& operator << ( ostream& ostr, const FilePtr& ptr ){

    ostr << "File Pointer" << endl;
    ostr << "  - Scene: " << ptr.scene_number << endl;
    ostr << "  - Parts: ";

    if( ptr.file_parts.size() > 0 ){
        ostr << ptr.file_parts[0];
        for ( size_t i=1; i<ptr.file_parts.size(); i++ )
            ostr << ", " << ptr.file_parts[i];
        ostr << endl;
    }
    else{ 
        ostr << "empty" << endl;
    }
    ostr << "  - Init : " << ptr.initialized << endl;

    return ostr;
}

