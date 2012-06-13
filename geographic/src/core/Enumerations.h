#ifndef __SRC_CORE_ENUMERATIONS_H__
#define __SRC_CORE_ENUMERATIONS_H__

namespace GEO{

    /**
     * Enumeration useful for loading different image types. The
     * only planned type considered now is the NITF.
     */
    enum GEO_IMAGE_TYPE{
        NITF,
        DTED,
        SRTM
    };

}

#endif
