

class GeoImage{


    /** This is for loading images or dem models directly from file.
     *  
     *  @param[in] filename         Name of file to load.  Currently SRTM, DTED, and NITF supported
     *  @param[in] load_image_flag  Whether or not to load image into memory
    */
    GeoImage( string  filename,  load_image_flag = false );


    /**
     * This is for loading a region of interest in a DEM model. The DEM model will require
     * parameters which are set in the DEM_PARAMS (likely have a different name) and 
     * specify parameters such as the home directory of the dted data, whether dted or
     * srtm, etc.  Default parameters will be created to prevent this from being required
     * beyond a simple constructor. 
     *
     * @param[in]  roi     Box containing the Lat/Lon coordinates of the corners
     * @param[in] params   Configuration options to located 
    */
    GeoImage( Rect_<float> roi,  DEM_PARAMS params );

    /**
     * This is for loading a specific coordinate.  The data will persist as long as the file exists. 
     * Note that it will only load that pixel and will not use any large data arrays.  DEM_PARAMS
     * will also be used specifically to ask if you want the sub-pixel accuracy to be computed 
     * by either nearest neighbor or by interpolation. 
   */
    GeoImage( Point2f coordinate,  DEM_PARAMS params );


