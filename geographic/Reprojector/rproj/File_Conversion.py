#    File:    File_Conversion.py
#    Author:  Marvin Smith
#    Date:    11/29/2015
#

#  Python Libraries
import logging
from osgeo import gdal, ogr, osr

#  Project Libraries
from GDAL_Utilities import *


#--------------------------------------#
#-      Convert Shapefile Formats     -#
#--------------------------------------#
def Convert_Shapefile(options):

    #  Log Entry
    logging.debug('Converting Shapefile')

    #  Get the Input Spatial Reference
    in_srs  = options.Get_Input_SRS()
    out_srs = options.Get_Output_SRS()

    #  Set the driver
    driver = ogr.GetDriverByName('ESRI Shapefile')

    # Open the File
    dataset = driver.Open(options.config['shapefile_path'],gdal.GA_ReadOnly)

    #  Make sure it succeeded
    if dataset is None:
        raise Exception('Unable to open the Shapefile (' + options.config['shapefile_path'] + ')')


    #  Log Driver Info
    logging.debug('Opened Shapefile: ' + str(options.config['shapefile_path']))
    logging.debug(' - Driver    : ' + str(driver.GetName()))
    logging.debug(' - Input SRS : ' + str(in_srs.ExportToProj4()))
    logging.debug(' - Output SRS: ' + str(out_srs.ExportToProj4()))

    #  Construct the Coordinate Transform
    crd_xform = osr.CoordinateTransformation(in_srs,out_srs)

    #  Fetch each layer
    layer_cnt = dataset.GetLayerCount()
    logging.debug(' - Detected ' + str(layer_cnt) + ' layers.')
    for lidx in range(layer_cnt):


        #  Grab the layer
        layer = dataset.GetLayerByIndex(lidx)

        #  Grab the layer definition
        feature_def = layer.GetLayerDefn()

        #  Iterate over features
        for feature in layer:

            #  Get the feature count
            feature_count = feature.GetGeomFieldCount()
            for fidx in range(feature_count):

                #  Get the geometry
                geom = feature.GetGeomFieldRef(fidx)

                #  Get the geometry components
                field_defn = feature_def.GetFieldDefn(fidx)

                #   Process Point
                if geom is not None and geom.GetGeometryType() == ogr.wkbPoint:
                    print "%.3f, %.3f" % ( geom.GetX(), geom.GetY() )

                #  Process Polygon
                elif geom is not None and geom.GetGeometryType() == ogr.wkbPolygon:

                    #  Get the Geometry Count
                    geo_count = geom.GetGeometryCount()
                    pnt_count = geom.GetPointCount()

                    for gidx in range(geo_count):

                        #  Get the geometry
                        igeom = geom.GetGeometryRef(gidx)
                        ipnt  = igeom.GetPointCount()
                        print('\nSub-Geometry: ' + str(gidx))
                        print('Sub-Point Cnt: ' + str(ipnt))

                        #  Print each point
                        for pidx in range(ipnt):

                            tp = igeom.GetPoint_2D(pidx)
                            print("Point " + str(pidx) + ': ' + str(tp) )


                else:
                    print "no point geometry\n"

                #  Print stuff
                print('Feature Idx: ' + str(fidx))
                print('  Def: ' + str(field_defn))
                print('  Geometry Type: ' + str(geom.GetGeometryType()) + ' vs ' + str(ogr.wkbPolygon))
                raw_input('pause')




