#!/usr/bin/env python

#  Import GDAL
import os, sys, argparse, math, subprocess, logging
import multiprocessing
from multiprocessing import Pool


from osgeo import gdal
from osgeo.gdalconst import *


CREATION_OPTIONS='-co TILED=YES -co TFW=YES'
OUTPUT_FORMAT='-of GTiff'

counter     = None
counter_max = None
counter_mtx = None

def Init_Pool( cnt, cnt_max, cnt_mtx):

    global counter, counter_max, counter_mtx
    counter = cnt
    counter_max = cnt_max
    counter_mtx = cnt_mtx


#---------------------------#
#-     Run the command     -#
#---------------------------#
def Run_Command( cmd, fp = None ):

    #  Create the popen
    p = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)

    #  Get output
    while True:

        #  Get output
        line = p.stdout.readline()

        #  Print
        if not line:
            break

        #  Write
        if fp is not None:
            fp.buffer.write(line)
            fp.flush()
        else:
            print(line)


def Image_To_BBox( image ):

    #  Otherwise, check bounds
    min_pnt = map(min, *[image['tl'], image['bl'], image['tr'], image['br']])
    max_pnt = map(max, *[image['bl'], image['br'], image['tr'], image['br']])

    return {'min': min_pnt,
            'max': max_pnt}

def Generate_Output_Pathname( output_base_path, frame_number, imager_number, tx, ty):

    #  Create File
    output_path = output_base_path + '/frame_' + str(frame_number).zfill(5) + '/frame_' + str(frame_number).zfill(5) + '_imager_' + str(imager_number) + '_tile_' + str(tx).zfill(3) + '_' + str(ty).zfill(3) + '.tif'

    return output_path


def Generate_Merged_Output_Pathname( output_base_path, tx, ty):

    #  Create File
    output_path = output_base_path + '/tile_' + str(tx).zfill(3) + '_' + str(ty).zfill(3) + '.tif'

    return output_path



#  Parse Command-Line
def Parse_Command_Line():

    #  create parser
    parser = argparse.ArgumentParser(description='Find Bounds of Datasets.')

    parser.add_argument('-d','--directory',
                        dest='base_path',
                        required=True,
                        help='Provide base path to start searching from.')

    #  Output DIrectory
    parser.add_argument('-o','--output-directory',
                        dest='output_directory',
                        required=True,
                        help='Provide Output Directory.')

    parser.add_argument('-t','--temp-directory',
                        dest='temp_directory',
                        required=True,
                        help='Provide Temp Working Data Directory for Writing.')

    #  Tile Size
    parser.add_argument('-tsm','--tile-size-meters',
                        dest='tile_size_meters',
                        required=False,
                        default=256,
                        type=float,
                        help='Size of Tile in Meters')

    #  Output Format
    parser.add_argument('-of', '--output-format',
                        dest='output_format',
                        required=False,
                        default=OUTPUT_FORMAT,
                        help='Output Image Format')


    #  Creation Options
    parser.add_argument('-co','--creation-options',
                        dest='creation_options',
                        required=False,
                        default=CREATION_OPTIONS,
                        help='GDAL Creation Options.')

    #  Image GSD
    parser.add_argument('-gsd',
                        dest='gsd',
                        required=False,
                        default=0.25,
                        type=float,
                        help='Image GSD in Meters Per Pixel')

    parser.add_argument('-minx',
                        dest='minx',
                        required=False,
                        default=None,
                        type=float,
                        help='Specify the Minimum X Value')

    parser.add_argument('-miny',
                        dest='miny',
                        required=False,
                        default=None,
                        type=float,
                        help='Specify the Minimum Y Value')

    parser.add_argument('-maxx',
                        dest='maxx',
                        required=False,
                        default=None,
                        type=float,
                        help='Specify the Max Y Value')

    parser.add_argument('-maxy',
                        dest='maxy',
                        required=False,
                        default=None,
                        type=float,
                        help='Specify the Max Y Value')

    #  Size of Work Queue
    parser.add_argument('-mqs','--max-queue-size',
                        dest='max_queue_size',
                        default=1000000,
                        type=int,
                        required=False,
                        help='Set the size of the work queue.')

    parser.add_argument('-nt', '-num-threads',
                        dest='num_threads',
                        default=1,
                        type=int,
                        required=False,
                        help='Set the number of threads to process with.')

    #  Skip If Output Exists
    parser.add_argument('-s','--skip-exists',
                        dest='skip_exists',
                        required=False,
                        action='store_true',
                        default=False,
                        help='Skip processing data if output already exists.')

    #  Flag if we wish to skip processing GDAL Translate Commands
    parser.add_argument('--skip-crop',
                        dest='skip_crop',
                        default=False,
                        action='store_true',
                        required=False,
                        help='Skip cropping of images.')

    parser.add_argument('--skip-merge',
                        dest='skip_merge',
                        default=False,
                        action='store_true',
                        required=False,
                        help='Skip merging of crops.')

    #  Request Dry Run
    parser.add_argument('--dry-run',
                        dest='dry_run',
                        default=False,
                        action='store_true',
                        required=False,
                        help='Print commands but don\'t execute.')


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
    y = xform[4] * pix[0] + xform[5] * pix[1] + xform[3]

    return (x,y)

