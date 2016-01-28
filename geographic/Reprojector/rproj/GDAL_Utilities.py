#    File:    GDAL_Utilities.py
#    Author:  Marvin Smith
#    Date:    11/29/2015
#

#  Python Libraries
from osgeo import gdal, ogr
import os, logging


#  Create lookup
WkbType = { 0 : 'WKBUnknown',
            1 : 'WKBPoint',
            2 : 'WKBLineString',
            3 : 'WKBPolygon',
            4 : 'WKBMultiPoint',
            5 : 'WKBMultiLineString',
            6 : 'WKBMultiPolygon',
            7 : 'WKBNoGeometry',
            8 : 'WKBPoint25D',
            9 : 'WKBLineString25D',
           10 : 'WKBPolygon25D',
           11 : 'WKBMultiPoint25D',
           12 : 'WKBMultiLineString25D',
           13 : 'WKBMultiPolygon25D' }


SHP_DRIVER='ESRI Shapefile'

DriverConverter = { '.shp' : SHP_DRIVER }


def Open_Vector_File( shapefile_path ):

    
    #  Log Entry
    logging.debug('Opening Vector File')

    #  Get the extension
    ext = os.path.splitext(shapefile_path)[1]

    #  Compute the driver
    driver_name = DriverConverter[ext]

    #  Create Driver
    driver = ogr.GetDriverByName(driver_name)
    if driver is None:
        raise Exception('Unable to find driver (' + driver_name + ')')

    #  Open the File
    dataset = driver.Open( shapefile_path,
                           gdal.GA_ReadOnly )

    #  Make sure succeeded
    if dataset is None:
        raise Exception('Unable to open the Shapefile (' + shapefile_path + ')')

    #  Get the list of layers
    layer_cnt = dataset.GetLayerCount()


    #  Log Driver Info
    logging.debug('Opened Shapefile: ' + shapefile_path)
    logging.debug(' - Driver : ' + str(driver.GetName()))
    logging.debug(' - Layers : ' + str(layer_cnt))

    return dataset, driver


def Process_Vector_Subgeometry( geometry, gidx, min_point_threshold, counter ):

    #  Log Entry
    logging.debug('      - Processing Subgeometry, Iteration: ' + str(counter))

    #  Get the geometry
    sub_geom = geometry.GetGeometryRef(gidx)
    gtype    = sub_geom.GetGeometryType()

    # Get the Point Count
    num_points  = sub_geom.GetPointCount()

    #  Process Geometry Point
    if sub_geom is not None and sub_geom.GetGeometryType() == ogr.wkbPoint:
        return [[sub_geom.GetX(), sub_geom.GetY()]]

    #  Process Polygon
    elif sub_geom is not None and (gtype == ogr.wkbPolygon or gtype == ogr.wkbMultiPolygon):
        
        #  Fetch the geometry count
        geo_count = sub_geom.GetGeometryCount()

        #  Create output container
        output = []

        #  Recursively process each sub-geometry
        for geoidx in range(geo_count):
            
            #  Fetch the next geometry
            geometries = Process_Vector_Subgeometry( sub_geom, 
                                                     geoidx, 
                                                     min_point_threshold,
                                                     counter+1 )

            for ii in geometries:
                if ii is not None and len(ii) > 0:
                    output.append(ii)

        #  Return output
        return output


    #  Process Linestring
    elif sub_geom is not None and sub_geom.GetGeometryType() == ogr.wkbLineString:

        #  Get the list of points
        point_count = sub_geom.GetPointCount()

        #  Create output
        output = []
        for pidx in range(point_count):
            tp = sub_geom.GetPoint_2D(pidx)
            output.append( [tp[0], tp[1]] )

        return [output]

    else:
        logging.warning('Unknown geometry type: ' + WkbType[sub_geom.GetGeometryType()])
        return []


    return []


def Process_Vector_Geometry( feature, gidx, min_point_threshold, counter = 0 ):

    #  Log Entry
    logging.debug('    - Processing Vector Geometry, Iteration: ' + str(counter))

    #  Get the geometry
    geom = feature.GetGeomFieldRef(gidx)
    geom_type = geom.GetGeometryType()

    #  Get the geometry components
    #field_defn = feature_def.GetFieldDefn(gidx)

    #  Process Geometry Point
    if geom is not None and geom.GetGeometryType() == ogr.wkbPoint:
        return [[geom.GetX(), geom.GetY()]]

    #  Process Polygon
    elif geom is not None and (geom_type == ogr.wkbPolygon or geom_type == ogr.wkbMultiPolygon):
        
        #  Fetch the geometry count
        geo_count = geom.GetGeometryCount()

        #  Create output container
        output = []

        #  Recursively process each sub-geometry
        for geoidx in range(geo_count):
            
            #  Fetch the next geometry
            geometries = Process_Vector_Subgeometry( geom, geoidx, min_point_threshold, counter+1 )

            for igeom in geometries:
                if igeom is not None and len(igeom) > 0:
                    output.append(igeom)

        #  Return output
        return output

    #  Process Linestring
    elif geom is not None and geom.GetGeometryType() == ogr.wkbLineString:

        #  Get the list of points
        point_count = geom.GetPointCount()

        #  Check if over the threshold
        if point_count < min_point_threshold:
            return []

        #  Create output
        output = []
        for pidx in range(point_count):
            tp = geom.GetPoint_2D(pidx)
            output.append( [tp[0], tp[1]] )

        return [output]

    else:
        logging.warning('Unknown geometry type: ' + WkbType[geom.GetGeometryType()])
        return []


    return []



def Load_Vector_Data( shapefile_path, options ):
    
    #  Load the Shapefile
    dataset, driver = Open_Vector_File( shapefile_path )
    if dataset is None or driver is None:
        raise Exception('Unable to open file.')
    
    #  Create output container
    output = {}

    #  Get the Layer count
    layer_cnt = dataset.GetLayerCount()

    #  Fetch Each Layer
    for lidx in range(layer_cnt):
    
        #  Grab the layer
        layer = dataset.GetLayerByIndex(lidx)

        #  Make sure layer is valid
        if layer is None:
            logging.warning("Layer " + str(lidx) + ' does not exist.')
            continue

        #  Get the SRS
        srs = layer.GetSpatialRef()

        #  Get the Feature Definition
        feature_def = layer.GetLayerDefn()

        #  Define a new layer
        layer_output = []

        #  Get the list of features
        logging.debug('   - Layer ' + str(lidx) + ' feature count : ' + str(layer.GetFeatureCount()))

        #  Iterate over features
        for feature in layer:

            #  Get the Feature Geometry Count Count
            geometry_cnt = feature.GetGeomFieldCount()
            
            logging.debug('    - Layer ' + str(lidx) + ', Feature Geometry Count: ' + str(geometry_cnt))


            #  Iterate over each Geometry
            for gidx in range(geometry_cnt):

                #  Process the Geometry
                geometries = Process_Vector_Geometry( feature, gidx, options.config['feature_point_min_threshold'] )
                
                if len(geometries) > 0:
                    for geometry in geometries:
                        if geometry is not None and len(geometry) > 0:
                            layer_output.append(geometry)
        
        # Add to the output
        output[lidx] = layer_output

    return output

