

class GeoImage{


    /** This is for loading images or dem models directly from file.
     *  
     *  @param[in] filename         Name of file to load.  Currently SRTM, DTED, and NITF supported
     *  @param[in] load_image_flag  Whether or not to load image into memory
    */
    GeoImage( string  filename,  load_image_flag = false );
     
    /**
     * I am going to overload the function operator to allow you to grab specific pixels.  This will make
     * the DEM class capable of using the GeoImage for all operations and remain memory efficient.
    */
    template <typename TP>
    TP  operator()( const int X, const int Y )const;


}


class DEM{

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
    DEM( Rect_<float> roi,  DEM_PARAMS params );

    /**
     * This is for loading a single coordinate into a dem model.  The strength of this
     * functionality is that you can retrieve a single coordinate without loading the entire
     * DEM model.  This would be useful for interpolating across a line or path which does not
     * require loading entire datasets. 
    */
    DEM( Point2f   coordinate, DEM_PARAMS params );

    /**
     * This is for retrieving the elevation of a specific coordinate given a DEM constructed with a region of interest.
     * example:  GEO::DEM dem = GEO::DEM( Rect_<float>( 36, 119, 1, 1));
     *           double elevation = dem( Point2f( 36.5, 119.4 ));
   */
    double operator()( Point2f coordinate );

    /**
     * This is for retrieving the elevation of a specific coordinate given a DEM constructed with a single coordinate.
     * If the user gives a DEM with a region, it will return the upper left corner.
     * example:  GEO::DEM dem = GEO::DEM( Point2f( 36, 119));
     *           double elevation = dem();
    */
    double operator()();
    
}


/**
 * Other potential functionality and considerations
 *  
 *  i)   if a region of interest covers multiple lat/long int pairs, then it will pull all necessary files
 *       until the tile is ready. 
 *  
 *  ii)  I want to add a suite of utility functions for processing the filenames of the GS2 image data. 
 *       For example, given a dataset with gs2 image data, I want to be able to build a set of ordered images
 *       so that we can rapidly reference them.  Since we don't have a camera to pick images off of, we need 
 *       to have a ready stack of images to pull from.  This would store the image list. 
 *
 *  iii) I am going to build DTED support first, then add SRTM data. 
 *
 *  iv)  I want to add helper functions to modify header data for NITF images.  Currently I just shove the header data
 *       from GDAL into a container.  This is stored as a char**.  I want to put this into a vector<pair<string, string>> where
 *       each pair is the variable and the result.  Then, have a function to modify this header info. 
*/


