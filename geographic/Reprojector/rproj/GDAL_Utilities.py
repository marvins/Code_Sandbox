#    File:    GDAL_Utilities.py
#    Author:  Marvin Smith
#    Date:    11/29/2015
#

#  Python Libraries
from osgeo import gdal, ogr


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
