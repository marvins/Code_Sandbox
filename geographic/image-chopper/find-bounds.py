#!/usr/bin/env python

#  Import GDAL
import os, sys, argparse, math

from osgeo import gdal
from osgeo.gdalconst import *

#  Parse Command-Line
def Parse_Command_Line():

    #  create parser
    parser = argparse.ArgumentParser(description='Find Bounds of Datasets.')

    parser.add_argument('-d','--directory',
                        dest='base_path',
                        required=True,
                        help='Provide base path to start searching from.')

    #  Tile Size
    parser.add_argument('-tsm','--tile-size-meters',
                        dest='tile_size_meters',
                        required=False,
                        default=10000,
                        type=float,
                        help='Size of Tile in Meters')

    #  Image GSD
    parser.add_argument('-gsd',
                        dest='gsd',
                        required=False,
                        default=0.25,
                        type=float,
                        help='Image GSD in Meters Per Pixel')

    #  Parse args
    return parser.parse_args()


def Find_Images( options, extensions ):

    #  Create output
    output = []

    #  Start walking the directory tree
    for root, dirs, files in os.walk( options.base_path):

        #  Check files
        for bpath in files:

            #  Check Extension
            ext = os.path.splitext(bpath)[-1]
            if ext in extensions:

                #  Join the path
                fpath = os.path.join(root, bpath)

                output.append(fpath)

    return output

def GeoTransform( xform, pix):

    x = xform[1] * pix[0] + xform[2] * pix[1] + xform[0]
    y = xform[1] * pix[0] + xform[4] * pix[1] + xform[3]

    return (x,y)

def Compute_Image_Bounds( img_path, options ):

    #  Initialize Dataset
    dataset = gdal.Open( img_path, GA_ReadOnly )

    #  Get the bounds
    geotransform = dataset.GetGeoTransform()

    rows = dataset.RasterYSize
    cols = dataset.RasterXSize

    #  Compute Corners
    corners = {'tl': GeoTransform(geotransform, (0,0)),
               'tr': GeoTransform(geotransform, (cols, 0)),
               'bl': GeoTransform(geotransform, (0, rows)),
               'br': GeoTransform(geotransform, (cols,rows))}

    return corners

def Union_Bounds( current_bounds, input_bounds ):

    #  Check If None
    if current_bounds is None:
        output = {'min': input_bounds['bl'],
                  'max': input_bounds['tr']}

        return output

    #  Otherwise, check bounds
    min_pnt = map( min, *[current_bounds['min'], input_bounds['bl']])
    max_pnt = map( max, *[current_bounds['max'], input_bounds['tr']])

    return {'min': min_pnt,
            'max': max_pnt}

def Is_Inside( bbox, test_bbox ):

    #  Check if test fully inside
    if bbox['min'][0] < test_bbox['min'][0]:
        return False
    if bbox['min'][1] < test_bbox['min'][1]:
        return False
    if bbox['max'][0] > test_bbox['max'][0]:
        return False
    if bbox['max'][1] > test_bbox['max'][1]:
        return False

    return True


def Chop_Bounds( bounds, tile_size ):

    minX = bounds['min'][0]
    minY = bounds['min'][1]
    maxX = bounds['max'][0]
    maxY = bounds['max'][1]

    #  Compute Differences
    dX = maxX - minX
    dY = maxY - minY

    nX_tiles = dX / tile_size
    nY_tiles = dY / tile_size

    tile_x_cnt = int(math.ceil(nX_tiles))
    tile_y_cnt = int(math.ceil(nY_tiles))

    print('NXT: ' + str(nX_tiles) + ', CNT: ' + str(tile_x_cnt))
    print('NYT: ' + str(nY_tiles) + ', CNT: ' + str(tile_y_cnt))
    raw_input('')

    # Create Output
    output = []

    for tx in range(0, tile_x_cnt):
        for ty in range(0, tile_y_cnt):

            #  Compute Tile Bounds
            tmin_x = tx * tile_size + minX
            tmin_y = ty * tile_size + minY

            tmax_x = min( (tx+1) * tile_size + minX, maxX)
            tmax_y = min( (ty+1) * tile_size + minY, maxY)

            output.append({'min': (tmin_x, tmin_y),
                           'max': (tmax_x, tmax_y)})


    return output

def Main():


    #  Parse the command-line options
    options = Parse_Command_Line()

    #  Create list of authorized extensions
    extensions = ['.tif']


    #  Get list of images
    img_list = Find_Images( options, extensions )
    bounds = None

    for img in img_list:

        #  Compute Image Bounds
        print('Loading: ' + str(img))
        img_bounds = Compute_Image_Bounds( img, options )

        bounds = Union_Bounds(bounds, img_bounds)

    print(bounds)

    #  Chop the Bounds into smaller tiles
    tile_bounds = Chop_Bounds( bounds, options.tile_size_meters )

    for tile in tile_bounds:

        print(tile)


if __name__ == '__main__':
    Main()