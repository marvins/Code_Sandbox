#!/usr/bin/env python

import os, logging, subprocess

CRS='EPSG:32613'

minx=510635
miny=4297236.6
maxx=518635
maxy=4305236

gsd=0.3
image_max_size_x=2000
image_max_size_y=2000

block_size_x_meters=image_max_size_x * gsd
block_size_y_meters=image_max_size_y * gsd

temp_dir='temp-wms'

output_pathname='wms-image.tif'


#---------------------------#
#-     Run the command     -#
#---------------------------#
def Run_Command( cmd, fp = None ):

    #  Log the output
    logging.info('running: ' + cmd)

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



#---------------------------#
#-      Main Function      -#
#---------------------------#
def Main():

    try:
        os.mkdir(temp_dir)
    except:
        pass

    #  Open the template
    control_data = None
    with open('scripts/gdal-wms.template.xml', 'r') as tmpfile:
        
        #  read all data
        control_data = tmpfile.read()

    #  Iterate over bounds
    tlx=minx
    tly=maxy
    counter = 0

    file_list=[]

    while tlx < maxx:
        while tly > miny:

            #  Compute the endpoints
            brx = min( maxx, tlx + block_size_x_meters )
            bry = max( miny, tly - block_size_y_meters )

            #  Set the new image sizes
            img_size_x = (brx - tlx) / gsd
            img_size_y = (tly - bry) / gsd


            #  Build the WMS String
            active_wms_file = control_data

            #  Start replacing fields
            active_wms_file = active_wms_file.replace('__MIN_X__', str(tlx))
            active_wms_file = active_wms_file.replace('__MIN_Y__', str(bry))
            active_wms_file = active_wms_file.replace('__MAX_X__', str(brx))
            active_wms_file = active_wms_file.replace('__MAX_Y__', str(tly))
            active_wms_file = active_wms_file.replace('__IMG_SIZE_X__', str(img_size_x))
            active_wms_file = active_wms_file.replace('__IMG_SIZE_Y__', str(img_size_y))
            active_wms_file = active_wms_file.replace('__CRS__', CRS )

            #  Write the WMS Files
            opathname = temp_dir + '/gdal-wms-t' + str(counter).zfill(4) +'.xml'
            with open(opathname, 'w') as tmpfile:
                tmpfile.write(active_wms_file)

            file_list.append([counter, opathname])

            # Increment TL
            tly -= block_size_y_meters
            counter += 1

        tly = maxy
        tlx += block_size_x_meters


    #  Iterate over wms xml files
    image_list=[]
    for f in file_list:

        print('Processing Tile: ' + str(f[0]))
        
        oimgname = temp_dir + '/tile_' + str(f[0]) + '.tif'
        image_list.append(oimgname)

        #  Create command
        cmd='gdal_translate -of GTiff -ot Byte ' + f[1] + ' ' + oimgname
        print('    CMD: ' + str(cmd))
        Run_Command( cmd )

    print('Processed ' + str(counter) + ' tiles')

    #  Create Merge Command
    merge_cmd = 'gdal_merge.py -o ' + output_pathname + ' -ot Byte '
    for i in image_list:
        merge_cmd += ' ' + i

    #  Run Command
    print('Merging Images')
    Run_Command( merge_cmd )


if __name__ == '__main__':
    Main()