def Compute_Image_Bounds( img_path, options ):

    #  Initialize Dataset
    dataset = gdal.Open( img_path, GA_ReadOnly )

    #  Get the bounds
    geotransform = dataset.GetGeoTransform()

    rows = float(dataset.RasterYSize)
    cols = float(dataset.RasterXSize)

    #  Compute Corners
    corners = {'path': img_path,
               'tl': GeoTransform(geotransform, (0.0,0.0)),
               'tr': GeoTransform(geotransform, (cols, 0.0)),
               'bl': GeoTransform(geotransform, (0.0, rows)),
               'br': GeoTransform(geotransform, (cols,rows))}

    return corners


def Union_Bounds( current_bounds, input_bounds ):

    #  Check If None
    if current_bounds is None:
        output = {'min': input_bounds['bl'],
                  'max': input_bounds['tr']}

        return output

    #  Otherwise, check bounds
    min_pnt = map( min, *[current_bounds['min'], input_bounds['bl'], input_bounds['tl'], input_bounds['br'], input_bounds['tr']])
    max_pnt = map( max, *[current_bounds['max'], input_bounds['bl'], input_bounds['tl'], input_bounds['br'], input_bounds['tr']])

    return {'min': min_pnt,
            'max': max_pnt}

def Intersect_Bounds( current_bounds, input_bounds):

    #  Check If None
    if current_bounds is None:
        output = input_bounds
        return output

    #  Otherwise, Do the Intersection

    #  Check if they intersect at all
    if current_bounds['max'][0] <= input_bounds['min'][0] or input_bounds['max'][0] <= current_bounds['min'][0]:
        return None
    if current_bounds['max'][1] <= input_bounds['min'][1] or input_bounds['max'][1] <= current_bounds['min'][1]:
        return None

    output = {}

    #  Return Intersection
    output['min'] = (max(current_bounds['min'][0], input_bounds['min'][0]),
                     max(current_bounds['min'][1], input_bounds['min'][1]))

    output['max'] = (min(current_bounds['max'][0], input_bounds['max'][0]),
                     min(current_bounds['max'][1], input_bounds['max'][1]))

    return output


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


def Find_Intersecting_Images( tile, image_list ):

    #  Create output list
    output_paths = []
    current_extent = image_list[0]

    #  Iterate over image list
    for image in image_list:

        #  Check for Overlap
        if Intersect_Bounds(tile, Image_To_BBox(image)):
            output_paths.append(image['path'])

    #  Return Results
    return output_paths


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

    print('Number of Tiles (' + str(tile_x_cnt) + ' x ' + str(tile_y_cnt) + ' ) = ' + str(tile_x_cnt*tile_y_cnt))

    # Create Output
    output = []

    for tx in range(0, tile_x_cnt):
        for ty in range(0, tile_y_cnt):

            #  Compute Tile Bounds
            tmin_x = tx * tile_size + minX
            tmin_y = ty * tile_size + minY

            tmax_x = min( (tx+1) * tile_size + minX, maxX)
            tmax_y = min( (ty+1) * tile_size + minY, maxY)

            output.append({'tx': tx,
                           'ty': ty,
                           'min': (tmin_x, tmin_y),
                           'max': (tmax_x, tmax_y),
                           'opath-list': []})


    return output

def Generate_GDAL_Tile_Command( options, tile, image_path):

    bname = os.path.basename(image_path)
    print(bname)
    fid = bname.find('frame-')
    iid = bname.find('imager-')
    frame_number = 0
    imager_number = 0

    if fid < 0 or iid < 0:
        return None, None
    else:
        frame_number  = bname[ fid+6 : fid+11 ]
        imager_number = bname[ iid+7 : iid+9 ]


    #  Create cmd
    cmd = 'gdal_translate '

    #  Add Driver INfo
    cmd += CREATION_OPTIONS + ' ' + OUTPUT_FORMAT

    #  Define NoData
    cmd += ' -a_nodata 0'

    #  Set GSD
    cmd += ' -tr ' + str(options.gsd) + ' ' + str(options.gsd)

    #  Add Tile Bounds
    cmd += ' -projwin ' + str(tile['min'][0]) + ' ' + str(tile['max'][1]) + ' ' + str(tile['max'][0]) + ' ' + str(tile['min'][1]) + ' '

    #  Add Datasets
    output_pathname = Generate_Output_Pathname(options.temp_directory, frame_number, imager_number, tile['tx'], tile['ty'])

    #  Check if output eixsts
    if options.skip_exists is True and os.path.exists(output_pathname):
        print('Output (' + output_pathname + ') already exists.')
        return None, output_pathname

    #  Create output directory
    pdir = os.path.dirname(output_pathname)
    if not os.path.exists(pdir):
        os.makedirs(pdir)

    cmd += ' ' + image_path + ' ' + output_pathname
    return cmd, output_pathname


