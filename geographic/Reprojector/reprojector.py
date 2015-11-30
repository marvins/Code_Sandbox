#!/usr/bin/env python2
#
#    File:    reprojector.py
#    Author:  Marvin Smith
#    Date:    11/28/2015
#

#   Import the Reprojection Utils
import rproj


#---------------------------#
#-      Main Function      -#
#---------------------------#
def Main():

    #  Parse Options
    options = rproj.Options()

    #  Convert Shapefile to Proper Format
    if options.config['program_mode'] == 'convert':
        rproj.Convert_Shapefile(options)

    #  Otherwise, Project Files
    elif options.config['program_mode'] == 'project':

        #  Process the Convert
        pass


    #  Otherwise, error
    else:
        raise Exception('Unknown program-mode (' + str(options.config['program_mode']) + ')')


if __name__ == '__main__':
    Main()
