#! /usr/bin/env python

import sys, os, shutil

def main():
    
    # check for arguments
    if len(sys.argv) < 2:
        print 'usage: ./deploy.sh  <opencv directory>'
        return 0
    
    # set opencv directory
    opencv_dir = sys.argv[1]

    # start moving files over
    print 'deploying to ' + opencv_dir
    
    #load file moving list
    f = open('deployment_table.txt', 'r')
    table = f.readlines()  
    
    file_items = []
    for line in table:
        file_items = file_items +  line.strip('\n').split(' ')
        
    f.close()

    file_items = filter(lambda a: a != '', file_items)
      
    # for each pair of items, copy the data accordingly
    for x in xrange(len(file_items)/2):
        idx = 2*x
        
        # Check if file is directory or file
        if os.path.isfile(file_items[idx]) == True :
            print 'copying ' + file_items[idx] + '  to  ' + opencv_dir+'/'+file_items[idx+1] + ' as a file'
            shutil.copy( file_items[idx], opencv_dir+'/'+file_items[idx+1])

        elif( os.path.isdir( file_items[idx]) == True ):
            
            print 'copying ' + file_items[idx] + '  to  ' + opencv_dir+'/'+file_items[idx+1] + ' as a directory'
            shutil.rmtree(   opencv_dir+'/'+file_items[idx+1])
            shutil.copytree( file_items[idx], opencv_dir+'/'+file_items[idx+1])


if __name__ == '__main__':
    main()