def Generate_GDAL_Merge_CMD( options, tile, image_list ):

    if len(image_list) <= 0:
        return None

    #  Generate the Output Pathname
    output_pathname = Generate_Merged_Output_Pathname( options.output_directory, tile['tx'], tile['ty'])

    #  Check if output eixsts
    if options.skip_exists is True and os.path.exists(output_pathname):
        print('Output (' + output_pathname + ') already exists.')
        return None

    #  Create output directory
    pdir = os.path.dirname(output_pathname)
    if not os.path.exists(pdir):
        os.makedirs(pdir)

    #  Create Command
    cmd  = 'gdal_merge.py'
    cmd += ' -o ' + output_pathname

    #  Set the Output Format and Creation Options
    cmd += ' ' + options.output_format
    cmd += ' ' + options.creation_options

    #  Define NoData
    cmd += ' -a_nodata 0 -n 0'

    #  Set the PIxel Size
    cmd += ' -ps ' + str(options.gsd) + ' ' + str(options.gsd) + ' '

    for img in image_list:
        cmd += img + ' '
    return cmd

def Extract_Option_Bounds( options, img_bounds ):

    if options.minx is None or options.miny is None or options.maxx is None or options.maxy is None:
        return img_bounds

    return {'min': (min(options.minx, options.maxx), min(options.miny, options.maxy)),
            'max': (max(options.minx, options.maxx), max(options.miny, options.maxy))}


def Run_GDAL_Command( command ):

    #  Print the COmmand
    print(command)
    Run_Command(command)

    # Update Counter
    global counter_max, counter, counter_mtx

    counter_mtx.acquire()
    counter.value += 1
    res = 100 * counter.value/float(counter_max.value)
    counter_mtx.release()
    print("Current Progress: " + str(res))

def Main():

    #  Parse the command-line options
    options = Parse_Command_Line()


    #  Create list of authorized extensions
    extensions = ['.tif']


    #  Get list of images
    img_list = Find_Images( options, extensions )
    bounds = None
    image_bounds_list = []

    for img in img_list:

        #  Compute Image Bounds
        print('Loading: ' + str(img))
        img_bounds = Compute_Image_Bounds( img, options )
        image_bounds_list.append(img_bounds)

        bounds = Union_Bounds(bounds, img_bounds)

    #  If Image Bounds Specified in Options, Override
    bounds = Extract_Option_Bounds( options, img_bounds )

    #  Chop the Bounds into smaller tiles
    tile_bounds = Chop_Bounds( bounds, options.tile_size_meters )


    #  Command-List
    cmd_list = []

    #  Find Images that Fit Inside Each Tile
    for tile in tile_bounds:

        #  Log Tile
        print('Processing Tile: ' + str(tile))

        #  Find list of intersecting Images
        image_list = Find_Intersecting_Images( tile, image_bounds_list)


        #  Iterate over all images
        for img in image_list:

            #  Generate the GDAL Command
            gdal_cmd, opath = Generate_GDAL_Tile_Command( options, tile, img)

            if gdal_cmd:
                cmd_list.append(gdal_cmd)
            if opath:    
                tile['opath-list'].append(opath)


    #  Generate all Tiles
    if options.skip_crop is False and options.dry_run is False:
        print('Number of GDAL Tile Commands: ' + str(len(cmd_list)))
        cnt     = multiprocessing.Value('i', 0)
        cnt_max = multiprocessing.Value('i', max(1, len(cmd_list)))
        cnt_mtx = multiprocessing.Lock()
        pool = Pool(processes=options.num_threads, initializer=Init_Pool, initargs=(cnt, cnt_max, cnt_mtx))
        pool.map(Run_GDAL_Command, cmd_list)
    elif options.dry_run is True:
        for gdal_cmd in cmd_list:
            print(gdal_cmd)
            

    #  Start Generating the GDAL Merge Commands
    cmd_list = []
    for tile in tile_bounds:

        #  Merge Like Frames
        gdal_cmd = Generate_GDAL_Merge_CMD(options, tile, tile['opath-list'])
        if gdal_cmd:
            #Run_Command(gdal_cmd)
            cmd_list.append(gdal_cmd)

    #  Merge all Tiles
    if options.skip_merge is False and options.dry_run is False:
        print('Number of GDAL Merge Commands: ' + str(len(cmd_list)))
        cnt = multiprocessing.Value('i', 0)
        cnt_max = multiprocessing.Value('i', max(1, len(cmd_list)))
        cnt_mtx = multiprocessing.Lock()
        pool = Pool(processes=options.num_threads, initializer=Init_Pool, initargs=(cnt, cnt_max, cnt_mtx))
        pool.map(Run_GDAL_Command, cmd_list)
    elif options.dry_run is True:
        for gdal_cmd in cmd_list:
            print(gdal_cmd)


if __name__ == '__main__':
    Main()
