#ifndef __SRC_CORE_ENUMERATIONS_H__
#define __SRC_CORE_ENUMERATIONS_H__

namespace GEO{

    /**
     * Enumerations useful for loading different image types.
     *
     */
    enum GEO_IMAGE_TYPE{
        NITF  /*< NITF Imagery */,
        DTED  /*< DTED Elevation Imagery */,
        SRTM  /*< SRTM Elevation Imagery */,
        UNKNOWN
    };

}

#endif
